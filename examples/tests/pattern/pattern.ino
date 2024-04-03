#include <IRdecoder.h>

#define DELAY 250
#define IR_PIN 2

// Shift Register Connections
#define DS_PIN 4
#define LATCH_PIN 6
#define CLOCK_PIN 8

IRrecv recv(IR_PIN);
decode_results results;

const byte pattern1[6] = { B10000000, B01000000, B00100000, B00010000, B00001000, B00000100};
const byte pattern2[3] = { B11000000, B00110000, B00001100};
volatile bool begin = false;

void ISRHandler() {
    volatile int success = recv.decode(&results);
    if(!success) return;

    begin = !begin;
    recv.resume();
}

void setup() {
    recv.enableIRIn();

    pinMode(DS_PIN, OUTPUT);
    pinMode(CLOCK_PIN, OUTPUT);
    pinMode(LATCH_PIN, OUTPUT);

    attachInterrupt(digitalPinToInterrupt(2), ISRHandler, CHANGE);
}

void writeSerialData(byte data) {
    digitalWrite(LATCH_PIN, LOW);
    shiftOut(DS_PIN, CLOCK_PIN, LSBFIRST, data); // Only sending the first byte
    digitalWrite(LATCH_PIN, HIGH);
}

void loop() {
    if(!begin) return;

    for(int index = 0; index < 6; index++) {
        writeSerialData(pattern1[index]);
        delay(DELAY);
    }

    writeSerialData(B00000000);
    delay(DELAY);
    exit(0);
}
