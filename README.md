<div align="center">
  
<img width="360" alt="logo" src="https://github.com/user-attachments/assets/e957013c-785d-43d1-8f38-de09966fb1f3" />
<h3>Virtual ASM Interpreter</h3>

</div>

Welcome to the **VASM** utility suite. This repository contains two versions of a lightweight, custom 8-bit Virtual CPU interpreter designed to parse, decode, and execute a custom Assembly Instruction Set Architecture (ISA).

* **VASM:** An interactive REPL terminal workspace featuring a dynamic user interface blocks system.
* **VASM-Lite:** A streamlined, non-interactive script runner tailored for direct file execution via terminal arguments.

---

## Hardware Architecture Specifications

Both interpreters simulate an underlying hardware profile with the following structural limitations:

* **Registers:** 8 general-purpose registers labeled `R0` through `R7`. Each register stores a **1-byte signed integer** (valid value range: -128 to 127).
* **Memory Array:** 64 bytes of volatile random-access memory (addressable space: indices `0` to `63`).
* **Hardware Stack:** A bounded LIFO data structure capped at an **8-byte maximum capacity**.
* **Status Flags:** 4 boolean status indicators tracking algorithmic boundaries:
    * `CF` (Carry Flag): Set during unsigned arithmetic wraps exceeding 8 bits.
    * `ZF` (Zero Flag): Set when the outcome of an operation yields exactly zero.
    * `UF` (Underflow Flag): Set when signed values drop below -128.
    * `OF` (Overflow Flag): Set when signed values exceed 127.
* **Control Registers:** Internal Program Counter (`PC`) and Stack Index (`SI`) tracking program offsets.

---

## 1. VASM (Interactive REPL Mode)

The interactive tool initializes an execution environment prompt (`> `), allowing you to pass single operations inline or run external macro files while maintaining environmental persistency.

### Execution

```bash
./VASM
```

### REPL System Commands

Within the interactive prompt, you can use the following control utilities instead of raw assembly instructions:

| Command | Description |
| --- | --- |
| `status` | Explicitly prints out the current layout of Registers, Flags, PC, and Memory matrices. |
| `reset` | Wipes the machine state clean: zeros out all registers/memory, purges the stack, and clears flags. |
| `run <filename.asm>` | Batch-processes an external file sequentially, applying its changes directly onto the active state. |
| `exit` | Safely terminates the interpreter process. |
| *(Any valid VASM instruction)* | Passing a raw assembly line executes it immediately and auto-displays the updated CPU block state. |

---

## 2. VASM-Lite (Script Execution Mode)

The Lite executable provides an automated pipe mechanism designed to execute a program script source from start to finish without pausing for terminal interactions (unless an explicit `INPUT` instruction halts execution for user data entries).

### Execution

```bash
./VASM-Lite <path_to_file.asm>
```

### Operational Behavior

1. The engine reads the entirety of the specified source file into a processing queue.
2. It completely resets the internal hardware to zero states before starting step-by-step decoding.
3. Upon processing the final instruction, it prints a final snapshot block illustrating the terminal state configuration of the Virtual CPU and then exits.

---

## Supported Assembly Syntax Reference

The interpreter parses operations as uppercase directives followed by exact operand spacing parameters. Blank text lines and leading/trailing whitespace are ignored.

### Data Manipulation & Memory Operands

> ⚠️ **Syntax Note on Brackets:** `LOAD` parameters **require** enclosing brackets for both physical addresses and tracking registers (e.g., `[20]`, `[R1]`). However, immediate values for target addresses passed to `STORE` **must omit** brackets (e.g., `20`), while its register references preserve them (`[R1]`).

| Instruction Syntax | Allowed Arguments | Behavior Description |
| --- | --- | --- |
| **`MOV`** `Dest, Src` | `Dest`: Register (`R0-R7`) <br>

<br>`Src`: Register, Value, or `[Register]` | Copies the source item value into the destination register. Can parse indirect memory lookups like `MOV R1, [R2]`. |
| **`LOAD`** `Dest, [Src]` | `Dest`: Register (`R0-R7`) <br>

<br>`Src`: Absolute address integer (`0-63`) or Register | Extracts the 1-byte value from memory index location `Src` and copies it into target `Dest`. Examples: `LOAD R0, [45]` or `LOAD R0, [R1]`. |
| **`STORE`** `Src, Dest` | `Src`: Register (`R0-R7`) <br>

<br>`Dest`: Absolute address integer (`0-63`) or `[Register]` | Saves the value found inside register `Src` down into physical memory address `Dest`. Examples: `STORE R0, 45` or `STORE R0, [R1]`. |
| **`PUSH`** `Reg` | `Reg`: Register (`R0-R7`) | Appends the 1-byte state value of the given register onto the top of the stack. Throws a Stack Overflow error if size exceeds 8 bytes. |
| **`POP`** `Reg` | `Reg`: Register (`R0-R7`) | Extracts the top byte value sitting on the stack, removes it, and writes it directly into target register `Reg`. |

### Mathematical & Algorithmic Blocks

All standard math instructions modify system status flags (`OF`, `UF`, `CF`, `ZF`) relative to the outcome of their final value generation bounds checks.

| Instruction Syntax | Allowed Source Arguments | Behavior Description |
| --- | --- | --- |
| **`ADD`** `Dest, Src` | Register (`R0-R7`) or Integer value | Adds `Src` value directly into the value inside `Dest`. |
| **`SUB`** `Dest, Src` | Register (`R0-R7`) or Integer value | Subtracts `Src` value directly from the value inside `Dest`. |
| **`MUL`** `Dest, Src` | Register (`R0-R7`) or Integer value | Multiplies `Dest` register value by the factor of `Src`. |
| **`DIV`** `Dest, Src` | Register (`R0-R7`) or Integer value | Divides `Dest` value by `Src`. Crashes with a Division by Zero exception if `Src` resolves to 0. |
| **`INC`** `Reg` | Register (`R0-R7`) | Increments the targeted register's contents exactly by 1. |
| **`DEC`** `Reg` | Register (`R0-R7`) | Decrements the targeted register's contents exactly by 1. |

### Bitwise Shifting & Hardware Flags

| Instruction Syntax | Allowed Arguments | Behavior Description |
| --- | --- | --- |
| **`SHL`** `Reg, Count` | `Reg`: Register, `Count`: Positive Integer | Bitwise logical shift-left applied to the register value by `Count` spaces. Fills empty bits with 0. |
| **`SHR`** `Reg, Count` | `Reg`: Register, `Count`: Positive Integer | Bitwise logical shift-right applied to the register value by `Count` spaces. Fills empty bits with 0. |
| **`ROL`** `Reg, Count` | `Reg`: Register, `Count`: Positive Integer | Rotates bits left symmetrically by `Count` spaces. Outflowing upper bits wrap directly into lower bits. |
| **`ROR`** `Reg, Count` | `Reg`: Register, `Count`: Positive Integer | Rotates bits right symmetrically by `Count` spaces. Outflowing lower bits wrap directly into upper bits. |
| **`RESET`** `Flag` | String literal name: `CF`, `ZF`, `UF`, `OF` | Forcibly clears the targeted system status flag back down to a false/zero representation state. |

### Input / Output Contexts

| Instruction Syntax | Target Parameter | Behavior Description |
| --- | --- | --- |
| **`INPUT`** `Reg` | Register (`R0-R7`) | Freezes terminal loop and asks for an integer entry via a `? ` interface indicator. Values are clamped to signed byte thresholds; tracking overflow or zero status triggers flags accordingly. Invalid input text evaluates to 0. |
| **`DISPLAY`** `Reg` | Register (`R0-R7`) | Outputs the current raw decimal representation value held inside the target register out onto the stream standard output, closed by a line break. |

---

## Sample Program Run Example

Below is a valid example program script you can copy into a file named `math_demo.asm`. It exercises arithmetic operations, memory store pipelines, stack registers interactions, and bitwise execution:

```assembly
INPUT R0
MOV R1, 10
ADD R0, R1
STORE R0, 32
LOAD R2, [32]
SHL R2, 2
PUSH R2
POP R3
DISPLAY R3
```

### Running with VASM (Interactive):

```text
$ ./VASM
Virtual ASM Interpreter ('exit' to quit)
-------------------------------------------
> run math_demo.asm

? 5
#Begin#
#Registers#0015#0010#0060#0060#0000#0000#0000#0000#
#Flags#0#0#0#0#
#PC#0009#
#Memory#
#0000#0000#0000#0000#0000#0000#0000#0000#
#0000#0000#0000#0000#0000#0000#0000#0000#
#0000#0000#0000#0000#0000#0000#0000#0000#
#0000#0000#0000#0000#0000#0000#0000#0000#
#0015#0000#0000#0000#0000#0000#0000#0000#
#0000#0000#0000#0000#0000#0000#0000#0000#
#0000#0000#0000#0000#0000#0000#0000#0000#
#0000#0000#0000#0000#0000#0000#0000#0000#
#End#
```
