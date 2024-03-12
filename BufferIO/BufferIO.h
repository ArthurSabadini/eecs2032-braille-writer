#ifndef BUFFERIO_H
#define BUFFERIO_H

#include <Arduino.h>
//#include <cstdint> //@Delete this when adding to library

template<uint8_t Rows, uint8_t Cols>
class BufferIO {
    private:
        uint8_t (*buffer)[Rows][Cols]; 
        uint8_t bufferSize;
        uint8_t capacity;

        void copy(const uint8_t src[Rows][Cols], uint8_t dst[Rows][Cols]);
        void clear(uint8_t dst[Rows][Cols]);

    public:
        BufferIO(uint8_t size) : capacity(size), bufferSize(0) { 
            buffer = new uint8_t[size][Rows][Cols]{};
        }

        ~BufferIO() { delete[] buffer; }

        void add(const uint8_t element[Rows][Cols]);
        uint8_t size() const { return bufferSize; }
        bool empty() { return size() == 0; }
        void get(uint8_t index, uint8_t element[Rows][Cols]) const;
        void pop();
        
        // Function to remove an element from the buffer at a specific index
        void remove(int8_t index);
        
        // Overriding assignment
        BufferIO& operator=(const BufferIO& other);
};

#endif
