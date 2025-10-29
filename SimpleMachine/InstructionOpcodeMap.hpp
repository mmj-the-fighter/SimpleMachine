#ifndef _INSTRUCTIONOPCODEMAP_
#define _INSTRUCTIONOPCODEMAP_

#include <map>
#include <cstring>
#include <string>

#include "CommonDefs.h"


class InstructionOpcodeMap
{
private:
	std::map<std::string, unsigned char> inopmap;
	int inlengtharray[MAXOPCODES];

	std::map<unsigned char, std::string> opinmap;
	InstructionOpcodeMap() {
		inopmap["HLT"] = HLT_CODE;
		inopmap["ADD"] = ADD_CODE;
		inopmap["SUB"] = SUB_CODE;
		inopmap["ADD3"] = ADD3_CODE;
		inopmap["SUB3"] = SUB3_CODE;
		inopmap["JNZ"] = JNZ_CODE;
		inopmap["JZ"] = JZ_CODE;
		inopmap["MOV"] = MOV_CODE;
		inopmap["LOAD"] = LOAD_CODE;
		inopmap["LDR"] = LDR_CODE;
		inopmap["STORE"] = STORE_CODE;
		inopmap["STR"] = STORE_CODE;
		inopmap["INC"] = INC_CODE;
		inopmap["DCR"] = DCR_CODE;
		inopmap["MVI"] = MVI_CODE;
		inopmap["DISP"] = DISP_CODE;

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

		inlengtharray[HLT_CODE] = HLT_CODE_LENGTH;
		inlengtharray[ADD_CODE] = ADD_CODE_LENGTH;
		inlengtharray[SUB_CODE] = SUB_CODE_LENGTH;
		inlengtharray[ADD3_CODE] = ADD3_CODE_LENGTH;
		inlengtharray[SUB3_CODE] = SUB3_CODE_LENGTH;
		inlengtharray[JNZ_CODE] = JNZ_CODE_LENGTH;
		inlengtharray[JZ_CODE] = JZ_CODE_LENGTH;
		inlengtharray[MOV_CODE] = MOV_CODE_LENGTH;
		inlengtharray[LOAD_CODE] = LOAD_CODE_LENGTH;
		inlengtharray[LDR_CODE] = LDR_CODE_LENGTH;
		inlengtharray[STORE_CODE] = STORE_CODE_LENGTH;
		inlengtharray[STR_CODE] = STR_CODE_LENGTH;
		inlengtharray[INC_CODE] = INC_CODE_LENGTH;
		inlengtharray[DCR_CODE] = DCR_CODE_LENGTH;
		inlengtharray[MVI_CODE] = MVI_CODE_LENGTH;
		inlengtharray[DISP_CODE] = DISP_CODE_LENGTH;
	}

public:

	inline static InstructionOpcodeMap& GetInstance() {
		static InstructionOpcodeMap instance;
		return instance;
	}


	inline bool GetOpcodeAndInstrLength(char *str, unsigned char *pOpcode, int *pInstrLength) {
		unsigned char opcode = 0xFF;
		std::string keyToFind(str);
		std::map<std::string, unsigned char>::iterator it = inopmap.find(keyToFind);
		if (it != inopmap.end()) {
			opcode = it->second;
			if (opcode < MAXOPCODES) {
				*pInstrLength = inlengtharray[opcode];
				*pOpcode = opcode;
				return true;
			}
			else {
				return false;
			}
		}
		else {
			return false;
		}
	}

	inline std::string* GetOpcodeStrAndInstrLength(unsigned char opcode, int* pInstrLength, bool *found) {
		if (opcode >= MAXOPCODES) {
			*found = false;
			return nullptr;
		}
		*pInstrLength = inlengtharray[opcode];
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

	inline int GetInstructionLengthForOpcode(unsigned char code) {
		if (code >= MAXOPCODES) {
			return 0;
		}
		return inlengtharray[code];
	}

};

#endif
