#ifndef IRDECODER_H
#define IRDECODER_H

#include <Arduino.h>
#include <IRremote.h>

#define ACTION_WINDON_MS 250
#define ROWS 7
#define COLS 3
#define POWER 0xFFA25D
#define VOL_PLUS 0xFF629D
#define FUNC_STOP 0xFFE21D
#define REWIND 0xFF22DD
#define PAUSE 0xFF02FD
#define FAST_FORWARD 0xFFC23D
#define DOWN 0xFFE01F
#define VOL_MINUS 0xFFA857
#define UP 0xFF906F
#define ZERO 0xFF6897
#define EQ 0xFF9867
#define ST_REPT 0xFFB04F
#define ONE 0xFF30CF
#define TWO 0xFF18E7
#define THREE 0xFF7A85
#define FOUR 0xFF10EF
#define FIVE 0xFF38C7
#define SIX 0xFF5AA5
#define SEVEN 0xFF42BD
#define EIGHT 0xFF4AB5
#define NINE 0xFF52AD

class IRdecoder {
    public: 
        uint8_t pin;
        uint8_t control_state[ROWS][COLS];

        IRdecoder(uint8_t pin);

        //char* getButtonsPressed(); // To be implemented
        void setup();
        uint32_t readSignal(); // Receive and Read signals
        String getStringfiedState();
        void resetState();

    private:
        IRrecv* irrec;
        decode_results* results;

        uint32_t indexToButtonVal(uint8_t index1, uint8_t index2);
        uint8_t* buttonValToIndex(uint32_t buttonVal); // Don't forget to free memory when not needed!
};

#endif
