#include <IRdecoder.h>
#include <TimerOne.h>
#include "BrailleConverter.h"
#include "utils.h" // Add any other functions or definitions in utils.h

// Initialize IRdecoder instance
IRdecoder irDecoder(IR_PIN);
uint8_t symbol_buffer[3][2]{};
byte serialDataFormat = B0;

// We can use this interrupt to print input to LCD display
void bufferPrintISR() {
    String state = irDecoder.getStringfiedSymbol();
    Serial.println(state);
}

void setup() {
    Serial.begin(9600);
    // Read potentiometer value and map it to delay range (500 to 2000 ms)
    int delayValue = map(analogRead(POT_PIN), 0, 1023, 500, 2000);

    // Initialize IR sensor setup
    irDecoder.setup();

    // Initialize button pin
    //pinMode(BUTTON_PIN, INPUT_PULLUP);

    // Initialize potentiometer pin
    //pinMode(POT_PIN, INPUT);

    // Initialize display pin
    //pinMode(DISPLAY_PIN, OUTPUT);

    // Initialize shift register pins
    pinMode(DS_PIN, OUTPUT);
    pinMode(LATCH_PIN, OUTPUT);
    pinMode(CLOCK_PIN, OUTPUT);

    irDecoder.beginReceiveInput();

    // Using Timer Interrupt to print input buffer
    Timer1.initialize(1000000);
    Timer1.attachInterrupt(bufferPrintISR);
}

void loop() {
    // Check for IR input and process if available
    if(irDecoder.isInInputMode) return;
    Timer1.stop();

    // Check if the button is pressed
    //if (digitalRead(BUTTON_PIN) == LOW) {
        // Display and process the character
        if (!irDecoder.input_buffer.empty()) {
            // Display characters as the user goes through the incoming characters
            while (!irDecoder.input_buffer.empty()) {
                // Get last element
                irDecoder.input_buffer.get(0, symbol_buffer);
                serialDataFormat = convertToByte(symbol_buffer);
                String currentCharacter = brailleConverter.getTextFromSymbol(serialDataFormat);

                // Not actuating the solenoids, for testing
                //displayCharacter(serialDataFormat);
                Serial.print(currentCharacter);
                delay(DELAY); // Display delay 
                irDecoder.input_buffer.pop_front();
            }
        }
        exit(0);
    //}
}
