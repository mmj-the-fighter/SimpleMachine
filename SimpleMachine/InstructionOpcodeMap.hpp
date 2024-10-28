#ifndef _INSTRUCTIONOPCODEMAP_
#define _INSTRUCTIONOPCODEMAP_

#include <map>
#include <cstring>


enum {
	HLT_CODE = 0x0,
	ADD_CODE = 0x1,
	SUB_CODE = 0x2,
	JNZ_CODE = 0x3,
	JZ_CODE = 0x4,
	MOV_CODE = 0x5,
	LOAD_CODE = 0x6,
	LDR_CODE = 0x7,
	STORE_CODE = 0x8,
	STR_CODE = 0x9,
};
class InstructionOpcodeMap
{
private:
	std::map<std::string, unsigned char> inopmap;
public:
	InstructionOpcodeMap(){
		inopmap["hlt"] = HLT_CODE;
		inopmap["add"] = ADD_CODE;
		inopmap["sub"] = SUB_CODE;
		inopmap["jnz"] = JNZ_CODE;
		inopmap["jz"] = JZ_CODE;
		inopmap["mov"] = MOV_CODE;
		inopmap["load"] = LOAD_CODE;
		inopmap["ldr"] = LDR_CODE;
		inopmap["store"] = STORE_CODE;
		inopmap["str"] = STORE_CODE;
	}

	unsigned char GetOpcode(std::string str){
		unsigned char opcode = 0xFF;
		std::string keyToFind(str);
		std::map<std::string, unsigned char>::iterator it = inopmap.find(keyToFind);
		if (it != inopmap.end()) {
			opcode = it->second;
		}
		return opcode;
	}

	bool isHaltInstruction(char * str){
		return strcmp(str, "hlt") == 0;
	}
};

#endif