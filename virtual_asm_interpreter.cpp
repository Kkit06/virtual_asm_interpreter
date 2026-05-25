#include <iostream>
#include <string>

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
            if (topIndex >= static_cast<int>(capacity) - 1) throw OverflowException();
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
        T* data;            // Pointer to stack array
        size_t capacity;    // Maximum size of the stack array
        int topIndex;       // Index of the top element, -1 if empty
};

// Vector
template <typename T>
class Vector {
    public:
        Vector(int initialCapacity = 4) : currentSize(0), capacity(initialCapacity) {
            if (capacity < 1) throw NegativeSizeException();
            data = new T[capacity];
        }

        // Constructor for pre-filling the vector
        Vector(int size, const T& defaultValue) : currentSize(size), capacity(size) {
            if (size < 0) throw NegativeSizeException();
            data = new T[capacity];
            
            for (size_t i = 0; i < currentSize; i++) {
                data[i] = defaultValue;
            }
        }

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
            for (size_t i = currentSize; i > index; i--) {
                data[i] = data[i - 1];
            }

            data[index] = value;
            currentSize++;
        }

        // Remove the value at index and shift right position to left
        void remove(int index) {
            if (index < 0 || index >= currentSize || currentSize == 0) throw IndexOutOfBoundsException();

            // Shift elements to the left to fill the gap
            for (size_t i = index; i < currentSize - 1; i++) {
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
        class OverflowException {};

    private:
        T* data;                // Pointer to the dynamic array
        size_t currentSize;     // Number of elements currently in the vector
        size_t capacity;        // Total space currently allocated

        // Reset null vector to default 
        void reinitialize() {
            delete[] data;
            capacity = 4;
            currentSize = 0;
            data = new T[capacity];
        }

        // Called when array is full to double the size
        void resize() {
            if (capacity > 1073741823) throw OverflowException();

            size_t newCapacity = (capacity == 0) ? 1 : capacity * 2;
            T* newData = new T[newCapacity];
            
            for (size_t i = 0; i < currentSize; i++) {
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
        Queue(int initialCapacity = 4) : data(initialCapacity, T()), head(0), tail(0), count(0), capacity(initialCapacity) {}
        
        // Add data into queue
        void enqueue(const T& value) {
            if (isFull()) resize();

            data.replace(tail, value);
            tail = (tail + 1) % capacity; // Wrap around if reach end
            count++;
        }

        // Remove data into queue
        T dequeue() {
            if (isEmpty()) throw EmptyQueueException();

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

        // Exception classes
        class EmptyQueueException {};
        class OverflowException {};

    private:
        Vector<T> data;     // Vector to store the queue
        int head;           // Index of the front element
        int tail;           // Index where the next element will be inserted
        int count;          // Current number of elements
        int capacity;       // Total space currently allocated

        void resize() {
            if (capacity > 1073741823) throw OverflowException();

            int newCapacity = capacity * 2;
            Vector<T> newData(newCapacity, T());

            // Manually transfer elements in linear order
            for (size_t i = 0; i < count; i++) {
                newData.replace(i, data[(head + i) % capacity]);
            }

            data = static_cast<Vector<T>&&>(newData);
            capacity = newCapacity;
            head = 0;
            tail = count;
        }
};

// ----- MACHINE ARCHITECTURE -----
// Forward declaration
class Instruction;

// Register
class Register {
    public:
        Register() : value(0) {}

        virtual ~Register() {}

        signed char getValue() const { 
            return value; 
        }

        void setValue(signed char newValue) { 
            value = newValue; 
        }

        // Get register value in 8 bit binary
        void getBinary(int bits[8]) const {
            unsigned char val = static_cast<unsigned char>(value);
            for (int i = 0; i < 8; i++) {
                bits[7 - i] = (val >> i) & 1; // Move and check if they are 1
            }
        }

        // Used to update ZF
        bool isZero() const { 
            return value == 0;
        }

    private:
        signed char value;  // 1-byte signed value stored (-128 to 127)
};

// R0-R7 registers 
class GeneralRegister : public Register {
    public:
        GeneralRegister(int id) : Register(), registerID(id) {}

        int getID() const { 
            return registerID; 
        }

    private:
        int registerID;     // ID of the general register
};

// Flag Register
class FlagRegister {
    private:
        bool OF; // Overflow flag
        bool UF; // Underflow flag
        bool CF; // Carry flag
        bool ZF; // Zero flag

    public:
        FlagRegister() : OF(false), UF(false), CF(false), ZF(false) {}

        // Set value functions
        void setOF(bool val) { OF = val; }
        void setUF(bool val) { UF = val; }
        void setCF(bool val) { CF = val; }
        void setZF(bool val) { ZF = val; }

        // Get value functions
        bool getOF() const { return OF; }
        bool getUF() const { return UF; }
        bool getCF() const { return CF; }
        bool getZF() const { return ZF; }

        // For RESET instruction to reset all flags
        void resetFlags() {
            OF = UF = CF = ZF = false;
        }
};

// Memory array
class Memory {
    private:
        Vector<signed char> data;   // The array of 64 byte memory

    public:
        Memory() : data(64, 0) {} // Create data vector with 64 zeros

        // To detect whether array index is over or not
        signed char getByte(int address) const {
            if (address < 0 || address >= 64) throw IndexOutOfBoundsException();
            return data[address];
        }

        // Sets data at address when it is valid
        void setByte(int address, signed char value) {
            if (address < 0 || address >= 64) throw IndexOutOfBoundsException();
            data.replace(address, value);
        }

        // Exception classes
        class IndexOutOfBoundsException {};
};

// CPU
class CPU {
    private:
        GeneralRegister registers[8];       // 8 general registers, in a array
        FlagRegister* flags;                // Flag register
        Memory memory;                      // 64 byte of memory
        Stack<signed char, 8> stack;        // 8 byte of stack
        signed char PC;                     // Program counter
        signed char SI;                     // Stack index
         
    public:
        CPU(FlagRegister* externalFlags) : flags(externalFlags), PC(0), SI(0),
            registers{GeneralRegister(0), GeneralRegister(1), GeneralRegister(2), GeneralRegister(3),
                      GeneralRegister(4), GeneralRegister(5), GeneralRegister(6), GeneralRegister(7)} {}

        // Get reference to the specific general register.
        GeneralRegister& getRegister(int id) { return registers[id]; }

        // Get reference to the CPU's status flags.
        FlagRegister& getFlags() { return *flags; }

        // Get reference to the main memory.
        Memory& getMemory() { return memory; }

        // Get reference to the cpu stack.
        Stack<signed char, 8>& getStack() { return stack; }

        // Get and set the value of the Program Counter.
        int getPC() const { return PC; }
        void setPC(int newPC) { PC = newPC; }

        // Get the value of the stack index.
        int getSI() const { return SI; }

        void run(Vector<Instruction*>& program);
};

// ----- INSTRUCTIONS -----
// Base Instruction Class
class Instruction {
    public:
        // Will be implemented by all children instructions
        virtual void execute(CPU& cpu) = 0; // Pending implementation of CPU class 

        // Clean up all children instructions
        virtual ~Instruction() {}
};

// ADD Instruction
class AddInstruction : public Instruction {
    private:
        int destRegister;
        int srcRegister;

    public:
        AddInstruction(int destId, int srcId) : destRegister(destId), srcRegister(srcId) {}

        void execute(CPU& cpu) override {
            GeneralRegister& dest = cpu.getRegister(destRegister);
            GeneralRegister& src = cpu.getRegister(srcRegister);
            
            int result = dest.getValue() + src.getValue();
            
            dest.setValue(static_cast<signed char>(result));

            // haven't change flags
        }
};

// Run the cpu once all instruction are defined
void CPU::run(Vector<Instruction*>& program) {
    while (PC >= 0 && PC < program.size()) {
        Instruction* currentInstruction = program[PC];
        PC++; 
        currentInstruction->execute(*this);
        SI = stack.getTopIndex() + 1;
    }
}

// ----- INTERPRETER -----
class Runner {
    public:
        Runner(CPU& externalCpu) : cpu(externalCpu) {}

        void run() {
            std::string input;

            while (true) {
                std::cout << "> ";
                if (!std::getline(std::cin, input) || input == "exit") break;

                if (input.find("run ") == 0)
                    loadAndRunFile(input.substr(4));
                else
                    executeCommand(input);

                //cpu.display();
            }
        }

        // Convert single command into a single element vector
        void executeCommand(const std::string& cmd) {
            if (cmd.empty()) return;

            Instruction* instr = decode(cmd);
            if (instr) {
                Vector<Instruction*> program;
                program.push_back(instr);
                
                cpu.run(program);   // Delegate to CPU execution unit
                
                delete instr;       // Free allocation
            } else {
                std::cerr << "Syntax Error / Unknown instruction." << std::endl << std::endl;
            }
        }

        // Read entire file into a vecotr of instruction
        void loadAndRunFile(const std::string& filename) {
            // Reset cpu to 0 state
            // Reads file into vector, ignores empty lines
            // Updates PC from 0 to total instructions 
            // Calls executeCommand() in a loop
            
            std::cout << "Sorry bang, not implemented yet\n";
        }

    private:
        CPU& cpu;       // Refrence to CPU in the program

        Instruction* decode(const std::string& line) {
            // Logic to parse "MOV R1, 5" or "ADD R3, R1"
            // Returns a pointer to a new Instruction subclass
            // Example: if (line == "MOV") return new MOVInstruction(...);

            return nullptr; // No command found
        }
};

// ----- MAIN -----
int main() {
    FlagRegister flagsRegister;
    CPU virtualCpu(&flagsRegister);
    
    // Aggregate the CPU inside the runner framework
    Runner interpreter(virtualCpu); 
    
    std::cout << "Virtual ASM Interpreter ('exit' to quit)\n";
    std::cout << "-------------------------------------------\n";
    interpreter.run();
    
    return 0;
}