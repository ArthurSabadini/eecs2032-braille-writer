#include <IRdecoder.h>

#define IR_PIN 2

Buffer<3, 2> buff(BUFFER_SIZE);
IRdecoder decoder(IR_PIN);

void setup() {
    Serial.begin(9600);
    decoder.setup();
}

void loop() {
    buff = decoder.receiveInput();
    //String state = decoder.getStringfiedState(); 
    //Serial.println(state);
    //delay(500);
    /*
    uint32_t signal = decoder.readSignal();

    Serial.print(signal, HEX);
    Serial.print("\t");
    Serial.println(state);

    delay(500);
    */
}
