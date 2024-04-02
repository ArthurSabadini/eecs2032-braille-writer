#include <IRdecoder.h>
#include <TimerOne.h>
#include "BrailleConverter.h"
#include "converter.h"

// Braille for lowercase letters
const byte braille[] = {
    B10000000, // a
    B10100000, // b
    B11000000, // c
    B11010000, // d
    B10010000, // e
    B11100000, // f
    B11110000, // g
    B10110000, // h
    B01100000, // i
    B01110000, // j
    B10001000, // k
    B10101000, // l
    B11001000, // m
    B11011000, // n
    B10011000, // o
    B11101000, // p
    B11111000, // q
    B10111000, // r
    B01101000, // s
    B01111000, // t
    B10001100, // u
    B10101100, // v
    B01110100, // w
    B11001100, // x
    B11011100, // y
    B10011100, // z
    B00000000, // space
};


// Initialize IRdecoder instance
IRdecoder irDecoder(IR_PIN);
uint8_t symbol_buffer[3][2]{};
byte serialDataFormat = B0;

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
                irDecoder.input_buffer.get(irDecoder.input_buffer.size()-1, symbol_buffer);
                serialDataFormat = convertToByte(symbol_buffer);

                // Not actuating the solenoids, for testing
                //displayCharacter(serialDataFormat);
                String currentCharacter = brailleConverter.getTextFromSymbol(serialDataFormat);
                Serial.print(currentCharacter);
                //Serial.println(serialDataFormat, HEX);
                delay(DELAY); // Display delay 
                irDecoder.input_buffer.pop();
            }
        }
        exit(0);
    //}
}
