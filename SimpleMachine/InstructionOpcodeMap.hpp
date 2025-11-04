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
	unsigned char inlengtharray[MAXOPCODES];

	std::map<unsigned char, std::string> opinmap;
	InstructionOpcodeMap() {
		inopmap["HLT"] = HLT_CODE;
		inopmap["ADD2"] = ADD2_CODE;
		inopmap["SUB2"] = SUB2_CODE;
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
		inopmap["ADD1"] = ADD1_CODE;
		inopmap["SUB1"] = SUB1_CODE;
		inopmap["PUSH"] = PUSH_CODE;
		inopmap["POP"] = POP_CODE;
		inopmap["CALL"] = CALL_CODE;
		inopmap["RET"] = RET_CODE;
		inopmap["CMP"] = CMP_CODE;
		inopmap["JLT"] = JLT_CODE;
		inopmap["JEQ"] = JEQ_CODE;
		inopmap["JGT"] = JGT_CODE;
		inopmap["JNEQ"] = JNEQ_CODE;


		opinmap[HLT_CODE] = "HLT";
		opinmap[ADD2_CODE] = "ADD2";
		opinmap[SUB2_CODE] = "SUB2";
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
		opinmap[ADD1_CODE] = "ADD1";
		opinmap[SUB1_CODE] = "SUB1";
		opinmap[PUSH_CODE] = "PUSH";
		opinmap[POP_CODE] = "POP";
		opinmap[CALL_CODE] = "CALL";
		opinmap[RET_CODE] = "RET";
		opinmap[CMP_CODE] = "CMP";
		opinmap[JLT_CODE] = "JLT";
		opinmap[JEQ_CODE] = "JEQ";
		opinmap[JGT_CODE] = "JGT";
		opinmap[JNEQ_CODE] = "JNEQ";


		inlengtharray[HLT_CODE] = HLT_CODE_LENGTH;
		inlengtharray[ADD2_CODE] = ADD2_CODE_LENGTH;
		inlengtharray[SUB2_CODE] = SUB2_CODE_LENGTH;
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
		inlengtharray[ADD1_CODE] = ADD1_CODE_LENGTH;
		inlengtharray[SUB1_CODE] = SUB1_CODE_LENGTH;
		inlengtharray[PUSH_CODE] = PUSH_CODE_LENGTH;
		inlengtharray[POP_CODE] = POP_CODE_LENGTH;
		inlengtharray[CALL_CODE] = CALL_CODE_LENGTH;
		inlengtharray[RET_CODE] = RET_CODE_LENGTH;
		inlengtharray[CMP_CODE] = CMP_CODE_LENGTH;
		inlengtharray[JLT_CODE] = JLT_CODE_LENGTH;
		inlengtharray[JEQ_CODE] = JEQ_CODE_LENGTH;
		inlengtharray[JGT_CODE] = JGT_CODE_LENGTH;
		inlengtharray[JNEQ_CODE] = JNEQ_CODE_LENGTH;

	}

public:

	inline static InstructionOpcodeMap& GetInstance() {
		static InstructionOpcodeMap instance;
		return instance;
	}


	inline bool GetOpcodeAndInstrLength(char *str, unsigned char *pOpcode, unsigned char *pInstrLength) {
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

	inline std::string* GetOpcodeStrAndInstrLength(unsigned char opcode, unsigned char* pInstrLength, bool *found) {
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

	inline unsigned char GetInstructionLengthForOpcode(unsigned char code) {
		if (code >= MAXOPCODES) {
			return 0;
		}
		return inlengtharray[code];
	}

};

#endif
