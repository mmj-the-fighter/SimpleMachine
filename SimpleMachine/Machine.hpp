#ifndef _MACHINE_
#define _MACHINE_

#include <iostream>
#include <cstdlib>
#include "InstructionOpcodeMap.hpp"
#include "TextPrinter.hpp"
#include "Program.hpp"
#include "CommonDefs.h"

enum FailureType {
	BAD_ACCESS,
	INVALID_OPCODE,
	INVALID_REG,
	INVALID_PROGRAM,
};

struct Machine{
	unsigned char regs[MAXREGS];
	unsigned char pc;
	unsigned char memory[MAXMEMBYTES];
	Program* program;
	bool zeroFlag;
	TextPrinter textPrinter;

	Machine(){
		zeroFlag = false;
		program = NULL;
		pc = 0;
		for (int i = 0; i < MAXMEMBYTES; i++) {
			memory[i] = HLT_CODE;
		}
		for (int i = 0; i < MAXREGS; i++) {
			regs[i] = HLT_CODE;
		}
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

	bool GetBytes(int startAddress, size_t numberOfBytes, unsigned char* bytes) {
		if (startAddress < 0  || startAddress >= MAXMEMBYTES || numberOfBytes <= 0) {
			return false;
		}
		if (startAddress + numberOfBytes - 1 >= MAXMEMBYTES) {
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

	void LoadProgram(unsigned char startingAddress, Program* aProgram){
		program = aProgram;
		unsigned char* code = program->GetByteCodePointer();
		unsigned char codeLength = program->GetCurrentMarker();
		memcpy(memory + startingAddress, code, codeLength);
		pc = startingAddress;
		program->SetLoadingOffset(startingAddress);
	}

	bool InterpretProgram() {
		unsigned char loadingOffset = program->GetLoadingOffset();
		unsigned char opcode;
		bool pcIncremented = false;
		bool isOp1WithinRegLimits = true;
		bool isOp2WithinRegLimits = true;
		bool isOp3WithinRegLimits = true;
		unsigned char op1 = 0;
		unsigned char op2 = 0;
		unsigned char op3 = 0;
		unsigned char temp;
		textPrinter.Clear();

		FailureType failureType;
		if (NULL == program) {
			failureType = INVALID_PROGRAM;
			goto errexit;
		}

		//uncomment these to test edge cases
		//pc = 0xFF;
		//loadingOffset = 0xFF;

		do{
			opcode = memory[pc];
			if (opcode == HLT_CODE){
				textPrinter.PrintAndFlush();
				return true;
			}

			//uncomment this to test edge case
			//opcode = 0xff;

			unsigned char instrLen = InstructionOpcodeMap::GetInstance()
				.GetInstructionLengthForOpcode(opcode);
			if (instrLen == 0){
				failureType = INVALID_OPCODE;
				goto errexit;
			}
			switch (instrLen){
			case 2:
				if (pc + 1 < MAXMEMBYTES) {
					op1 = memory[pc + 1];
					isOp1WithinRegLimits = op1 < MAXREGS;
				}
				else {
					failureType = BAD_ACCESS;
					goto errexit;
				}
				break;
			case 3:
				if (pc + 2 < MAXMEMBYTES) {
					op1 = memory[pc + 1];
					op2 = memory[pc + 2];
					isOp1WithinRegLimits = op1 < MAXREGS;
					isOp2WithinRegLimits = op2 < MAXREGS;
				}
				else {
					failureType = BAD_ACCESS;
					goto errexit;
				}
				break;
			case 4:
				if (pc + 3 < MAXMEMBYTES) {
					op1 = memory[pc + 1];
					op2 = memory[pc + 2];
					op3 = memory[pc + 3];
					isOp1WithinRegLimits = op1 < MAXREGS;
					isOp2WithinRegLimits = op2 < MAXREGS;
					isOp3WithinRegLimits = op3 < MAXREGS;
				}
				else {
					failureType = BAD_ACCESS;
					goto errexit;
				}
				break;
			}
			unsigned char acc = regs[0];
			pcIncremented = false;
			switch (opcode){
			case ADD1_CODE:
				if (isOp1WithinRegLimits) {
					acc += regs[op1];
					if (acc == 0) {
						zeroFlag = true;
					}
					else {
						zeroFlag = false;
					}
					regs[0] = acc;
				}
				else {
					failureType = INVALID_REG;
					goto errexit;
				}
				break;
			case SUB1_CODE:
				if (isOp1WithinRegLimits) {
					acc -= regs[op1];
					if (acc == 0) {
						zeroFlag = true;
					}
					else {
						zeroFlag = false;
					}
					regs[0] = acc;
				}
				else {
					failureType = INVALID_REG;
					goto errexit;
				}
				break;
			case ADD2_CODE:
				if (isOp1WithinRegLimits && isOp2WithinRegLimits) {
					acc = regs[op1] + regs[op2];
					if (acc == 0) {
						zeroFlag = true;
					}
					else {
						zeroFlag = false;
					}
					regs[0] = acc;
				}
				else {
					failureType = INVALID_REG;
					goto errexit;
				}
				break;
			case SUB2_CODE:
				if (isOp1WithinRegLimits && isOp2WithinRegLimits) {
					acc = regs[op1] - regs[op2];
					if (acc == 0) {
						zeroFlag = true;
					}
					else {
						zeroFlag = false;
					}
					regs[0] = acc;
				}
				else {
					failureType = INVALID_REG;
					goto errexit;
				}
				break;
			case JNZ_CODE:
				if (!zeroFlag) {
					if (pc + op1 + loadingOffset < MAXMEMBYTES) {
						pc = op1 + loadingOffset;
						pcIncremented = true;
					}
					else {
						failureType = BAD_ACCESS;
						goto errexit;
					}
				}
				break;
			case JZ_CODE:
				if (zeroFlag) {
					if (pc + op1 + loadingOffset < MAXMEMBYTES) {
						pc = op1 + loadingOffset;
						pcIncremented = true;
					}
					else {
						failureType = BAD_ACCESS;
						goto errexit;
					}
				}
				break;
			case MOV_CODE:
				if (isOp1WithinRegLimits && isOp2WithinRegLimits) {
					regs[op1] = regs[op2];
				}
				else {
					failureType = INVALID_REG;
					goto errexit;
				}
				break;
			case LOAD_CODE:
				if (op2 >= MAXMEMBYTES) {
					failureType = BAD_ACCESS;
					goto errexit;
				}
				else if (!isOp1WithinRegLimits) {
					failureType = INVALID_REG;
					goto errexit;
				}
				regs[op1] = memory[op2];
				break;
			case LDR_CODE:
				if (!isOp1WithinRegLimits || !isOp2WithinRegLimits) {
					failureType = INVALID_REG;
					goto errexit;
				}
				temp = regs[op2];
				if (temp < MAXMEMBYTES) {
					regs[op1] = memory[temp];
				}
				else {
					failureType = BAD_ACCESS;
					goto errexit;
				}
				break;
			case STORE_CODE:
				if (op2 > MAXMEMBYTES) {
					failureType = BAD_ACCESS;
					goto errexit;
				}
				else if (!isOp1WithinRegLimits) {
					failureType = INVALID_REG;
					goto errexit;
				}
				memory[op2] = regs[op1];
				break;
			case STR_CODE:
				if (!isOp1WithinRegLimits || !isOp2WithinRegLimits) {
					failureType = INVALID_REG;
					goto errexit;
				}
				temp = regs[op2];
				if (temp < MAXMEMBYTES) {
					memory[temp] = regs[op1];
				}
				else {
					failureType = BAD_ACCESS;
					goto errexit;
				}
				break;
			case INC_CODE:
				if (isOp1WithinRegLimits) {
					regs[op1] += 1;
					if (regs[op1] == 0) {
						zeroFlag = true;
					}
					else {
						zeroFlag = false;
					}
				}
				else {
					failureType = INVALID_REG;
					goto errexit;
				}
				break;
			case DCR_CODE:
				if (isOp1WithinRegLimits) {
					regs[op1] -= 1;
					if (regs[op1] == 0) {
						zeroFlag = true;
					}
					else {
						zeroFlag = false;
					}
				}
				else {
					failureType = INVALID_REG;
					goto errexit;
				}
				break;
			case MVI_CODE:
				if (isOp1WithinRegLimits) {
					regs[op1] = op2;
				}
				else {
					failureType = INVALID_REG;
					goto errexit;
				}
				break;
			case ADD3_CODE:
				if (isOp1WithinRegLimits && isOp2WithinRegLimits && isOp3WithinRegLimits) {
					regs[op1] = regs[op2] + regs[op3];
					if (regs[op1] == 0) {
						zeroFlag = true;
					}
					else {
						zeroFlag = false;
					}
				}
				else {
					failureType = INVALID_REG;
					goto errexit;
				}
				break;
			case SUB3_CODE:
				if (isOp1WithinRegLimits && isOp2WithinRegLimits && isOp3WithinRegLimits) {
					regs[op1] = regs[op2] - regs[op3];
					if (regs[op1] == 0) {
						zeroFlag = true;
					}
					else {
						zeroFlag = false;
					}
				}
				else {
					failureType = INVALID_REG;
					goto errexit;
				}
				break;
			case DISP_CODE:
				if (isOp1WithinRegLimits) {
					textPrinter.AddInt((int)regs[op1]);
					textPrinter.AddChar('\n');
				}
				else {
					failureType = INVALID_REG;
					goto errexit;
				}
				break;
			default:
				failureType = INVALID_OPCODE;
				goto errexit;
			}
			if (!pcIncremented) {
				if (pc + instrLen < MAXMEMBYTES) {
					pc += instrLen;
				}
				else {
					failureType = BAD_ACCESS;
					goto errexit;
				}
			}
		} while (true);
	errexit:
		switch (failureType) {
		case BAD_ACCESS:
			textPrinter.AddConstCString("Invalid memory address calculated: Aborting interpretation!\n");
			break;
		case INVALID_OPCODE:
			textPrinter.AddConstCString("Invalid opcode ");
			textPrinter.AddInt((int)opcode);
			textPrinter.AddConstCString(" found: Aborting interpretation!\n");
			break;
		case INVALID_REG:
			textPrinter.AddConstCString("Invalid register address calculated: Aborting interpretation!\n");
			break;
		case INVALID_PROGRAM:
			textPrinter.AddConstCString("Null or Invalid program: Aborting interpretation!\n");
			break;
		default:
			textPrinter.AddConstCString("Unexpectd error: Aborting interpretation!\n");
			break;
		}

		WriteRegistersOnPrinterBuffer();
		WriteMemoryOnPrinterBuffer();
		Print();
		return false;
	}

	void Print() {
		textPrinter.PrintAndFlush();
	}

	void WriteMemoryOnPrinterBuffer(){
		textPrinter.AddConstCString("Memory\n");
		for (int i = 0; i < 4; i++) {
			textPrinter.AddInt((int)memory[i]);
			textPrinter.AddChar('\t');
		}
		for (int i = 4; i < MAXMEMBYTES; i++){
			if (i%4 == 0){
				textPrinter.AddChar('\n');
			}
			textPrinter.AddInt((int)memory[i]);
			textPrinter.AddChar('\t');
		}
		textPrinter.AddChar('\n');
	}

	void WriteRegistersOnPrinterBuffer() {
		textPrinter.AddConstCString("Registers\n");
		for (int i = 0; i < MAXREGS; i++) {
			textPrinter.AddInt(i);
			textPrinter.AddChar(' ');
			textPrinter.AddInt((int)regs[i]);
			textPrinter.AddChar('\n');
		}
		textPrinter.AddChar('\n');
	}
};

#endif