/* This file is used to define some useful functions, definitions and macros */

#ifndef CONVERTER_H
#define CONVERTER_H

#include <Arduino.h> // Only needed for lsp

// Shift Register Connections
#define DS_PIN 4
#define LATCH_PIN 6
#define CLOCK_PIN 8

// LCD Connections
#define RS_PIN 33
#define EN_PIN 37
#define D4_PIN 41
#define D5_PIN 45
#define D6_PIN 49
#define D7_PIN 53


// InfraRed Module Connection
#define IR_PIN 2
#define DELAY 500
#define SPACING_DELAY 250

byte convertToByte(uint8_t data[3][2]) {
    byte formattedByte = 0;

    // Convert data to byte format
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 2; j++) {
            formattedByte = (formattedByte << 1) | data[i][j];
        }
    }

    // Add 0x00 at the end; the shift register expects input to be 8-bits
    formattedByte = (formattedByte << 1);
    formattedByte = (formattedByte << 1);
    return formattedByte;
}

// Function to display a character on the display
void solenoidsWriteCharacter(byte serialDataFormat) {
  // Send the solenoid pattern via shift register
  digitalWrite(LATCH_PIN, LOW);
  shiftOut(DS_PIN, CLOCK_PIN, LSBFIRST, serialDataFormat); // Only sending the first byte
  digitalWrite(LATCH_PIN, HIGH);
  delay(DELAY);

  // Write all solenoids low for safety, and characters transition
  digitalWrite(LATCH_PIN, LOW);
  shiftOut(DS_PIN, CLOCK_PIN, LSBFIRST, B00000000); // Only sending the first byte
  digitalWrite(LATCH_PIN, HIGH);
  delay(SPACING_DELAY);
}

#endif
