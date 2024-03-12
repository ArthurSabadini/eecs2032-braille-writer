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
    String state = decoder.getStringfiedState(); 
    Serial.println(state);
    delay(ACTION_WINDON_MS);
}
