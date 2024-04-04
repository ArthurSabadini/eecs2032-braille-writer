/*
 *  This file contains the BrailleConverter class, that converts serial data to text.
 * */

#ifndef BRAILLE_CONVERTER_H
#define BRAILLE_CONVERTER_H

#include <Arduino.h> // Only needed to get lsp to work

class BrailleConverter {
    private:
        static const uint8_t SIZE = 27; // Number of total symbols, need to be updated if we add symbols
        struct BrailleTextPair {
            byte symbol;
            String text;
        };

        static BrailleTextPair items[SIZE];

        BrailleConverter() {} // Private Constructor to prevent instantiation
        BrailleConverter(const BrailleConverter&) = delete;
        BrailleConverter& operator=(const BrailleConverter&) = delete;

    public:
        static BrailleConverter& getInstance();
        String getTextFromSymbol(byte symbol) const;
        String getTextFromSymbols(const byte symbols[], uint8_t size) const;
};

BrailleConverter& brailleConverter = BrailleConverter::getInstance();
// Defining each symbol with its correnspondent text
BrailleConverter::BrailleTextPair BrailleConverter::items[] = {
    { B10000000, "a" },
    { B10100000, "b" },
    { B11000000, "c" },
    { B11010000, "d" },
    { B10010000, "e" },
    { B11100000, "f" },
    { B11110000, "g" },
    { B10110000, "h" },
    { B01100000, "i" },
    { B01110000, "j" },
    { B10001000, "k" },
    { B10101000, "l" },
    { B11001000, "m" },
    { B11011000, "n" },
    { B10011000, "o" },
    { B11101000, "p" },
    { B11111000, "q" },
    { B10111000, "r" },
    { B01101000, "s" },
    { B01111000, "t" },
    { B10001100, "u" },
    { B10101100, "v" },
    { B01110100, "w" },
    { B11001100, "x" },
    { B11011100, "y" },
    { B10011100, "z" },
    { B00000000, "_" },
};

BrailleConverter& BrailleConverter::getInstance() {
    static BrailleConverter brailleConverter;
    return brailleConverter;
}

String BrailleConverter::getTextFromSymbol(byte symbol) const {
    String text = "";
    for(uint8_t idx = 0; idx < BrailleConverter::SIZE; idx++) {
        if(symbol != BrailleConverter::items[idx].symbol) continue; 
        text = BrailleConverter::items[idx].text;
        break;
    }

    return text;
}

String BrailleConverter::getTextFromSymbols(const byte symbols[], uint8_t size) const {
    String text = "";
    for(uint8_t idx = 0; idx < size; idx++) 
        text += getTextFromSymbol(symbols[idx]);

    return text;
}

#endif
