#ifndef _DISASSEMBLER_
#define _DISASSEMBLER_

#include <sstream>

#include "InstructionOpcodeMap.hpp"
#include "Machine.hpp"
#include "RevSymbolTable.hpp"
#include "CommonDefs.h"
#include "Profiler.hpp"
#include "TextPrinter.hpp"
#include "RegisterHelper.hpp"

class Disassembler
{
	Machine* machine;
	int loadingOffset;
	int programLength;
	RevSymbolTable addressLabelTable;
	TextPrinter printer;
	RegisterHelper regHelper;
	std::ostringstream ss;
public:
	Disassembler(Machine* m, int offset, int length) {
		machine = m;
		loadingOffset = offset;
		programLength = length;
	}

	Disassembler() {
		machine = NULL;
		loadingOffset = 0;
		programLength = 0;
	}

	std::string GenLabel(int count) {
		ss.str("");
		ss.clear();
		ss << "Label" << count;
		return ss.str();
	}


	void Set(Machine* m, int offset, int length) {
		machine = m;
		loadingOffset = offset;
		programLength = length;
	}

	bool PassForSymbols() {
		//util::ProfilerScope prof(311);
		int address = loadingOffset;
		int lastLoc = address + programLength - 1;
		int labelCount = 0;

		bool instrFound;
		bool validAccess;
		unsigned char instrLength;

		while (address <= lastLoc) {
			unsigned char opcode = machine->GetByteAtFast(address);
			InstructionOpcodeMap::GetInstance()
				.GetOpcodeStrAndInstrLength(opcode, &instrLength, &instrFound);
			if (instrFound) {
				//std::cout << *pInstr << std::endl;
				switch (opcode) {
				case CALL_CODE:
				case JZ_CODE:
				case JNZ_CODE:
				case JLT_CODE:
				case JEQ_CODE:
				case JGT_CODE:
				case JNEQ_CODE:
					unsigned char jumpAddr = machine->GetByteAt(address + 1, &validAccess);
					if (!validAccess) {
						std::cout << "bad exe format\n";
						return false;
					}
					addressLabelTable.AddKeyValue(jumpAddr, GenLabel(labelCount));
					++labelCount;
					break;
				}
				address += instrLength;
			}
			else {
				std::cout << "bad exe format\n";
				return false;
			}
		}
		return true;
	}

	bool Translate() {
		int address = loadingOffset;
		bool validAccess;
		bool foundInstr;
		bool foundLabel;
		bool badExeFormat = false;
		bool badAccess = false;
		int badLocation = 0;
		int lastLoc = address + programLength - 1;

		//machine->memory[12] = 67;

		if (!PassForSymbols()) {
			return false;
		}

		printer.AddConstCString("Disassembly:\n");
		while (address <= lastLoc) {
			std::string instr;
			char reg;
			unsigned char operand;
			unsigned char operands[3];
			RegisterPair regPair;
			RegisterTriplet regTriplet;
			unsigned char instrLength;
			unsigned char opcode = machine->GetByteAt(address, &validAccess);
			if (!validAccess) {
				badLocation = address;
				badAccess = true;
				goto exit;
			}
			//std::string* pReg;

			std::string* pInstr = InstructionOpcodeMap::GetInstance()
				.GetOpcodeStrAndInstrLength(opcode, &instrLength, &foundInstr);
			if (foundInstr) {
				if (!machine->IsValidAddres(address - loadingOffset)) {
					badLocation = address - loadingOffset;
					badAccess = true;
					goto exit;
				}
				std::string* pLabel = addressLabelTable.Lookup(address - loadingOffset, &foundLabel);
				if (foundLabel) {
					//std::cout << *pLabel << ":\n";
					printer.AddString(*pLabel);
					printer.AddChar(':');
					printer.AddChar('\n');
				}

				switch (opcode) {
				case HLT_CODE:
					//std::cout << "HLT\n";
					printer.AddConstCString("HLT\n\n");
					break;
				case RET_CODE:
					printer.AddConstCString("RET\n\n");
					break;
				case CALL_CODE:
				case JNZ_CODE:
				case JZ_CODE:
				case JLT_CODE:
				case JEQ_CODE:
				case JGT_CODE:
				case JNEQ_CODE:
					operand = machine->GetByteAt(address + 1, &validAccess);
					if (!validAccess) {
						badAccess = true;
						badLocation = address + 1;
						goto exit;
					}
					pLabel = addressLabelTable.Lookup(operand, &foundLabel);
					if (foundLabel) {
						//std::cout << *pInstr << " " << *pLabel << "\n";
						printer.AddString(*pInstr);
						printer.AddChar(' ');
						printer.AddString(*pLabel);
						printer.AddChar('\n');
					}
					else {
						badExeFormat = true;
						badLocation = address + 1;
						goto exit;
					}
					break;
				case INC_CODE:
				case DCR_CODE:
				case DISP_CODE:
				case ADD1_CODE:
				case SUB1_CODE:
				case PUSH_CODE:
				case POP_CODE:
					operand = machine->GetByteAt(address + 1, &validAccess);
					if (!validAccess) {
						badAccess = true;
						badLocation = address + 1;
						goto exit;
					}
					//pReg = revRegisterTable.Lookup(operand1, &foundRegister);
					reg = regHelper.FindRegisterChar(operand);
					if (reg != '\0') {
						//std::cout << *pInstr << " " << *pReg << "\n";
						printer.AddString(*pInstr);
						printer.AddChar(' ');
						printer.AddChar(reg);
						printer.AddChar('\n');
					}
					else {
						badExeFormat = true;
						badLocation = address + 1;
						goto exit;
					}
					break;
				case ADD2_CODE:
				case SUB2_CODE:
				case MOV_CODE:
				case LDR_CODE:
				case STR_CODE:
				case CMP_CODE:
					validAccess = machine->GetAtMost4Bytes(address + 1, 2, &operands[0]);
					//operand1 = machine->GetByteAt(address + 1, &validAccess);
					if (!validAccess) {
						badAccess = true;
						badLocation = address + 1;
						goto exit;
					}
					validAccess = regHelper.FindRegisterPair(operands[0], operands[1], &regPair);
					if (!validAccess) {
						badAccess = true;
						badLocation = address + 1;
						goto exit;
					}
					//std::cout << *pInstr << " " << *pReg;
					printer.AddString(*pInstr);
					printer.AddChar(' ');
					printer.AddChar(regPair.r1);
					//std::cout << " " << *pReg << "\n";
					printer.AddChar(' ');
					printer.AddChar(regPair.r2);
					printer.AddChar('\n');
					break;
				case LOAD_CODE:
				case STORE_CODE:
				case MVI_CODE:
					//operand1 = machine->GetByteAt(address + 1, &validAccess);
					validAccess = machine->GetAtMost4Bytes(address + 1, 2, &operands[0]);
					if (!validAccess) {
						badAccess = true;
						badLocation = address + 1;
						goto exit;
					}
					//pReg = revRegisterTable.Lookup(operand1, &foundRegister);
					reg = regHelper.FindRegisterChar(operands[0]);
					if (reg != '\0') {
						//std::cout << *pInstr << " " << *pReg;
						printer.AddString(*pInstr);
						printer.AddChar(' ');
						printer.AddChar(reg);
					}
					else {
						badExeFormat = true;
						badLocation = address + 1;
						goto exit;
					}
					/*operand2 = machine->GetByteAt(address + 2, &validAccess);
					if (!validAccess) {
						badAccess = true;
						badLocation = address + 2;
						goto exit;
					}*/
					//std::cout << " " << (int)operand2 << "\n";
					printer.AddChar(' ');
					printer.AddInt((int)operands[1]);
					printer.AddChar('\n');
					break;
				case ADD3_CODE:
				case SUB3_CODE:
					validAccess = machine->GetAtMost4Bytes(address + 1, 3, &operands[0]);
					if (!validAccess) {
						badAccess = true;
						badLocation = address + 1;
						goto exit;
					}
					validAccess = regHelper.FindRegisterTriplet(
						operands[0], 
						operands[1], 
						operands[2], 
						& regTriplet);
					if (!validAccess) {
						badAccess = true;
						badLocation = address + 1;
						goto exit;
					}
					//std::cout << *pInstr << " " << *pReg;
					printer.AddString(*pInstr);
					printer.AddChar(' ');
					printer.AddChar(regTriplet.r1);
					//std::cout << " " << *pReg;
					printer.AddChar(' ');
					printer.AddChar(regTriplet.r2);
					//std::cout << " " << *pReg << "\n";
					printer.AddChar(' ');
					printer.AddChar(regTriplet.r3);
					printer.AddChar('\n');
					break;
				}
				address += instrLength;
			}
			else {
				badExeFormat = true;
				badLocation = address;
				goto exit;
			}
		}
	exit:
		if (badExeFormat) {
			//std::cout << "bad exe format at address: " << badLocation << "\n";
			printer.AddConstCString("bad exe format at address: ");
			printer.AddInt(badLocation);
			printer.AddChar('\n');
			printer.PrintAndFlush();
			return false;
		}
		else if (badAccess) {
			//std::cout << "unexpected access error for location: " << badLocation << "\n";
			printer.AddConstCString("unexpected access error for location: ");
			printer.AddInt(badLocation);
			printer.AddChar('\n');
			printer.PrintAndFlush();
			return false;
		}
		else {
			printer.PrintAndFlush();
			return true;
		}
	}
};

#endif
