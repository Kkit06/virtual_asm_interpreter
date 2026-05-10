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
            if (topIndex >= capacity - 1) throw OverflowException();
            data[++topIndex] = value;
        }

        // Return top value, then remove them
        T pop() {
            if (isEmpty()) throw UnderflowException();
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

        // Move Assignment Operator
        Stack& operator=(Stack&& other) noexcept {
            if (this != &other) {
                delete[] data;

                // Transfer data from other
                data = other.data;
                capacity = other.capacity;
                topIndex = other.topIndex;

                // Delete other data
                other.data = nullptr;
                other.capacity = 0;
                other.topIndex = -1;
            }
            return *this;
        }

        // Move Constructor
        Stack(Stack&& other) noexcept : data(other.data), capacity(other.capacity), topIndex(other.topIndex) {
            other.data = nullptr;
            other.capacity = 0;
            other.topIndex = -1;
        }
   
        // Disable copying stack
        Stack(const Stack& other) = delete;
        Stack& operator=(const Stack& other) = delete;

        // Exception classes
        class OverflowException {}; 
        class UnderflowException {};

    private:
        T* data;        // Pointer to stack array
        int capacity;   // Maximum size of the stack array
        int topIndex;   // Index of the top element, -1 if empty
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
            if (currentSize == capacity) resize();
            data[currentSize++] = value;
        }

        // Remove last element, data will not be deleted but replaced in the future
        void pop_back() {
            if (isEmpty()) throw IndexOutOfBoundsException();
            currentSize--;
        }

        // Insert a value into a certain position
        void insert(int index, const T& value) {
            if (index < 0 || index > currentSize) throw IndexOutOfBoundsException();

            if (currentSize == capacity) resize();

            // Shift elements to the right to make a hole
            for (int i = currentSize; i > index; i--) {
                data[i] = data[i - 1];
            }

            data[index] = value;
            currentSize++;
        }

        // Remove the value at index and shift right position to left
        void remove(int index) {
            if (index < 0 || index >= currentSize) throw IndexOutOfBoundsException();

            // Shift elements to the left to fill the gap
            for (int i = index; i < currentSize - 1; i++) {
                data[i] = data[i + 1];
            }

            currentSize--;
        }

        // Replace value at index with new value
        void replace(int index, const T& value) {
            if (index < 0 || index >= currentSize) throw IndexOutOfBoundsException();
            data[index] = value;
        }

        // Overload operator [], when trying to access an invalid/out of bounds index, throws error
        T& operator[](int index) {
            if (index < 0 || index >= currentSize) throw IndexOutOfBoundsException();
            return data[index];
        }

        // Const for reading a vector
        const T& operator[](int index) const {
            if (index < 0 || index >= currentSize) throw IndexOutOfBoundsException();
            return data[index];
        }

        // Move Assignment Operator
        Vector& operator=(Vector&& other) noexcept {
            if (this != &other) {
                delete[] data;

                // Transfer data from other
                data = other.data;
                currentSize = other.currentSize;
                capacity = other.capacity;

                // Delete other data
                other.data = nullptr;
                other.currentSize = 0;
                other.capacity = 0;
            }
            return *this;
        }

        // Move Constructor
        Vector(Vector&& other) noexcept : data(other.data), currentSize(other.currentSize), capacity(other.capacity) {
            other.data = nullptr;
            other.currentSize = 0;
            other.capacity = 0;
        }

        // Returns current number of elements
        int size() const {
            return currentSize;
        }

        // True if vector is empty
        bool isEmpty() const {
            return currentSize == 0;
        }

        // Disable copying vector
        Vector(const Vector& other) = delete;
        Vector& operator=(const Vector& other) = delete;

        // Exception classes
        class IndexOutOfBoundsException {};
        class AllocationFailedException {};

    private:
        T* data;           // Pointer to the dynamic array
        int currentSize;   // Number of elements currently in the vector
        int capacity;      // Total space currently allocated

        // Called when array is full to double the size
        void resize() {
            capacity = (capacity == 0) ? 1 : capacity * 2;

            T* newData = new T[capacity];
            try {
                for (int i = 0; i < currentSize; i++) {
                    newData[i] = data[i];
                }
            } catch (...) {
                delete[] newData;
                throw AllocationFailedException();
            }

            delete[] data;
            data = newData;
        }
};

// ----- MACHINE ARCHITECTURE -----
// Register
class Register {
    public:
        Register() : value(0) {}

        signed char getValue() const { 
            return value; 
        }

        void setValue(signed char newValue) { 
            value = newValue; 
        }

    private:
        signed char value;  // 1-byte signed value stored (-128 to 127)
};

// ----- MAIN -----
int main() {
    return 0;
}