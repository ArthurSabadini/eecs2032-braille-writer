#include <IRdecoder.h>

#define IR_PIN 2

IRdecoder decoder(IR_PIN);

void setup() {
    Serial.begin(9600);
    decoder.setup();
    decoder.beginReceiveInput();
}

void loop() {
    // Debugin interrupt functionality
    if(decoder.isInInputMode) {
        // While in input mode, wait for input to be completed 
        String bufferState = decoder.getStringfiedSymbol();

        Serial.print("Current Buffer State: ");
        Serial.println(bufferState);
        delay(ACTION_WINDON_MS);
    } else {
        // Proceed with the program
        Serial.println("Done");
        exit(0);
    }
}
