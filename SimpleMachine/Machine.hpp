#ifndef _MACHINE_
#define _MACHINE_

#include <iostream>
#include "InstructionOpcodeMap.hpp"
#include "Program.hpp"

struct Machine{
	unsigned char regs[16];
	unsigned char pc;
	unsigned char memory[256];

	Machine(){
		memset(memory, 0xff, 256);
	}

	void LoadProgram(int startingAddress, Program& program){
		unsigned char* code = program.GetHexcodePointer();
		int codeLength = program.GetCurrentMarker();
		memcpy(memory + startingAddress, code, codeLength);
		pc = startingAddress;
	}

	void Execute() {
		do{
			unsigned char opcode = memory[pc];
			if (opcode == HLT_CODE){
				break;
			}
			unsigned char op1 = memory[pc+1];
			unsigned char op2 = memory[pc+2];
			switch (opcode){
			case ADD_CODE:
				regs[0] = regs[op1] + regs[op2];
				pc += 3;
				break;
			case SUB_CODE:
				regs[0] = regs[op1] - regs[op2];
				pc += 3;
				break;
			case JNZ_CODE:
				if (regs[op1] != 0) {
					pc = op2;
				}
				else{
					pc += 3;
				}
				break;
			case JZ_CODE:
				if (regs[op1] == 0) {
					pc = op2;
				}
				else{
					pc += 3;
				}
				break;
			case MOV_CODE:
				regs[op1] = regs[op2];
				pc += 3;
				break;
			case LOAD_CODE:
				regs[op1] = memory[op2];
				pc += 3;
				break;
			case LDR_CODE:
				regs[op1] = memory[regs[op2]];
				pc += 3;
				break;
			case STORE_CODE:
				memory[op2] = regs[op1];
				pc += 3;
				break;
			case STR_CODE:
				memory[regs[op2]] = regs[op1];
				pc += 3;
				break;
			}
			
		} while (true);
	}

	void ShowMemory(){
		for (int i = 0; i < 256; i++){
			if (i%3 == 0){
				std::cout << std::endl;
			}
			std::cout << (int)memory[i] << " ";
		}
		std::cout << std::endl;
	}
};

#endif