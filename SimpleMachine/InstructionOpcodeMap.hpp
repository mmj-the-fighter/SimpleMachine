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
	std::map<unsigned char, int> inlengthmap;
	std::map<std::string, int> inlengthmapstr;
	int inlengtharray[MAXOPCODES];
public:
	InstructionOpcodeMap(){
		inopmap["hlt"] = HLT_CODE;
		inopmap["add"] = ADD_CODE;
		inopmap["sub"] = SUB_CODE;
		inopmap["add3"] = ADD3_CODE;
		inopmap["sub3"] = SUB3_CODE;
		inopmap["jnz"] = JNZ_CODE;
		inopmap["jz"] = JZ_CODE;
		inopmap["mov"] = MOV_CODE;
		inopmap["load"] = LOAD_CODE;
		inopmap["ldr"] = LDR_CODE;
		inopmap["store"] = STORE_CODE;
		inopmap["str"] = STORE_CODE;
		inopmap["inc"] = INC_CODE;
		inopmap["dcr"] = DCR_CODE;
		inopmap["mvi"] = MVI_CODE;
		inopmap["disp"] = DISP_CODE;

		inlengthmap[HLT_CODE] = 1;
		inlengthmap[ADD_CODE] = 3;
		inlengthmap[SUB_CODE] = 3;
		inlengthmap[ADD3_CODE] = 4;
		inlengthmap[SUB3_CODE] = 4;
		inlengthmap[JNZ_CODE] = 2;
		inlengthmap[JZ_CODE] = 2;
		inlengthmap[MOV_CODE] = 3;
		inlengthmap[LOAD_CODE] = 3;
		inlengthmap[LDR_CODE] = 3;
		inlengthmap[STORE_CODE] = 3;
		inlengthmap[STR_CODE] = 3;
		inlengthmap[INC_CODE] = 2;
		inlengthmap[DCR_CODE] = 2;
		inlengthmap[MVI_CODE] = 3;
		inlengthmap[DISP_CODE] = 2;

		inlengtharray[HLT_CODE] = 1;
		inlengtharray[ADD_CODE] = 3;
		inlengtharray[SUB_CODE] = 3;
		inlengtharray[ADD3_CODE] = 4;
		inlengtharray[SUB3_CODE] = 4;
		inlengtharray[JNZ_CODE] = 2;
		inlengtharray[JZ_CODE] = 2;
		inlengtharray[MOV_CODE] = 3;
		inlengtharray[LOAD_CODE] = 3;
		inlengtharray[LDR_CODE] = 3;
		inlengtharray[STORE_CODE] = 3;
		inlengtharray[STR_CODE] = 3;
		inlengtharray[INC_CODE] = 2;
		inlengtharray[DCR_CODE] = 2;
		inlengtharray[MVI_CODE] = 3;
		inlengtharray[DISP_CODE] = 2;



		inlengthmapstr["hlt"] = 1;
		inlengthmapstr["add"] = 3;
		inlengthmapstr["sub"] = 3;
		inlengthmapstr["add3"] = 4;
		inlengthmapstr["sub3"] = 4;
		inlengthmapstr["jnz"] = 2;
		inlengthmapstr["jz"] = 2;
		inlengthmapstr["mov"] = 3;
		inlengthmapstr["load"] = 3;
		inlengthmapstr["ldr"] = 3;
		inlengthmapstr["store"] = 3;
		inlengthmapstr["str"] = 3;
		inlengthmapstr["inc"] = 2;
		inlengthmapstr["dcr"] = 2;
		inlengthmapstr["mvi"] = 3;
		inlengthmapstr["disp"] = 2;

	}

	unsigned char GetOpcode(char *str, bool *found){
		unsigned char opcode = 0xFF;
		std::string keyToFind(str);
		std::map<std::string, unsigned char>::iterator it = inopmap.find(keyToFind);
		if (it != inopmap.end()) {
			opcode = it->second;
			*found = true;
		}
		else {
			*found = false;
		}
		return opcode;
	}

	int GetInstructionLengthForOpcode(unsigned char code){
		int len = 0;
		std::map<unsigned char, int>::iterator it = inlengthmap.find(code);
		if (it != inlengthmap.end()) {
			len = it->second;
		}
		return len;
	}

	int GetInstructionLengthForOpcodeFast(unsigned char code) {
		if (code >= MAXOPCODES) {
			return 0;
		}
		return inlengtharray[code];
	}

	int GetInstructionLengthForOpcodeStr(char *str){
		int len = 0;
		std::string keyToFind(str);
		std::map<std::string, int>::iterator it = inlengthmapstr.find(keyToFind);
		if (it != inlengthmapstr.end()) {
			len = it->second;
		}
		return len;
	}

};

#endif
