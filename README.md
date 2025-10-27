# Simple Machine

Simple machine simulator built for learning purpose.  
[Demo on YouTube](https://www.youtube.com/watch?v=FoIajC5jvm0)  
It is a fictional computer with the following specifications: 
- **Registers**: 16 registers, (a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p), each 1 byte wide
- **Memory**: Sequential array of 256 unsigned char bytes

## Supported Instruction Formats

- [opcode]
- [opcode][operand1]
- [opcode][operand1][operand2]
- [opcode][operand1][operand2][operand3]

## Supported Instructions

The Simple Machine supports the following 16 instructions:

- **`hlt`**: Stop execution. e.g. `HLT`
- **`add`**: Add the contents of two registers and store the result in register `a`. e.g. `ADD B C`
- **`sub`**: Subtract the content of operand2 register from the content of operand1 register and store the result in register 'a'. e.g. `SUB B C`
- **`add3`**: Add the contents of two registers and store the result in the specified register at operand1. eg. `ADD3 A B C`
- **`sub3`**: Subtract the content of operand3 register from the content of operand2 register and store the result in the specified register at operand1. `SUB3 A B C`
- **`jnz`**: Jump to a different memory location specified at operand1 if the last operation did not set the zero flag. `JNZ LOOP`
- **`jz`**: Jump to a different memory location specified at operand2 if the last operation set the zero flag. `JZ LOOP`
- **`mov`**: Copy a value from a source register(operand2) to a destination register(operand1). `MOV A B`
- **`load`**: Load a value from a memory location(operand2) into a destination register(operand1). `LOAD A 100`
- **`ldr`**: Load a value from a memory location specified by a register(operand2) into a destination register(operand1). `LDR A B`
- **`store`**: Store a value from a register(operand1) to a destination memory location(operand2). `STORE A 200`
- **`str`**: Store a value from a register(operand1) to a memory location specified by another register(operand1). `STR A B`
- **`inc`**: Increment a register by one. `INC B`
- **`dcr`**: Decrement a register by one. `DCR C`
- **`mvi`**: Store a byte literal in a register. `MVI A 20`
- **`disp`**: Print number from the provided register. `DISP A`

## Project Overview

This project serves as an assembler, disassembler and simulator for the Simple Machine.

