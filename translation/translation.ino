#include <IRdecoder.h>
#include <TimerOne.h>
#include <LiquidCrystal.h>

#include "BrailleConverter.h"
#include "utils.h" // Add any other functions or definitions in utils.h

// Initialize IRdecoder instance
IRdecoder irDecoder(IR_PIN);
LiquidCrystal lcd(RS_PIN, EN_PIN, D4_PIN, D5_PIN, D6_PIN, D7_PIN);

// Dynamic values used throughout the code
uint8_t symbol_buffer[3][2] {};
byte serialDataFormat = B0;
int delayValue = 0;

// We use this interrupt to print input to LCD display
void bufferPrintISR() {
    // Update LCD display
    String state = irDecoder.getStringfiedSymbol();

    // Writing input state
    if(!irDecoder.input_buffer.empty()) lcd.clear();
    lcd.setCursor(0, 1);
    lcd.print("Braille: " + state);

    // Only write message if input is confirmed
    if(irDecoder.input_buffer.empty()) return;

    String message = "";
    uint8_t size = irDecoder.input_buffer.size();

    // Translation input to english
    for(uint8_t idx = 0; idx < size; idx++) {
        irDecoder.input_buffer.get(idx, symbol_buffer);
        serialDataFormat = convertToByte(symbol_buffer);
        String currentCharacter = brailleConverter.getTextFromSymbol(serialDataFormat);
        message += currentCharacter;
    }

    // Writing english message
    lcd.setCursor(0, 0);
    lcd.print("Text: " + message);

    serialDataFormat = B0;
}

void setup() {
    // No clue why, but if this is removed the final section of the loop is never reached
    Serial.begin(9600); 

    // Initialize IR sensor setup
    irDecoder.setup();

    // Initialize LCD display
    lcd.begin(16, 2);
    lcd.print("Begin Input");

    // Initialize shift register pins
    pinMode(DS_PIN, OUTPUT);
    pinMode(LATCH_PIN, OUTPUT);
    pinMode(CLOCK_PIN, OUTPUT);

    irDecoder.beginReceiveInput();
    solenoidsWriteCharacter(B00000000);

    // Using Timer Interrupt to print input buffer
    Timer1.initialize(ACTION_WINDOW_US);
    Timer1.attachInterrupt(bufferPrintISR);
}

void loop() {
    // Check for IR input and process if available
    if(irDecoder.isInInputMode) return;
    Timer1.stop();

    // Display and process the character
    if(irDecoder.input_buffer.empty()) exit(0);

    lcd.setCursor(0, 1);
    lcd.print("Translating ...");

    // Display characters as the user goes through the incoming characters
    while (!irDecoder.input_buffer.empty()) {
        // Get next element, and translate
        irDecoder.input_buffer.get(0, symbol_buffer);
        byte serialDataFormat = convertToByte(symbol_buffer);

        // Actuating solenoids
        solenoidsWriteCharacter(serialDataFormat);
        irDecoder.input_buffer.pop_front();
    }

    // Turn all solenoids off
    solenoidsWriteCharacter(B00000000);

    // Finishing up and turning off display
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Thank you!");
    delay(1000);
    lcd.clear();
    lcd.noDisplay();

    exit(0);
}
