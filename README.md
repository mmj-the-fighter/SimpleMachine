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

## Supported instructions with usage, action and status

| Mnemonic | Format |Description | Example | Status |
|:----------|:-------------|:-------------|:---------|:---------| 
| **HLT** | `HLT` | Stop execution | `HLT` |[Done]
| **ADD1** | `ADD1 reg` | [A] = [A] + reg  | `ADD1 B` |[Done]
| **SUB1** | `SUB1 reg` | [A] = [A] - reg  | `SUB1 B`|[Done]
| **ADD2** | `ADD2 reg1 reg2` | [A] = reg1  + reg2  | `ADD2 C D` |[Done]
| **SUB2** | `SUB2 reg1 reg2` | [A] =  reg1  - reg2  | `SUB2 C D` |[Done]
| **ADD3** | `ADD3 reg1 reg2 reg3` | reg1 =  reg2  + reg3  | `ADD3 C D E` |[Done]
| **SUB3** | `SUB3 reg1 reg2 reg3` | reg1 =  reg2  - reg3  | `SUB3 C D E` |[Done]
| **JZ** | `JZ label` | set pc = label_address if zero flag **is** set | `JZ LOOP` |[Done]
| **JNZ** | `JNZ label` | set pc = label_address if zero flag **not** set | `JNZ LOOP` |[Done]
| **CMP** | `CMP reg1 reg2` | temp = reg1 - reg2; affect_condition_codes(temp)  | `CMP A B` |[Done]
| **JEQ** | `JEQ label` | set pc = label_address if condition code is  **is** `==` | `JEQ LOOP` |[Done]
| **JNEQ** | `JNEQ label` | set pc = label_address if condition code **is not** `==` | `JNEQ LOOP` |[Done]
| **JLT** | `JLT label` | set pc = label_address if condition code **is** `<` | `JLT LOOP` |[Done]
| **JGT** | `JGT label` | set pc = label_address if condition code **is** `>` | `JGT LOOP` |[Done]
| **MOV** | `MOV reg1 reg2`| reg1 = reg2 | `MOV A B` |[Done]
| **LOAD** | `LOAD reg address` | reg = memory[address] | `LOAD A 100` |[Done]
| **LDR** | `LDR reg1 reg2` | reg1 = memory[reg2] | `LDR A B` |[Done]
| **STORE** | `STORE reg address` | memory[address]  = reg | `STORE A 100` |[Done]
| **STR** | `STR reg1 reg2` | memory[reg2]  = reg1 | `LDR A B` |[Done]
| **INC** | `INC reg` | reg = reg + 1 | `INC B` |[Done]
| **DCR** | `DCR reg` | reg = reg - 1 | `DCR B` |[Done]
| **MVI** | `MVI reg value` | reg = value | `MVI A 20` |[Done]
| **DISP** | `DISP reg` | Print the reg | `DISP A` |[Done]
| **PUSH** | `PUSH reg` | Push reg on stack | `PUSH A` |[Done]
| **POP** | `POP reg` | POP top of the stack to reg | `POP A` |[Done]
| **CALL** | `CALL label` | jump to subroutine indicated by label_address | `CALL FACTORIAL` |[Done]
| **RET** | `RET` | return from subroutine | `RET` |[Done]

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

## Assembler
The assembler analyzes the assembly source code and emits bytecode into a program object.  
The program object represents a continuous memory region containing both code and metadata.  

The assembler operates in two passes:  

- **First Pass:** The marker is set to zero. Labels and their corresponding addresses are recorded in a label table. Each opcode is recognized, and a marker is advanced according to the instruction length. When a label is encountered, it is stored in the table with the current marker value.  
- **Second Pass:** The marker is reset to zero. Each line is reprocessed, and opcodes and operands are resolved. The opcodes are written. If an operand is a label, its address is retrieved from the label table and written to the program object. For register operands, a register character to number mapper helps to get the right number for the register and that code is also written. Thus whether the string is Opcode, memory address, label or register it's corresponding number is found and written on the program object.

Throughout both passes, the assembler performs necessary error checks.  
A special label, `MAIN`, is reserved for indicating the program's entry point. This directive is handled during the first pass, updating the program object's `mainOffset` field. 

## Simulator (Interpreter)
The loader transfers the program object into memory, after which the interpreter begins execution from `mainOffset`. It reads opcodes and operands sequentially and performs the specified operations, advancing the instruction pointer accordingly. 

For example, the `MOV` operation corresponds to: 

registersArray[operand1] = registersArray[operand2]; 

All instructions are implemented within a large `switch-case` construct. Execution continues until the `HLT` instruction is encountered. The `CALL` and `RET` instructions manipulate the stack, which is maintained in a separate memory area. The `CALL` instruction pushes the return address to the stack and jumps to the location. The `RET` instruction pops the return address and jumps to that location. By carefully using stack the program can pass arguments to the callee and get return values from it. That can be done through memory or registers too.  

## Disassembler

The disassembler also operates in two passes.  
In the first pass, all jump instructions are analyzed, and their target addresses are stored in a table with pseudo-labels.  
During the second pass, these pseudo-labels are used when generating readable output.  
The disassembler emits mnemonics for each opcode and resolves register and memory references appropriately.  
It requires knowledge of both the program length and the starting memory address where the program is loaded.

### Note
This README has been written with the help of AI for formatting and some descriptions.
