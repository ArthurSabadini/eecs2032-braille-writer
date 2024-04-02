#ifndef CONVERTER_H
#define CONVERTER_H

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

#include <Arduino.h>

byte convertToByte(uint8_t data[3][2]) {
    byte formattedByte = 0;

    // Convert data to byte format
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 2; j++) {
            formattedByte = (formattedByte << 1) | data[i][j];
        }
    }

    // Add 0x00 at the end; the shift register expects input to be 8-bits
    formattedByte = (formattedByte << 1) | 0;
    formattedByte = (formattedByte << 1) | 0;
    return formattedByte;
}

// Function to display a character on the display
void displayCharacter(byte serialDataFormat) {
  // Send the solenoid pattern via shift register
  digitalWrite(LATCH_PIN, LOW);
  shiftOut(DS_PIN, CLOCK_PIN, LSBFIRST, serialDataFormat); // Only sending the first byte
  digitalWrite(LATCH_PIN, HIGH);

  // Assuming you're using an LED for display
  digitalWrite(DISPLAY_PIN, HIGH);
  delay(DELAY); // Display duration
  digitalWrite(DISPLAY_PIN, LOW);
}

#endif
