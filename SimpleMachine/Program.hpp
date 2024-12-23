#ifndef _PROGRAM_
#define _PROGRAM_
#include <string>
#include "InstructionOpcodeMap.hpp"
#include "SymbolTable.hpp"
#include "CommonDefs.h"

class Program
{
private:
	unsigned char byteCode[MAXMEMBYTES];
	int currentMarker;
	InstructionOpcodeMap *imap;
public:
	Program()
	{
		currentMarker = 0;
		imap = NULL;
	}
	Program(InstructionOpcodeMap *map)
	{
		currentMarker = 0;
		imap = map;
	}

	void SetInstructionOpcodeMap(InstructionOpcodeMap *map){
		imap = map;
	}

	void Clear(){
		currentMarker = 0;
	}

	void GetOpcodeAndLength(char *opcodeStr, unsigned char* opcode, int *len){
		*opcode = imap->GetOpcode(opcodeStr);
		*len = imap->GetInstructionLengthForOpcode(*opcode);
	}

	int GetInstructionLength(unsigned char opcode) {
		return imap->GetInstructionLengthForOpcode(opcode);
	}

	void WriteInstructionFirstPass(char *opcodeStr){
		int instrLen = imap->GetInstructionLengthForOpcodeStr(opcodeStr);
		if (currentMarker >= MAXMEMBYTES - instrLen){
			return;
		}
		currentMarker = currentMarker + instrLen;
	}

	void WriteCode1Byte(unsigned char byte1){
		if (currentMarker >= MAXMEMBYTES - 1){
			return;
		}
		byteCode[currentMarker] = byte1;
		currentMarker += 1;
	}

	void WriteCode2Bytes(unsigned char byte1, unsigned char byte2){
		if (currentMarker >= MAXMEMBYTES - 2){
			return;
		}
		byteCode[currentMarker] = byte1;
		byteCode[currentMarker + 1] = byte2;
		currentMarker += 2;
	}

	void WriteCode3Bytes(unsigned char byte1, unsigned char byte2, unsigned char byte3){
		if (currentMarker >= MAXMEMBYTES - 3){
			return;
		}
		byteCode[currentMarker] = byte1;
		byteCode[currentMarker + 1] = byte2;
		byteCode[currentMarker + 2] = byte3;
		currentMarker += 3;
	}

	void WriteCode4Bytes(unsigned char byte1, unsigned char byte2, unsigned char byte3, unsigned char byte4){
		if (currentMarker >= MAXMEMBYTES - 4){
			return;
		}
		byteCode[currentMarker] = byte1;
		byteCode[currentMarker + 1] = byte2;
		byteCode[currentMarker + 2] = byte3;
		byteCode[currentMarker + 3] = byte4;
		currentMarker += 4;
	}

	
	int GetCurrentMarker() {
		return currentMarker;
	}

	unsigned char* GetByteCodePointer() {
		return &byteCode[0];
	}
	
};

#endif