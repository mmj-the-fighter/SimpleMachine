#ifndef _DISASSEMBLER_
#define _DISASSEMBLER_

#include <sstream>

#include "OpcodeInstructionMap.hpp"
#include "Machine.hpp"
#include "RevSymbolTable.hpp"
#include "CommonDefs.h"

class Disassembler
{
	Machine* machine;
	int loadingOffset;
	RevSymbolTable addressLabelTable;
	RevSymbolTable revRegisterTable;
	OpcodeInstructionMap opcodeInstructionMap;
public:
	Disassembler(Machine* m, int offset) {
		machine = m;
		loadingOffset = offset;
		InitRevRegisterTable();
	}

	Disassembler() {
		machine = NULL;
		loadingOffset = 0;
		InitRevRegisterTable();
	}

	std::string GenLabel(int count) {
		std::ostringstream ss;
		ss << "Label" << count;
		return ss.str();
	}

	void InitRevRegisterTable()
	{
		revRegisterTable.AddKeyValue(0, "A");
		revRegisterTable.AddKeyValue(1, "B");
		revRegisterTable.AddKeyValue(2, "C");
		revRegisterTable.AddKeyValue(3, "D");
		revRegisterTable.AddKeyValue(4, "E");
		revRegisterTable.AddKeyValue(5, "F");
		revRegisterTable.AddKeyValue(6, "G");
		revRegisterTable.AddKeyValue(7, "H");
		revRegisterTable.AddKeyValue(8, "I");
		revRegisterTable.AddKeyValue(9, "J");
		revRegisterTable.AddKeyValue(10, "K");
		revRegisterTable.AddKeyValue(11, "L");
		revRegisterTable.AddKeyValue(12, "M");
		revRegisterTable.AddKeyValue(13, "N");
		revRegisterTable.AddKeyValue(14, "O");
		revRegisterTable.AddKeyValue(15, "P");
	}

	void Set(Machine* m, int offset) {
		machine = m;
		loadingOffset = offset;
	}

	bool PassForSymbols() {
		int address = loadingOffset;
		int labelCount = 0;

		bool hltFound = false;
		bool instrFound;
		bool validAccess;

		while (!hltFound) {
			unsigned char opcode = machine->GetByteAt(address, &validAccess);
			if (!validAccess) {
				std::cout << "Unexpected access error\n";
				return false;
			}
			std::string* pInstr = opcodeInstructionMap.GetInstruction(opcode, &instrFound);
			if (instrFound) {
				//std::cout << *pInstr << std::endl;
				switch (opcode) {
				case HLT_CODE:
					hltFound = true;
					break;
				case JZ_CODE:
				case JNZ_CODE:
					unsigned char jumpAddr = machine->GetByteAt(address + 1, &validAccess);
					if (!validAccess) {
						std::cout << "bad exe format\n";
						return false;
					}
					addressLabelTable.AddKeyValue(jumpAddr, GenLabel(labelCount));
					++labelCount;
					break;
				}
				address += opcodeInstructionMap.GetInstructionLengthForOpcode(opcode);
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
		bool hltFound = false;
		bool validAccess;
		bool foundInstr;
		bool foundLabel;
		bool foundRegister;
		bool badExeFormat = false;
		bool badAccess = false;
		int badLocation = 0;
		
		//machine->memory[12] = 67;

		if (!PassForSymbols()) {
			return false;
		}

		std::cout << "Disassembly:\n";
		while (!hltFound) {
			std::string instr;
			unsigned char operand1;
			unsigned char operand2;
			unsigned char operand3;
			unsigned char opcode = machine->GetByteAt(address, &validAccess);
			if (!validAccess) {
				badLocation = address;
				badAccess = true;
				goto exit;
			}
			std::string* pReg;
			std::string* pInstr = opcodeInstructionMap.GetInstruction(opcode, &foundInstr);
			if (foundInstr) {
				if (!machine->IsValidAddres(address - loadingOffset)) {
					badLocation = address - loadingOffset;
					badAccess = true;
					goto exit;
				}
				std::string* pLabel = addressLabelTable.Lookup(address - loadingOffset, &foundLabel);
				if (foundLabel) {
					std::cout << *pLabel << ":\n";
				}

				switch (opcode) {
				case HLT_CODE:std::cout << "HLT\n";
					break;
				case JNZ_CODE:
				case JZ_CODE:
					operand1 = machine->GetByteAt(address + 1, &validAccess);
					if (!validAccess) {
						badAccess = true;
						badLocation = address + 1;
						goto exit;
					}
					pLabel = addressLabelTable.Lookup(operand1, &foundLabel);
					if (foundLabel) {
						std::cout << *pInstr << " " << *pLabel << "\n";
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
					operand1 = machine->GetByteAt(address + 1, &validAccess);
					if (!validAccess) {
						badAccess = true;
						badLocation = address + 1;
						goto exit;
					}
					pReg = revRegisterTable.Lookup(operand1, &foundRegister);
					if (foundRegister) {
						std::cout << *pInstr << " " << *pReg << "\n";
					}
					else {
						badExeFormat = true;
						badLocation = address + 1;
						goto exit;
					}
					break;
				case ADD_CODE:
				case SUB_CODE:
				case MOV_CODE:
				case LDR_CODE:
				case STR_CODE:
					operand1 = machine->GetByteAt(address + 1, &validAccess);
					if (!validAccess) {
						badAccess = true;
						badLocation = address + 1;
						goto exit;
					}
					pReg = revRegisterTable.Lookup(operand1, &foundRegister);
					if (foundRegister) {
						std::cout << *pInstr << " " << *pReg;
					}
					else {
						badExeFormat = true;
						badLocation = address + 1;
						goto exit;
					}
					operand2 = machine->GetByteAt(address + 2, &validAccess);
					if (!validAccess) {
						badAccess = true;
						badLocation = address + 2;
						goto exit;
					}
					pReg = revRegisterTable.Lookup(operand2, &foundRegister);
					if (foundRegister) {
						std::cout << " " << *pReg << "\n";
					}
					else {
						badExeFormat = true;
						badLocation = address + 2;
						goto exit;
					}
					break;
				case LOAD_CODE:
				case STORE_CODE:
				case MVI_CODE:
					operand1 = machine->GetByteAt(address + 1, &validAccess);
					if (!validAccess) {
						badAccess = true;
						badLocation = address + 1;
						goto exit;
					}
					pReg = revRegisterTable.Lookup(operand1, &foundRegister);
					if (foundRegister) {
						std::cout << *pInstr << " " << *pReg;
					}
					else {
						badExeFormat = true;
						badLocation = address + 1;
						goto exit;
					}
					operand2 = machine->GetByteAt(address + 2, &validAccess);
					if (!validAccess) {
						badAccess = true;
						badLocation = address + 2;
						goto exit;
					}
					std::cout << " " << (int)operand2 << "\n";
					break;
				case ADD3_CODE:
				case SUB3_CODE:
					operand1 = machine->GetByteAt(address + 1, &validAccess);
					if (!validAccess) {
						badAccess = true;
						badLocation = address + 1;
						goto exit;
					}
					pReg = revRegisterTable.Lookup(operand1, &foundRegister);
					if (foundRegister) {
						std::cout << *pInstr << " " << *pReg;
					}
					else {
						badExeFormat = true;
						badLocation = address + 1;
						goto exit;
					}
					operand2 = machine->GetByteAt(address + 2, &validAccess);
					if (!validAccess) {
						badAccess = true;
						badLocation = address + 2;
						goto exit;
					}
					pReg = revRegisterTable.Lookup(operand2, &foundRegister);
					if (foundRegister) {
						std::cout << " " << *pReg;
					}
					else {
						badExeFormat = true;
						badLocation = address + 2;
						goto exit;
					}
					operand3 = machine->GetByteAt(address + 3, &validAccess);
					if (!validAccess) {
						badAccess = true;
						badLocation = address + 3;
						goto exit;
					}
					pReg = revRegisterTable.Lookup(operand3, &foundRegister);
					if (foundRegister) {
						std::cout << " " << *pReg << "\n";
					}
					else {
						badExeFormat = true;
						badLocation = address + 3;
						goto exit;
					}
					break;
				}
				address += opcodeInstructionMap.GetInstructionLengthForOpcode(opcode);
			}
			else {
				badExeFormat = true;
				badLocation = address;
				goto exit;
			}
			if (opcode == HLT_CODE) {
				hltFound = true;
			}
		}
	exit:
		if (badExeFormat) {
			std::cout << "bad exe format at address: "<<badLocation<<"\n";
			return false;
		}
		else if (badAccess) {
			std::cout << "unexpected access error for location: " << badLocation << "\n";
			return false;
		}
		else {
			return true;
		}
	}
};

#endif
