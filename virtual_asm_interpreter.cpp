#include <iostream>
#include <iomanip>
#include <string>
#include <limits>
#include <fstream>

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

        signed char getByte() const { return value; }
        int getValue() const { return static_cast<int>(value); }

        void setByte(signed char newValue) { value = newValue; }
        void setValue(int newValue) { value = static_cast<signed char>(newValue); }

        // Get register value in 8 bit binary
        void getBinary(Queue<int> bits) const {
            unsigned char val = static_cast<unsigned char>(value);
            for (int i = 7; i >= 0; i--) {
                bits.enqueue((val >> i) & 1); 
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
        int getOF() const { return static_cast<int>(OF); }
        int getUF() const { return static_cast<int>(UF); }
        int getCF() const { return static_cast<int>(CF); }
        int getZF() const { return static_cast<int>(ZF); }

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

        // Get the address byte
        signed char getByte(int address) const {
            if (address < 0 || address >= 64) throw IndexOutOfBoundsException();
            return data[address];
        }

        // Get the address value
        int getValue(int address) const {
            if (address < 0 || address >= 64) throw IndexOutOfBoundsException();
            return static_cast<int>(data[address]);
        }

        // Sets signed char at address when it is valid
        void setByte(int address, signed char value) {
            if (address < 0 || address >= 64) throw IndexOutOfBoundsException();
            data.replace(address, value);
        }

        // Sets int at address when it is valid
        void setValue(int address, int value) {
            if (address < 0 || address >= 64) throw IndexOutOfBoundsException();
            data.replace(address, static_cast<signed char>(value));
        }

        // Reset all memory slot to 0
        void resetMemory() {
            for (int i = 0; i < 64; i++) {
                data[i] = 0;
            }
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

        // Format the data to 4 digit
        void formatDisplay(int value) {
            if (value < 0)
                std::cout << "#-" << std::setw(3) << std::setfill('0') << -value;
            else
                std::cout << "#" << std::setw(4) << std::setfill('0') << value;
        }
         
    public:
        CPU(FlagRegister* externalFlags) : flags(externalFlags), PC(0), SI(0),
            registers{GeneralRegister(0), GeneralRegister(1), GeneralRegister(2), GeneralRegister(3),
                      GeneralRegister(4), GeneralRegister(5), GeneralRegister(6), GeneralRegister(7)} {}

        GeneralRegister& getRegister(int id) {
            if (id < 0 || id > 7) throw CPU::RegisterOverflowException();
            return registers[id];
        }

        void reset() {
            for (GeneralRegister& reg : registers) {
                reg.setValue(0);
            }

            while (!stack.isEmpty()) {
                signed char temp = stack.pop();
            }

            flags->resetFlags();
            memory.resetMemory();

            PC = 0;
            SI = stack.getTopIndex() + 1;
        }

        // Display everything data store into the cpu
        void display() {
            std::cout << "#Begin#\n";

            // General registers
            std::cout << "#Registers";
            for (const GeneralRegister reg : registers) {
                formatDisplay(static_cast<int>(reg.getValue()));
            }
            std::cout << "#\n";

            // Flags
            std::cout << "#Flags#" << flags->getCF() << "#" << flags->getZF() << "#" << flags->getUF() << "#" << flags->getOF() << "#\n";

            // Program Counter
            std::cout << "#PC";
            formatDisplay(static_cast<int>(PC));
            std::cout << "#\n";

            // Memory
            std::cout << "#Memory#\n";
            for (int i = 0; i < 8; i++) {
                for (int j = 0; j < 8; j++) {
                    formatDisplay(memory.getValue(i * 8 + j));
                }
                std::cout << "#\n";
            }

            std::cout << "#End#\n\n";
        }

        // Get the top most value in the stack and pop it
        signed char getStackByte() {
            try {
                return stack.pop(); 
            } 
            catch (const Stack<signed char, 8>::UnderflowException&) {
                std::cout << "ERROR: Stack is empty.\n\n";
                throw;
            }
        }

        int getStackValue() {
            try {
                return static_cast<int>(stack.pop()); 
            } 
            catch (const Stack<signed char, 8>::UnderflowException&) {
                std::cout << "ERROR: Stack is empty.\n\n";
                throw;
            }
        }

        // Push value into cpu's stack
        void pushStackByte(signed char value) {
            try {
                stack.push(value); 
            } 
            catch (const Stack<signed char, 8>::OverflowException&) {
                std::cout << "ERROR: Stack Overflow.\n\n";
                throw;
            }
        }

        void pushStackValue(int value) {
            pushStackByte(static_cast<signed char>(value));
        }

        // Get and set the value of the Program Counter.
        int getPC() const { return PC; }
        void setPC(int newPC) { PC = newPC; }

        // Get the value of the stack index.
        int getSI() const { return SI; }

        // Get refrence to flags
        FlagRegister& getFlags() { return *flags; }

        void run(Vector<Instruction*>& program);

        // Exception classes
        class RegisterOverflowException {}; 
};

// ----- INSTRUCTIONS -----
// Base Instruction Class
class Instruction {
    public:
        // Will be implemented by all children instructions
        virtual void execute(CPU& cpu) = 0;

        // Clean up all children instructions
        virtual ~Instruction() {}

        // Exception classes
        class InvalidSyntaxException {}; 
};

// ----- ARITHMETIC -----
// Arithmetic Intermediate Class (ADD, SUB, MUL, DIV)
class ArithmeticInstruction : public Instruction {
    protected:
        GeneralRegister* dest;
        GeneralRegister* src;

        // When source register slot in command is not a register (ADD R2, R1) but a literal value (67).
        // E.g. (ADD R2, 67), immediateValue = 67, useImmediate = true;
        int immediateValue;
        bool useImmediate;

        FlagRegister& flags;

        void updateFlags(int result, unsigned int unsignedResult) {
            flags.setOF(result > 127);
            flags.setUF(result < -128);
            flags.setCF(unsignedResult > 255);
            flags.setZF(static_cast<signed char>(result) == 0);
        }

        size_t findChar(CPU& cpu, const std::string& word, char charToFind) {
            size_t character = word.find(charToFind);
            if (character == std::string::npos) throw Instruction::InvalidSyntaxException();

            return character;
        }

        GeneralRegister* findRegister(CPU& cpu, std::string str, size_t pos) {
            int id = std::stoi(str.substr(pos + 1));
            if (id < 0 || id >= 8) throw CPU::RegisterOverflowException();

            return &cpu.getRegister(id);
        }

    public:
        ArithmeticInstruction(CPU& cpu, const std::string& args) : flags(cpu.getFlags()), dest(nullptr), src(nullptr), immediateValue(0), useImmediate(false) {        
            try {
                // Find "," from command
                size_t commaPos = findChar(cpu, args, ',');

                // Grab destination register and source register
                std::string destStr = args.substr(0, commaPos);
                std::string srcStr = args.substr(commaPos + 1);

                // Find "R" from destination register
                size_t destR = findChar(cpu, destStr, 'R');

                dest = findRegister(cpu, destStr,destR);

                // Find "R" from source register, if entered literal value, useImmediate = true
                size_t srcR = srcStr.find('R');

                if (srcR != std::string::npos) {
                    src = findRegister(cpu, srcStr,srcR);
                } else {
                    immediateValue = std::stoi(srcStr);
                    useImmediate = true;
                }
            } catch (...) {
                throw;
            }
        }
};

// ADD <Source Register>, <Value / Register>
class AddInstruction : public ArithmeticInstruction {
    public:
        AddInstruction(CPU& cpu, const std::string& args) : ArithmeticInstruction(cpu, args) {}

        void execute(CPU& cpu) override {
            if (!dest) return;

            int destVal = dest->getValue();
            int srcVal;

            if (useImmediate) srcVal = immediateValue;
            else srcVal = src->getValue();

            // Actually perform ADD here
            int result = destVal + srcVal;

            // Change to unsigned for Carry Flag
            unsigned int unsignedResult = static_cast<unsigned char>(dest->getByte()) + static_cast<unsigned char>(srcVal);
            updateFlags(result, unsignedResult);
            dest->setValue(result);
        }
};

// SUB <Source Register>, <Value / Register>
class SubInstruction : public ArithmeticInstruction {
    public:
        SubInstruction(CPU& cpu, const std::string& args) : ArithmeticInstruction(cpu, args) {}

        void execute(CPU& cpu) override {
            if (!dest) return;

            int destVal = dest->getValue();
            int srcVal;

            if (useImmediate) srcVal = immediateValue;
            else srcVal = src->getValue();

            // Actually perform SUB here
            int result = destVal - srcVal;

            // Change to unsigned for Carry Flag
            unsigned int unsignedResult = static_cast<unsigned int>(static_cast<unsigned char>(destVal)) - static_cast<unsigned int>(static_cast<unsigned char>(srcVal));
            updateFlags(result, unsignedResult);
            dest->setValue(result);
        }
};

// MUL <Source Register>, <Value / Register>
class MulInstruction : public ArithmeticInstruction {
    public:
        MulInstruction(CPU& cpu, const std::string& args) : ArithmeticInstruction(cpu, args) {}

        void execute(CPU& cpu) override {
            if (!dest) return;

            int destVal = dest->getValue();
            int srcVal;

            if (useImmediate) srcVal = immediateValue;
            else srcVal = src->getValue();

            // Actually perform MUL here
            int result = destVal * srcVal;

            // Change to unsigned for Carry Flag
            unsigned int unsignedResult = static_cast<unsigned int>(static_cast<unsigned char>(destVal)) * static_cast<unsigned int>(static_cast<unsigned char>(srcVal));
            updateFlags(result, unsignedResult);
            dest->setValue(result);
        }
};

// DIV <Source Register>, <Value / Register>
class DivInstruction : public ArithmeticInstruction {
    public:
        DivInstruction(CPU& cpu, const std::string& args) : ArithmeticInstruction(cpu, args) {}

        void execute(CPU& cpu) override {
            if (!dest) return;

            int destVal = dest->getValue();
            int srcVal;

            if (useImmediate) srcVal = immediateValue;
            else srcVal = src->getValue();

            // Checks for Division By 0 
            if (srcVal == 0) {
                std::cout << "ERROR: Division by zero\n\n";
                throw DivisionByZeroException();
            }

            // Actually perform DIV here
            int result = destVal / srcVal;

            // Change to unsigned for Carry Flag
            unsigned int unsignedResult = static_cast<unsigned int>(static_cast<unsigned char>(destVal)) / static_cast<unsigned int>(static_cast<unsigned char>(srcVal));
            updateFlags(result, unsignedResult);
            dest->setValue(result);
        }

        // Exception class for zero division
        class DivisionByZeroException {};
};

// INPUT <Register>
class InputInstruction : public Instruction {
    private:
        GeneralRegister* reg;
        FlagRegister& flags;

        void checkInput(int& value) {
            // If user enters text or an invalid value
            if (!(std::cin >> value)) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Discard bad input
                value = 0;
            } else {
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Clear the leftover '\n'
            }
        }

    public:
        InputInstruction(CPU& cpu, const std::string& regStr) : flags(cpu.getFlags()), reg(nullptr) {
            if (regStr.length() < 2 || regStr[0] != 'R') throw Instruction::InvalidSyntaxException();

            try {
                size_t processedChars = 0;
                int regId = std::stoi(regStr.substr(1), &processedChars); // Get how many characters parsed from processedChars

                if ((processedChars + 1) < regStr.length()) { // Check if processedChars is longer than 2 char
                    std::string remainder = regStr.substr(processedChars + 1);
                    if (remainder.find_first_not_of(" \t\r\n") != std::string::npos) throw Instruction::InvalidSyntaxException(); 
                }
                
                if (regId >= 0 && regId < 8) reg = &cpu.getRegister(regId);
                else throw CPU::RegisterOverflowException();
            } catch (...) {
                throw;
            }
        }

        void execute(CPU& cpu) override { // determine the value is it in the range of -128 to 127
            if (!reg) { return; }
            int value;
            
            std::cout << "\n? " << std::flush; 
            checkInput(value);

            flags.setOF(false);
            flags.setUF(false);
            flags.setZF(false);   

            if (value > 127) {
                reg->setValue(127);
                flags.setOF(true);
            } else if (value < -128) {
                reg->setValue(-128);
                flags.setUF(true);
            } else {
                reg->setValue(value);
                if (value == 0) flags.setZF(true);
            }
        }
};

// DISPLAY <Register>
class DisplayInstruction : public Instruction {
    private:
        GeneralRegister* reg;

    public:
        DisplayInstruction(CPU& cpu, const std::string& regStr)
            : reg(nullptr) {

            if (regStr.length() < 2 || regStr[0] != 'R') throw Instruction::InvalidSyntaxException();

            try {
                int regId = std::stoi(regStr.substr(1));

                if (regId >= 0 && regId < 8) reg = &cpu.getRegister(regId);
                else throw CPU::RegisterOverflowException();
            } catch (...) {
                throw;
            }
        }

        void execute(CPU& cpu) override {
            if (!reg) return;

            std::cout << reg->getValue() << std::endl;
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
            bool shouldDisplay = false;
            std::string input;

            while (true) {
                std::cout << "> ";
                if (!std::getline(std::cin, input) || input == "exit") break;

                std::string command = input;
                size_t first = command.find_first_not_of(" \t\r\n");
                if (first != std::string::npos) {
                    command = command.substr(first);
                }

                if (command.find("run ") == 0)
                    shouldDisplay = loadAndRunFile(command.substr(4));
                else if (command.find("status") == 0)
                    cpu.display();
                else if (command.find("reset") == 0) {
                    cpu.reset();
                    shouldDisplay = true;
                }
                else
                    shouldDisplay = executeCommand(input);

                if (shouldDisplay) cpu.display();
            }
        }

        // Convert single command into a single element vector
        bool executeCommand(const std::string& cmd) {
            if (cmd.empty()) return false;

            Instruction* instr = nullptr;

            try {
                instr = decode(cmd);

                if (instr) {
                    Vector<Instruction*> program;
                    program.push_back(instr);

                    cpu.setPC(0);
                    cpu.run(program);   
                    delete instr;           // Free allocation
                    return true;
                }
                else {
                    std::cout << "ERROR: Syntax Error / Unknown instruction.\n\n";
                    return false;
                }
            }
            catch (...) {
                if (instr) delete instr;    // Free allocation
                return false;
            }
        }

        // Read entire file into a vecotr of instruction
        bool loadAndRunFile(const std::string& filename) {
            std::ifstream file(filename);
            if (!file.is_open()) {
                std::cout << "ERROR: Could not open file '" << filename << "'\n\n";
                return false;
            }

            Vector<Instruction*> program;
            std::string line;
            int lineNumber = 0;

            try {
                while (std::getline(file, line)) {
                    lineNumber++;

                    size_t start = line.find_first_not_of(" \t\r\n");
                    if (start == std::string::npos) continue; // Skip empty/blank lines
                    size_t end = line.find_last_not_of(" \t\r\n");
                    std::string cleanedLine = line.substr(start, end - start + 1);

                    try {
                        // Decode the cleaned line
                        Instruction* instr = decode(cleanedLine);
                        if (!instr) throw Runner::UnknownSyntaxException();

                        program.push_back(instr);
                    } 
                    catch (const CPU::RegisterOverflowException&) {
                        std::cout << "ERROR: Invalid register at line " << lineNumber << "\n\n";
                        throw;
                    }
                    catch (...) {
                        std::cout << "ERROR: Syntax error at line " << lineNumber << "\n\n";
                        throw; 
                    }
                }

                if (program.isEmpty()) {
                    std::cout << "WARNING: File is empty.\n\n";
                    return false;
                }

                // Reset CPU and run the intructions
                cpu.reset();
                cpu.run(program);

                for (int i = 0; i < program.size(); ++i) {
                    delete program[i];
                }
                return true;
            }
            catch (...) {
                for (int i = 0; i < program.size(); ++i) {
                    delete program[i];
                }
                return false;
            }
        }

        class UnknownSyntaxException {}; 

    private:
        CPU& cpu;       // Refrence to CPU in the program

        Instruction* decode(const std::string& line) {
            if (line.rfind("INPUT ", 0) == 0) return new InputInstruction(cpu, line.substr(6));
            if (line.rfind("ADD ", 0) == 0) return new AddInstruction(cpu, line.substr(4));
            if (line.rfind("SUB ", 0) == 0) return new SubInstruction(cpu, line.substr(4));
            if (line.rfind("MUL ", 0) == 0) return new MulInstruction(cpu, line.substr(4));
            if (line.rfind("DIV ", 0) == 0) return new DivInstruction(cpu, line.substr(4));
            if (line.rfind("DISPLAY ", 0) == 0) return new DisplayInstruction(cpu, line.substr(8));
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