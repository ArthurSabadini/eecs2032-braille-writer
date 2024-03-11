#include "Buffer.h"

template<uint8_t Rows, uint8_t Cols>
void Buffer<Rows, Cols>::copy(const uint8_t src[Rows][Cols], uint8_t dst[Rows][Cols]) {
    for(uint8_t row = 0; row < Rows; row++) 
        for(uint8_t col = 0; col < Cols; col++)
            dst[row][col] = src[row][col];
};

template<uint8_t Rows, uint8_t Cols>
void Buffer<Rows, Cols>::clear(uint8_t dst[Rows][Cols]) {
    for(uint8_t row = 0; row < Rows; row++) 
        for(uint8_t col = 0; col < Cols; col++)
            dst[row][col] = 0;
}

// Function to add an element to the buffer
template<uint8_t Rows, uint8_t Cols>
void Buffer<Rows, Cols>::add(const uint8_t element[Rows][Cols]) {
    if (bufferSize >= capacity) return;
    copy(element, buffer[size()]);
    bufferSize++;
}


// Function to get an element from the buffer at a specific index
template<uint8_t Rows, uint8_t Cols>
void Buffer<Rows, Cols>::get(uint8_t index, uint8_t element[Rows][Cols]) const {
    if (index >= size()) return;  // Index out of bounds
    copy(buffer[index], element);
}

// Function to remove the last element from the buffer
template<uint8_t Rows, uint8_t Cols>
void Buffer<Rows, Cols>::pop() {
    if(size() <= 0) return;
    this->remove(size()-1);
}

// Function to remove an element from the buffer at a specific index
template<uint8_t Rows, uint8_t Cols>
void Buffer<Rows, Cols>::remove(int8_t index) {
    if (index < 0 || index >= size()) return;
    clear(buffer[index]);
    bufferSize--;

    if (index == size() - 1) return;
    for(uint8_t idx = index; idx < size() - 1; idx++) {
        copy(buffer[index+1], buffer[index]);
    }
}

// Overriding assignment
template<uint8_t Rows, uint8_t Cols>
Buffer<Rows, Cols>& Buffer<Rows, Cols>::operator=(const Buffer& other) {
    if (this != &other) {
        // Resize if necessary
        if (capacity != other.capacity) {
            delete[] buffer;
            capacity = other.capacity;
            buffer = new uint8_t[capacity][Rows][Cols]{};
        }

        // Copy the data
        for (uint8_t i = 0; i < other.size(); i++) {
            copy(other.buffer[i], buffer[i]);
        }

        bufferSize = other.bufferSize;
    }

    return *this;
}
