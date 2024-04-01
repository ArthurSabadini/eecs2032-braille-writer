#include <IRdecoder.h>

// Define pins
#define BUTTON_PIN 11 // Button pin for triggering actions
#define DISPLAY_PIN 13 // LED for display
#define POT_PIN A0 // Potentiometer pin

// Shift Register Connections
#define DS_PIN 4
#define LATCH_PIN 5
#define CLOCK_PIN 6

// InfraRed Module Connection
#define IR_PIN 2
#define DELAY 250

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

byte convertToByte(uint8_t data[3][2]) {
    byte formattedByte = 0;

    // Convert data to byte format
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 2; j++) {
            formattedByte = (formattedByte << 1) | data[i][j];
        }
    }

    // Add 0x00 at the end, since the shift register expects input to be 8-bits
    formattedByte = (formattedByte << 1) | 0;
    formattedByte = (formattedByte << 1) | 0;
    return formattedByte;
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
}

void loop() {
    // Check for IR input and process if available
    if(irDecoder.isInInputMode) {
        String state = irDecoder.getStringfiedSymbol();
        Serial.println(state);
        //delay(DELAY);
        return;
    }

    // Check if the button is pressed
    //if (digitalRead(BUTTON_PIN) == LOW) {
        // Display and process the character
        if (!irDecoder.input_buffer.empty()) {
            // Display characters as the user goes through the incoming characters
            while (!irDecoder.input_buffer.empty()) {
                irDecoder.input_buffer.get(irDecoder.input_buffer.size()-1, symbol_buffer);
                serialDataFormat = convertToByte(symbol_buffer);

                // Not actuating the solenoids, for testing
                //displayCharacter(serialDataFormat);
                Serial.println(serialDataFormat, HEX);
                delay(DELAY); // Display delay 
                irDecoder.input_buffer.pop();
            }
        }
        exit(0);
    //}
}

// Function to display a character on the display
void displayCharacter(byte serialDataFormat) {
  // Send the solenoid pattern via shift register
  digitalWrite(LATCH_PIN, LOW);
  shiftOut(DS_PIN, CLOCK_PIN, LSBFIRST, serialDataFormat); // Only sending the first byte
  digitalWrite(LATCH_PIN, HIGH);

  // Assuming you're using an LED for display
  digitalWrite(DISPLAY_PIN, HIGH);
  delay(500); // Display duration
  digitalWrite(DISPLAY_PIN, LOW);
}
