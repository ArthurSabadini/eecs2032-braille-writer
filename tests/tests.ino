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
    String bufferState = decoder.getStringfiedSymbol();
    
    Serial.print("Current Buffer State: ");
    Serial.println(bufferState);

   // delay(ACTION_WINDON_MS);
   delay(2000);
}
