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
        Vector(int initialCapacity = 4) : currentSize(0), capacity(initialCapacity) {
            if (capacity < 1) throw NegativeSizeException();
            data = new T[capacity];
        }

        ~Vector() {
            delete[] data;
        }

        // Add an element, if full, call resize
        void push_back(const T& value) {
            if (!data) reinitialize();
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

            if (!data) reinitialize();
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
        class NegativeSizeException {};

    private:
        T* data;           // Pointer to the dynamic array
        int currentSize;   // Number of elements currently in the vector
        int capacity;      // Total space currently allocated

        // Reset null vector to default 
        void reinitialize() {
            capacity = 4;
            currentSize = 0;
            data = new T[capacity];
        }

        // Called when array is full to double the size
        void resize() {
            int newCapacity = (capacity == 0) ? 1 : capacity * 2;
            T* newData = new T[newCapacity];
            
            for (int i = 0; i < currentSize; i++) {
                newData[i] = data[i];
            }

            delete[] data;
            data = newData;
            capacity = newCapacity;
        }
};

// Queue (First In, First Out)
template <typename T>
class Queue {
    public:
        Queue(int initialCapacity = 4) : data(initialCapacity), head(0), tail(0), count(0), capacity(initialCapacity) {}

        // Add data into queue
        void enqueue(const T& value) {
            if (isFull()) resize();

            if (data.size() < capacity) {
                data.push_back(value);
            } else {
                data.replace(tail, value);
            }

            tail = (tail + 1) % capacity; // Wrap around if reach end
            count++;
        }

        // Remove data into queue
        T dequeue() {
            if (isEmpty()) throw typename Vector<T>::IndexOutOfBoundsException();

            T value = data[head];
            head = (head + 1) % capacity; // Wrap around if reach end
            count--;
            return value;
        }

        int getSize() const {
            return count;
        }

        int getCapacity() const {
            return capacity;
        }

        bool isFull() const {
            return count == capacity;
        }

        bool isEmpty() const {
            return count == 0;
        }

        // Disable copying queue
        Queue(const Queue& other) = delete;
        Queue& operator=(const Queue& other) = delete;

    private:
        Vector<T> data;     // Vector to store the queue
        int head;           // Index of the front element
        int tail;           // Index where the next element will be inserted
        int count;          // Current number of elements
        int capacity;       // Total space currently allocated

        void resize() {
            int newCapacity = capacity * 2;
            Vector<T> newData(newCapacity);

            // Manually transfer elements in linear order
            for (int i = 0; i < count; i++) {
                newData.push_back(data[(head + i) % capacity]);
            }

            data = static_cast<Vector<T>&&>(newData);
            capacity = newCapacity;
            head = 0;
            tail = count;
        }
};

// ----- MAIN -----
int main() {
    return 0;
}