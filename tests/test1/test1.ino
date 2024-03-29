#include <IRremote.h>

#define ACTION_WINDOW_MS 250
#define IR_PIN 2
#define SOL_PIN 8 // Used for direct tests only

#define DS_PIN 4
#define LATCH_PIN 5
#define CLOCK_PIN 6

volatile uint8_t sol_state = LOW;
volatile byte state = B0;

IRrecv irrec(IR_PIN);
decode_results results;

void ISRHandler() {
    volatile int successful = irrec.decode(&results);
    if(!successful) return;

    sol_state = !sol_state;
    sol_state ? state = B01000000: state = B0;

    irrec.resume();
}

void setup() {
    irrec.enableIRIn();

    //pinMode(SOL_PIN, OUTPUT);

    pinMode(DS_PIN, OUTPUT);
    pinMode(CLOCK_PIN, OUTPUT);
    pinMode(LATCH_PIN, OUTPUT);

    attachInterrupt(digitalPinToInterrupt(IR_PIN), ISRHandler, CHANGE);
}

void loop() {
    //digitalWrite(SOL_PIN, sol_state);

    digitalWrite(LATCH_PIN, LOW);
    shiftOut(DS_PIN, CLOCK_PIN, LSBFIRST, state);
    digitalWrite(LATCH_PIN, HIGH);

    delay(ACTION_WINDOW_MS);
}
