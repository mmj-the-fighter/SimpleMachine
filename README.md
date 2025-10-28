# Simple Machine

A simple fictional computer simulator built for educational purposes.  
It acts as a minimal CPU with a basic assembler, disassembler, and simulator. 
[Demo on YouTube: https://www.youtube.com/watch?v=FoIajC5jvm0](https://www.youtube.com/watch?v=FoIajC5jvm0)   

---

## System Specifications

- **Registers**: 16 general-purpose registers (`A`–`P`), each 1 byte wide  
- **Memory**: 256 bytes of addressable memory (`0x00`–`0xFF`)  
- **Data Width**: 8-bit operations  
- **Instruction Lengths**: Variable (1–4 bytes)

---

## Supported Instruction Formats

| Format | Description |
|:-------|:-------------|
| `[opcode]` | No operands |
| `[opcode][operand1]` | Single operand instruction |
| `[opcode][operand1][operand2]` | Two-operand instruction |
| `[opcode][operand1][operand2][operand3]` | Three-operand instruction |

---

## Supported Instructions

| Mnemonic | Description | Example |
|:----------|:-------------|:---------|
| **HLT** | Stop execution | `HLT` |
| **ADD** | Add contents of two registers and store in register `A` | `ADD B C` |
| **SUB** | Subtract contents of operand2 from operand1, store in `A` | `SUB B C` |
| **ADD3** | Add two registers and store result in operand1 | `ADD3 A B C` |
| **SUB3** | Subtract operand3 from operand2, store in operand1 | `SUB3 A B C` |
| **JNZ** | Jump if zero flag **not** set | `JNZ LOOP` |
| **JZ** | Jump if zero flag **is** set | `JZ LOOP` |
| **MOV** | Copy value from operand2 to operand1 | `MOV A B` |
| **LOAD** | Load value from memory address into register | `LOAD A 100` |
| **LDR** | Load from memory address contained in register(operand2) | `LDR A B` |
| **STORE** | Store register value to a specific memory address | `STORE A 200` |
| **STR** | Store register value to memory address in another register(operand2) | `STR A B` |
| **INC** | Increment a register | `INC B` |
| **DCR** | Decrement a register | `DCR C` |
| **MVI** | Move immediate value into register | `MVI A 20` |
| **DISP** | Display the numeric value of a register | `DISP A` |

---

## Project Overview

The project provides:
- **Assembler**: Converts assembly-like source into binary machine code  
- **Disassembler**: Converts machine code back to assembly form  
- **Simulator**: Executes instructions on a virtual machine model

---

## Example Program

```asm
MVI A 10
MVI B 5
ADD3 C A B
DISP C
HLT
```


