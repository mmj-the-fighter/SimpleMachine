#ifndef _OPCODEINSTRUCTIONMAP_
#define _OPCODEINSTRUCTIONMAP_

#include <map>
#include <cstring>
#include <string>
#include "InstructionOpcodeMap.hpp"

//for disassmbly
class OpcodeInstructionMap
{
private:
	std::map<unsigned char, std::string> opinmap;
	std::map<unsigned char, int> opinlengthmap;
public:
	OpcodeInstructionMap() {
		opinmap[HLT_CODE] = "HLT";
		opinmap[ADD_CODE] = "ADD";
		opinmap[SUB_CODE] = "SUB";
		opinmap[ADD3_CODE] = "ADD3";
		opinmap[SUB3_CODE] = "SUB3";
		opinmap[JNZ_CODE] = "JNZ";
		opinmap[JZ_CODE] = "JZ";
		opinmap[MOV_CODE] = "MOV";
		opinmap[LOAD_CODE] = "LOAD";
		opinmap[LDR_CODE] = "LDR";
		opinmap[STORE_CODE] = "STORE";
		opinmap[STR_CODE] = "STR";
		opinmap[INC_CODE] = "INC";
		opinmap[DCR_CODE] = "DCR";
		opinmap[MVI_CODE] = "MVI";
		opinmap[DISP_CODE] = "DISP";

		opinlengthmap[HLT_CODE] = 1;
		opinlengthmap[ADD_CODE] = 3;
		opinlengthmap[SUB_CODE] = 3;
		opinlengthmap[ADD3_CODE] = 4;
		opinlengthmap[SUB3_CODE] = 4;
		opinlengthmap[JNZ_CODE] = 2;
		opinlengthmap[JZ_CODE] = 2;
		opinlengthmap[MOV_CODE] = 3;
		opinlengthmap[LOAD_CODE] = 3;
		opinlengthmap[LDR_CODE] = 3;
		opinlengthmap[STORE_CODE] = 3;
		opinlengthmap[STR_CODE] = 3;
		opinlengthmap[INC_CODE] = 2;
		opinlengthmap[DCR_CODE] = 2;
		opinlengthmap[MVI_CODE] = 3;
		opinlengthmap[DISP_CODE] = 2;
	}

	std::string* GetInstruction(unsigned char opcode, bool* found) {
		std::map<unsigned char, std::string>::iterator it = opinmap.find(opcode);

		if (it != opinmap.end()) {
			*found = true;
			return &it->second;
		}
		else {
			*found = false;
			return nullptr;
		}
	}

	int GetInstructionLengthForOpcode(unsigned char opcode) {
		int len = 0;
		std::map<unsigned char, int>::iterator it = opinlengthmap.find(opcode);

		if (it != opinlengthmap.end()) {
			len = it->second;
		}

		return len;
	}
};

#endif
