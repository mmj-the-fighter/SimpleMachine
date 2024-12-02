#ifndef _MACHINE_
#define _MACHINE_

#include <iostream>
#include "InstructionOpcodeMap.hpp"
#include "Program.hpp"
#include "CommonDefs.h"

struct Machine{
	unsigned char regs[MAXREGS];
	unsigned char pc;
	unsigned char memory[MAXMEMBYTES];
	Program* program;
	bool zeroFlag;
	

	Machine(){
		memset(memory, 0, MAXMEMBYTES);
		zeroFlag = false;
		program = NULL;
	}

	void LoadProgram(int startingAddress, Program* aProgram){
		program = aProgram;
		unsigned char* code = program->GetByteCodePointer();
		int codeLength = program->GetCurrentMarker();
		memcpy(memory + startingAddress, code, codeLength);
		pc = startingAddress;
	}

	void Execute() {
		if (NULL == program) {
			return;
		}
		

		do{
			unsigned char opcode = memory[pc];
			int op1 = 0;
			int op2 = 0;
			int op3 = 0;
			if (opcode == HLT_CODE){
				break;
			}
			int instrLen = program->GetInstructionLength(opcode);
			if (instrLen == 0){
				std::cout << "Unexpected program failure!\n";
				break;
			}
			switch (instrLen){
			case 2:
				op1 = memory[pc + 1];
				break;
			case 3:
				op1 = memory[pc + 1];
				op2 = memory[pc + 2];
				break;
			case 4:
				op1 = memory[pc + 1];
				op2 = memory[pc + 2];
				op3 = memory[pc + 3];
				break;
			}
				
			switch (opcode){
			case ADD_CODE:
				regs[0] = regs[op1] + regs[op2];
				if (regs[0] == 0){
					zeroFlag = true;
				}
				else{
					zeroFlag = false;
				}
				pc += instrLen;
				break;
			case SUB_CODE:
				regs[0] = regs[op1] - regs[op2];
				if (regs[0] == 0){
					zeroFlag = true;
				}
				else{
					zeroFlag = false;
				}
				pc += instrLen;
				break;
			case JNZ_CODE:
				if (!zeroFlag) {
					pc = op1;
				}
				else{
					pc += instrLen;
				}
				break;
			case JZ_CODE:
				if (zeroFlag) {
					pc = op1;
				}
				else{
					pc += instrLen;
				}
				break;
			case MOV_CODE:
				regs[op1] = regs[op2];
				pc += instrLen;
				break;
			case LOAD_CODE:
				regs[op1] = memory[op2];
				pc += instrLen;
				break;
			case LDR_CODE:
				regs[op1] = memory[regs[op2]];
				pc += instrLen;
				break;
			case STORE_CODE:
				memory[op2] = regs[op1];
				pc += instrLen;
				break;
			case STR_CODE:
				memory[regs[op2]] = regs[op1];
				pc += instrLen;
				break;
			case INC_CODE:
				regs[op1] += 1;
				if (regs[op1] == 0){
					zeroFlag = true;
				}
				else{
					zeroFlag = false;
				}
				pc += instrLen;
				break;
			case DCR_CODE:
				regs[op1] -= 1;
				if (regs[op1] == 0){
					zeroFlag = true;
				}
				else{
					zeroFlag = false;
				}
				pc += instrLen;
				break;
			case MVI_CODE:
				regs[op1] = op2;
				pc += instrLen;
				break;
			case ADD3_CODE:
				regs[op1] = regs[op2] + regs[op3];
				if (regs[op1] == 0){
					zeroFlag = true;
				}
				else{
					zeroFlag = false;
				}
				pc += instrLen;
				break;
			case SUB3_CODE:
				regs[op1] = regs[op2] - regs[op3];
				if (regs[op1] == 0){
					zeroFlag = true;
				}
				else{
					zeroFlag = false;
				}
				pc += instrLen;
				break;
			}
			
		} while (true);
	}

	void ShowMemory(){
		for (int i = 0; i < MAXMEMBYTES; i++){
			if (i%3 == 0){
				std::cout << std::endl;
			}
			std::cout << (int)memory[i] << " ";
		}
		std::cout << std::endl;
	}
};

#endif