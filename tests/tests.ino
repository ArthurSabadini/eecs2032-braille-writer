#include <IRremote.h>
#include <../IRdecoder/headers/IRdecoder.h>

#define IR_PIN 8

IRdecoder decode(IR_PIN);

void setup() {
    Serial.begin(9600);
    decode.setup();
}

void loop() {
    uint32_t signal = decode.readSignal();
    String state = decode.getStringfiedState(); 

    Serial.print(signal, HEX);
    Serial.print("\t");
    Serial.println(state);

    delay(1500);
}
