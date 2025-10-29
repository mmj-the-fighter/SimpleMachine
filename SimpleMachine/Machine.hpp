#ifndef _MACHINE_
#define _MACHINE_

#include <iostream>
#include <cstdlib>
#include "InstructionOpcodeMap.hpp"
#include "Program.hpp"
#include "CommonDefs.h"

struct Machine{
	unsigned char regs[MAXREGS];
	unsigned char pc;
	unsigned char memory[MAXMEMBYTES];
	Program* program;
	bool zeroFlag;
	InstructionOpcodeMap inOpMap;
	

	Machine(){
		memset(memory, 0, MAXMEMBYTES);
		zeroFlag = false;
		program = NULL;
	}

	inline bool IsValidAddres(int address) {
		if (address >= MAXMEMBYTES || address < 0) {
			return false;
		}
		else {
			return true;
		}
	}

	unsigned char GetByteAt(int address, bool* validAccess) {
		if (address >= MAXMEMBYTES || address < 0) {
			*validAccess = false;
			return 0;
		}
		else {
			*validAccess = true;
			return memory[address];
		}
	}

	bool GetBytes(int startAddress, int numberOfBytes, unsigned char* bytes) {
		if (startAddress < 0  || startAddress >= MAXMEMBYTES || numberOfBytes <= 0) {
			return false;
		}
		int endAddress = startAddress + numberOfBytes - 1;
		if (endAddress >= MAXMEMBYTES) {
			return false;
		}
		memcpy(bytes, &memory[startAddress], numberOfBytes);
		return true;
	}

	inline bool GetAtMost4Bytes(int startAddress, int numberOfBytes, unsigned char* bytes) {
		if (startAddress < 0 || startAddress >= MAXMEMBYTES || numberOfBytes <= 0 || numberOfBytes > 4) {
			return false;
		}
		int endAddress = startAddress + numberOfBytes - 1;
		if (endAddress >= MAXMEMBYTES) {
			return false;
		}
		switch (numberOfBytes) {
		case 1:
			bytes[0] = memory[startAddress];
			break;
		case 2:
			bytes[0] = memory[startAddress];
			bytes[1] = memory[startAddress+1];
			break;
		case 3:
			bytes[0] = memory[startAddress];
			bytes[1] = memory[startAddress + 1];
			bytes[2] = memory[startAddress + 2];
			break;
		case 4:
			bytes[0] = memory[startAddress];
			bytes[1] = memory[startAddress + 1];
			bytes[2] = memory[startAddress + 2];
			bytes[3] = memory[startAddress + 3];
			break;
		}
		return true;
	}

	void LoadProgram(int startingAddress, Program* aProgram){
		program = aProgram;
		unsigned char* code = program->GetByteCodePointer();
		int codeLength = program->GetCurrentMarker();
		memcpy(memory + startingAddress, code, codeLength);
		pc = startingAddress;
		program->SetLoadingOffset(startingAddress);
	}

	void Execute() {
		if (NULL == program) {
			return;
		}
		int loadingOffset = program->GetLoadingOffset();

		do{
			unsigned char opcode = memory[pc];
			int op1 = 0;
			int op2 = 0;
			int op3 = 0;
			if (opcode == HLT_CODE){
				break;
			}
			int instrLen = inOpMap.GetInstructionLengthForOpcode(opcode);
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
					pc = op1+loadingOffset;
				}
				else{
					pc += instrLen;
				}
				break;
			case JZ_CODE:
				if (zeroFlag) {
					pc = op1+loadingOffset;
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
			case DISP_CODE:
				std::cout << (int)regs[op1] << "\n";
				pc += instrLen;
				break;
			}
			
		} while (true);
	}

	void ShowMemory(){
		std::cout << "Memory\n";
		for (int i = 0; i < 4; i++) {
			std::cout << (int)memory[i] << "\t";
		}
		for (int i = 4; i < MAXMEMBYTES; i++){
			if (i%4 == 0){
				std::cout << '\n';
			}
			std::cout << (int)memory[i] << "\t";
		}
		std::cout << '\n';
	}

	void ShowRegisters() {
		std::cout << "Registers\n";
		for (int i = 0; i < MAXREGS; i++) {
			std::cout << "reg" << i << " : " << (int)regs[i] << '\n';
		}
		std::cout << '\n';
	}
};

#endif