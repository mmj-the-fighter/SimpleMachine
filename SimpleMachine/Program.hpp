#ifndef _PROGRAM_
#define _PROGRAM_
#include <string>
#include "InstructionOpcodeMap.hpp"

class Program
{
private:
	unsigned char hexcode[256];
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

	bool WriteIfHalt(char *opcode){
		if (currentMarker >= 256 - 3){
			return false;
		}
		if (imap->isHaltInstruction(opcode))
		{
			hexcode[currentMarker] = HLT_CODE;
			hexcode[currentMarker + 1] = 0;
			hexcode[currentMarker + 2] = 0;
			currentMarker = currentMarker + 3;
			return true;
		}
		return false;
	}

	void WriteInstruction(char* opcode, char* operand1, char* operand2){
		if (currentMarker >= 256 - 3){
			return;
		}
		unsigned char code = imap->GetOpcode(opcode);
		if (code == 0xFF){
			return;
		}
		unsigned char op1 = atoi(operand1);
		unsigned char op2 = atoi(operand2);
		
		hexcode[currentMarker] = code;
		hexcode[currentMarker + 1] = op1;
		hexcode[currentMarker + 2] = op2;
		currentMarker = currentMarker + 3;
	}

	int GetCurrentMarker(){
		return currentMarker;
	}

	unsigned char* GetHexcodePointer(){
		return &hexcode[0];
	}
	
};

#endif