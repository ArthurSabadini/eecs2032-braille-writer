#include <IRdecoder.h>

#define IR_PIN 8

IRdecoder decoder(IR_PIN);

void setup() {
    Serial.begin(9600);
    decoder.setup();
}

void loop() {
    uint32_t signal = decoder.readSignal();
    String state = decoder.getStringfiedState(); 

    Serial.print(signal, HEX);
    Serial.print("\t");
    Serial.println(state);

    delay(1500);
}
