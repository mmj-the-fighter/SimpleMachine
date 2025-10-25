#ifndef _INSTRUCTIONOPCODEMAP_
#define _INSTRUCTIONOPCODEMAP_

#include <map>
#include <cstring>
#include <string>


enum {
	HLT_CODE = 0,
	ADD_CODE = 1,
	SUB_CODE = 2,
	JNZ_CODE = 3,
	JZ_CODE = 4,
	MOV_CODE = 5,
	LOAD_CODE = 6,
	LDR_CODE = 7,
	STORE_CODE = 8,
	STR_CODE = 9,
	INC_CODE = 10,
	DCR_CODE = 11,
	MVI_CODE = 12,
	ADD3_CODE = 13,
	SUB3_CODE = 14,
	DISP_CODE = 15
};
class InstructionOpcodeMap
{
private:
	std::map<std::string, unsigned char> inopmap;
	std::map<unsigned char, int> inlengthmap;
	std::map<std::string, int> inlengthmapstr;
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

	unsigned char GetOpcode(char *str){
		unsigned char opcode = 0xFF;
		std::string keyToFind(str);
		std::map<std::string, unsigned char>::iterator it = inopmap.find(keyToFind);
		if (it != inopmap.end()) {
			opcode = it->second;
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
