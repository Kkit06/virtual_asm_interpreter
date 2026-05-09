#include <iostream>

// ----- CUSTOM LIBRARY -----
// Stack (Last In, First Out)
template <typename T, int Size = 8>
class Stack {
    static_assert(Size > 0, "Stack size must be positive!");

    public:
        Stack() : capacity(Size), topIndex(-1) {
            data = new T[capacity];
        }

        ~Stack() {
            delete[] data;
        }

        // Stores value into the stack
        void push(const T& value) {
            if (topIndex >= capacity - 1) {
                throw OverflowException();
            }
            data[++topIndex] = value;
        }

        // Return top value, then remove them
        T pop() {
            if (isEmpty()) {
                throw UnderflowException();
            }
            return data[topIndex--];
        }

        int getTopIndex() const {
            return topIndex;
        }
        
        int getCapacity() const {
            return capacity;
        }

        bool isEmpty() const {
            return topIndex == -1;
        }

        // Exception classes
        class OverflowException {}; 
        class UnderflowException {};

    private:
        T* data;        // Pointer to stack array
        int capacity;   // Maximum size of the stack array
        int topIndex;   // Index of the top element, -1 if empty

        // Disable copying stack
        Stack(const Stack& other) = delete;
        Stack& operator=(const Stack& other) = delete;
};

// Vector
template <typename T>
class Vector {
    public:
        Vector() : data(nullptr), currentSize(0), capacity(0) {}

        ~Vector() {
            delete[] data;
        }

        // Add an element, if full, call resize
        void push_back(const T& value) {
            if (currentSize == capacity) {
                resize();
            }
            data[currentSize++] = value;
        }

        // Overload operator [], when trying to access an invalid/out of bounds index, throws error
        T& operator[](int index) {
            if (index < 0 || index >= currentSize) {
                throw IndexOutOfBoundsException();
            }
            return data[index];
        }

        // Returns current number of elements
        int size() const {
            return currentSize;
        }

        // Disable copying stack
        Vector(const Vector& other) = delete;
        Vector& operator=(const Vector& other) = delete;

        // Exception classes
        class IndexOutOfBoundsException {};

    private:
        T* data;           // Pointer to the dynamic array
        int currentSize;   // Number of elements currently in the vector
        int capacity;      // Total space currently allocated

        // Called when array is full to double the size
        void resize() {
            if (capacity == 0) {
                capacity = 1;
            } else {
                capacity = capacity * 2;
            }

            T* newData = new T[capacity];
            for (int i = 0; i < currentSize; i++) {
                newData[i] = data[i];
            }

            delete[] data;
            data = newData;
        }
};

// ----- MAIN -----
int main() {
    return 0;
}