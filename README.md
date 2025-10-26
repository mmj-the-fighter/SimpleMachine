# Simple Machine
Simple machine simulator built for learning purpose.  
It is a computer with the following specifications: 

- **Registers**: 16 registers, each 8 bits wide
- **Memory**: Total of 256 bytes

## Supported Instructions

The Simple Machine supports the following 16 instructions:

- **`hlt`**: Stop execution
- **`add`**: Add the contents of two registers and store the result in the 0th register
- **`sub`**: Subtract the contents of two registers and store the result in the 0th register
- **`add3`**: Add the contents of two registers and store the result in the specified register
- **`sub3`**: Subtract the contents of two registers and store the result in the specified register
- **`jnz`**: Jump to a different memory location if the last operation did not set the zero flag
- **`jz`**: Jump to a different memory location if the last operation set the zero flag
- **`mov`**: Copy a value from a source register to a destination register
- **`load`**: Load a value from a memory location into a destination register
- **`ldr`**: Load a value from a memory location specified by a register into a destination register
- **`store`**: Store a value from a register to a destination memory location
- **`str`**: Store a value from a register to a memory location specified by another register
- **`inc`**: Increment a register by one
- **`dcr`**: Decrement a register by one
- **`mvi`**: Store a byte literal in a register
- **`disp`**: Print number from the provided register

## Project Overview

This project serves as an assembler and simulator for the Simple Machine.

