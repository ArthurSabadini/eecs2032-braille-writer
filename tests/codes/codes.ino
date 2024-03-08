#include <IRremote.h>

#define IR_PIN 8

IRrecv irrec(IR_PIN);
decode_results results;

void setup() {
    Serial.begin(9600);
    irrec.enableIRIn();
}

void loop() {
    if(irrec.decode(&results)) {
        Serial.println(results.value, HEX);
        irrec.resume();
    }
}
