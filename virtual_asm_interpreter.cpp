#include <iostream>

// ----- CUSTOM LIBRARY -----
// Stack (Last In, First Out)
template <typename T>
class Stack {
    public:
        Stack(int size = 8) : capacity(size) {
            data = new T[capacity];
            topIndex = -1;
        }

        ~Stack() {
            delete[] data;
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

    private:
        T* data;        // Pointer to stack array
        int capacity;   // Maximum size of the stack array
        int topIndex;   // Index of the top element, -1 if empty

        // Disable copying stack
        Stack(const Stack& other) = delete;
        Stack& operator=(const Stack& other) = delete;
};

// ----- MAIN -----
int main() {
    return 0;
}