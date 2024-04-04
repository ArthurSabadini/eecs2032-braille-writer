#ifndef IRDECODER_H
#define IRDECODER_H

#include <Arduino.h>
#include <IRremote.h>

// Custom input buffer definition
#ifndef BUFFERIO_H
#define BUFFERIO_H

template<uint8_t Rows, uint8_t Cols>
class BufferIO {
private:
    uint8_t (*buffer)[Rows][Cols];
    uint8_t bufferSize;
    uint8_t capacity;

    void copy(const uint8_t src[Rows][Cols], uint8_t dst[Rows][Cols]);
    void clear(uint8_t dst[Rows][Cols]);

public:
    BufferIO(uint8_t size) : capacity(size), bufferSize(0) {
        this->buffer = new uint8_t[size][Rows][Cols]{};
    };

    ~BufferIO() { delete[] buffer; };

    String getStringfiedState(uint8_t idx);
    void add(const uint8_t element[Rows][Cols]);
    uint8_t size() const { return bufferSize; }
    bool empty() { return bufferSize == 0; }
    void get(uint8_t index, uint8_t element[Rows][Cols]) const;
    void pop_back();
    void pop_front();
    void remove(int8_t index);
    
    // Overriding assignment
    BufferIO& operator=(const BufferIO& other);
};

template<uint8_t Rows, uint8_t Cols>
void BufferIO<Rows, Cols>::copy(const uint8_t src[Rows][Cols], uint8_t dst[Rows][Cols]) {
    for (uint8_t row = 0; row < Rows; row++)
        for (uint8_t col = 0; col < Cols; col++)
            dst[row][col] = src[row][col];
}

template<uint8_t Rows, uint8_t Cols>
void BufferIO<Rows, Cols>::clear(uint8_t dst[Rows][Cols]) {
    for (uint8_t row = 0; row < Rows; row++)
        for (uint8_t col = 0; col < Cols; col++)
            dst[row][col] = 0;
}

// Again, mainly for debugging purposes
template<uint8_t Rows, uint8_t Cols>
String BufferIO<Rows, Cols>::getStringfiedState(uint8_t idx) {
    if(idx >= bufferSize) return "";
    String state = "[";

    for(uint8_t i = 0; i < Rows; i++) {
        state += "[";
        for(uint8_t j = 0; j < Cols; j++) {
            state += String(buffer[idx][i][j]);
            if (j < Cols - 1) state += ", ";
        }
        state += (i == Rows - 1) ? "]" : "], ";
    } 
    state += "]";
    return state;
}

template<uint8_t Rows, uint8_t Cols>
void BufferIO<Rows, Cols>::add(const uint8_t element[Rows][Cols]) {
    if (bufferSize >= capacity) return;
    copy(element, buffer[bufferSize]);
    bufferSize++;
}

template<uint8_t Rows, uint8_t Cols>
void BufferIO<Rows, Cols>::get(uint8_t index, uint8_t element[Rows][Cols]) const {
    if (index >= bufferSize) return;  // Index out of bounds
    copy(buffer[index], element);
}

template<uint8_t Rows, uint8_t Cols>
void BufferIO<Rows, Cols>::pop_back() {
    if (bufferSize <= 0) return;
    remove(bufferSize - 1);
}

template<uint8_t Rows, uint8_t Cols>
void BufferIO<Rows, Cols>::pop_front() {
    if (bufferSize <= 0) return;
    remove(0);
}


template<uint8_t Rows, uint8_t Cols>
void BufferIO<Rows, Cols>::remove(int8_t index) {
    if (index < 0 || index >= size()) return;
    clear(buffer[index]);
    bufferSize--;

    if (index == bufferSize - 1) return;
    for (uint8_t idx = index; idx < size() - 1; idx++) {
        copy(buffer[idx+1], buffer[idx]);
    }
}

template<uint8_t Rows, uint8_t Cols>
BufferIO<Rows, Cols>& BufferIO<Rows, Cols>::operator=(const BufferIO& other) {
    if (this != &other) {
        // Resize if necessary
        if (capacity != other.capacity) {
            delete[] buffer;
            capacity = other.capacity;
            buffer = new uint8_t[capacity][Rows][Cols]{};
        }

        // Copy the data
        for (uint8_t i = 0; i < other.size(); i++) {
            copy(other.buffer[i], buffer[i]);
        }

        bufferSize = other.bufferSize;
    }

    return *this;
}

#endif

// Config Definitions
#define ACTION_WINDON_MS 250
#define ACTION_WINDOW_US 250000
#define BUFFER_SIZE 11 
#define ROWS 7
#define COLS 3

// Remote Buttons Code
#define POWER 0xFFA25D
#define VOL_PLUS 0xFF629D
#define FUNC_STOP 0xFFE21D
#define REWIND 0xFF22DD
#define PAUSE 0xFF02FD
#define FAST_FORWARD 0xFFC23D
#define DOWN 0xFFE01F
#define VOL_MINUS 0xFFA857
#define UP 0xFF906F
#define ZERO 0xFF6897
#define EQ 0xFF9867
#define ST_REPT 0xFFB04F
#define ONE 0xFF30CF
#define TWO 0xFF18E7
#define THREE 0xFF7A85
#define FOUR 0xFF10EF
#define FIVE 0xFF38C7
#define SIX 0xFF5AA5
#define SEVEN 0xFF42BD
#define EIGHT 0xFF4AB5
#define NINE 0xFF52AD
#define UNDEFINED 0xFFFFFF

// Main class definition
class IRdecoder {
    public: 
        BufferIO<3, 2> input_buffer;
        bool isInInputMode = false;

        // Buttons defined for I/O functionality
        const static uint32_t NEXT_WORD = FAST_FORWARD;
        const static uint32_t DELETE_WORD = REWIND;
        const static uint32_t CONCLUDE = POWER;

        IRdecoder(uint8_t pin) : pin(pin), input_buffer(BUFFER_SIZE), irrec(pin), results() {};

        void setup();
        void beginReceiveInput();
        void resetState();
        String getStringfiedState();
        String getStringfiedSymbol();

        static void ISRHandler() {
            if(!currentInstance) return;
            currentInstance->inputInterruptHandler();
        }

    private:
        static IRdecoder* currentInstance;
        bool wasInitialized = false;
        bool isActive = false;

        uint8_t pin;
        uint8_t control_state[ROWS][COLS]{};
        uint8_t symbol[3][2]{};

        IRrecv irrec;
        decode_results results;

        void endReceiveInput();
        volatile uint32_t readSignal(); // volatile used, since input could change

        void updateCurrentSymbol();
        uint32_t indexToButtonVal(uint8_t index1, uint8_t index2);
        uint8_t* buttonValToIndex(uint32_t buttonVal); // Don't forget to free memory when not needed
        void inputInterruptHandler();
};

#endif
