#include "IRdecoder.h"
#include "mega/pins_arduino.h" //@Delete this when adding to library

IRdecoder* IRdecoder::currentInstance = nullptr;

void IRdecoder::setup() {
    irrec.enableIRIn();
    wasInitialized = true;
    isActive = true;
}

void IRdecoder::beginReceiveInput() {
    if(!wasInitialized) return;
    if(!isActive) { irrec.resume(); isActive = true; }

    // This was needed to convert inputInterruptHandler to void(*)(), as required for attachInterrupt
    isInInputMode = true;
    currentInstance = this;
    attachInterrupt(digitalPinToInterrupt(pin), ISRHandler, CHANGE);
}

void IRdecoder::endReceiveInput() {
    detachInterrupt(pin);
    irrec.disableIRIn();

    currentInstance = nullptr;
    isActive = false;
    isInInputMode = false;
}

// Mainly for debug purposes
String IRdecoder::getStringfiedState() {
    String state = "[";

    for(uint8_t i = 0; i < ROWS; i++) {
        state += "[";
        for(uint8_t j = 0; j < COLS; j++) {
            state += String(control_state[i][j]);
            if (j < COLS - 1) state += ", ";
        }
        state += (i == ROWS - 1) ? "]" : "], ";
    } 
    state += "]";
    return state;
}

// mainly for debug pusposes
String IRdecoder::getStringfiedSymbol() {
    String state = "";

    for(uint8_t i = 0; i < 3; i++) {
        for(uint8_t j = 0; j < 2; j++) {
            state += String(symbol[i][j]);
        }
    } 
    return state;
}

// Update the current symbol on input, after control_state update
void IRdecoder::updateCurrentSymbol() {
    // Updating the current braille symbol inputted
    // The symbol input buttons is the lower right 3x2 buttons in the remote control
    for(uint8_t row = 0; row < 3; row++)
        for(uint8_t col = 0; col < 2; col++)
            symbol[row][col] = control_state[ROWS+row-3][COLS+col-2];
}

void IRdecoder::resetState() {
    for(uint8_t i = 0; i < ROWS; i++) {
        for(uint8_t j = 0; j < COLS; j++) {
            control_state[i][j] = 0;
        }
    } 
    updateCurrentSymbol();
}

uint32_t IRdecoder::indexToButtonVal(uint8_t index1, uint8_t index2) {
    // Enumerating binary matrix index to unary. i.e (0,0) -> 0, (0,1) -> 1 ...
    uint8_t matrixIndex = index2 + index1 * COLS; 

    switch(matrixIndex) {
        case 0: return POWER; 
        case 1: return VOL_PLUS;
        case 2: return FUNC_STOP;
        case 3: return REWIND;
        case 4: return PAUSE;
        case 5: return FAST_FORWARD;
        case 6: return DOWN;
        case 7: return VOL_MINUS;
        case 8: return UP;
        case 9: return ZERO;
        case 10: return EQ;
        case 11: return ST_REPT;
        case 12: return ONE;
        case 13: return TWO;
        case 14: return THREE;
        case 15: return FOUR;
        case 16: return FIVE;
        case 17: return SIX;
        case 18: return SEVEN;
        case 19: return EIGHT;
        case 20: return NINE;
        default: return UNDEFINED;
    }
}

uint8_t* IRdecoder::buttonValToIndex(uint32_t buttonVal) {
    switch (buttonVal) {
        case POWER: return new uint8_t[2] {0, 0};
        case VOL_PLUS: return new uint8_t[2] {0, 1};
        case FUNC_STOP: return new uint8_t[2] {0, 2};
        case REWIND: return new uint8_t[2] {1, 0};
        case PAUSE: return new uint8_t[2] {1, 1};
        case FAST_FORWARD: return new uint8_t[2] {1, 2};
        case DOWN: return new uint8_t[2] {2, 0};
        case VOL_MINUS: return new uint8_t[2] {2, 1};
        case UP: return new uint8_t[2] {2, 2};
        case ZERO: return new uint8_t[2] {3, 0};
        case EQ: return new uint8_t[2] {3, 1};
        case ST_REPT: return new uint8_t[2] {3, 2};
        case ONE: return new uint8_t[2] {4, 0};
        case TWO: return new uint8_t[2] {4, 1};
        case THREE: return new uint8_t[2] {4, 2};
        case FOUR: return new uint8_t[2] {5, 0};
        case FIVE: return new uint8_t[2] {5, 1};
        case SIX: return new uint8_t[2] {5, 2};
        case SEVEN: return new uint8_t[2] {6, 0};
        case EIGHT: return new uint8_t[2] {6, 1};
        case NINE: return new uint8_t[2] {6, 2};
        default: return nullptr;
    }
}

volatile uint32_t IRdecoder::readSignal() {
    // Reading input signal (button pressed) one by one
    volatile int successful = irrec.decode(&results);
    if(!successful) return UNDEFINED;

    // Decode signal and save
    uint8_t* indexes = buttonValToIndex(results.value);
    if(indexes) control_state[indexes[0]][indexes[1]] = 1; // Save activation

    irrec.resume();
    delete[] indexes;
    return results.value;
}

void IRdecoder::inputInterruptHandler() {
    volatile uint32_t buttonPressed = readSignal();
    if(buttonPressed == UNDEFINED) return;

    switch(buttonPressed) {
        case IRdecoder::NEXT_WORD:
            // Add next word to symbols list
            input_buffer.add(symbol);
            resetState();
            break; 
        case IRdecoder::DELETE_WORD:
            // Delete current word
            if(!input_buffer.empty()) input_buffer.pop_back(); 
            resetState();
            break;
        case IRdecoder::CONCLUDE:
            // Complete input, save and exit
            resetState();
            endReceiveInput();
            break;
        default: 
            updateCurrentSymbol();
            break;
    }
}
