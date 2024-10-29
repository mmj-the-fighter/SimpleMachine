#ifndef _ASSEMBLER_
#define _ASSEMBLER_

#include "InstructionOpcodeMap.hpp"
#include "Program.hpp"
#include "TextFileLoader.hpp"
#include "SymbolTable.hpp"

class Assembler
{
	TextFileLoader* tfLoader;
	Program* program;
	SymbolTable table;
public:
	Assembler(TextFileLoader* loader, Program* prog){
		program = prog;
		tfLoader = loader;
	}

	Assembler(){
		program = NULL;
		tfLoader = NULL;
	}

	void Set(TextFileLoader* loader, Program* prog){
		program = prog;
		tfLoader = loader;
	}

	void PassForSymbols(){
		int i = 0, k = 0;
		int marker = 0;
		char c, ch;
		char buffer[1024];
		unsigned char * hexcode = program->GetHexcodePointer();

		while ((c = tfLoader->GetTextAt(i)) != '\0'){
			k = 0;
			while (c != '\n' && c != '\0'){
				buffer[k] = c;
				++k;
				++i;
				c = tfLoader->GetTextAt(i);
			}
			buffer[k] = '\0';
			StoreSymbolIfAny(buffer);
			++i;
		}
	}


	void Translate(){
		int i = 0, k = 0;
		int marker = 0;
		char c, ch;
		char buffer[1024];
		unsigned char * hexcode = program->GetHexcodePointer();
		program->Clear();
		PassForSymbols();
		program->Clear();
		while ((c = tfLoader->GetTextAt(i)) != '\0'){
			k = 0;
			while (c != '\n' && c != '\0'){
				buffer[k] = c;
				++k;
				++i;
				c = tfLoader->GetTextAt(i);
			}
			buffer[k] = '\0';
			Assemblehex(buffer);
			++i;
		}
	}
private:
	void StoreSymbolIfAny(char* buffer){
		int i = 0;
		int k = 0;
		char c = buffer[i];
		char opcodestr[1024];
		bool isLabel = false;
		while (c != '#' && c != '\0' && (isalpha(c) || c == ':')){
			opcodestr[k] = c;
			++k;
			++i;
			c = buffer[i];
			if (c == ':'){
				isLabel = true;
			}
		}
		if (c == '#'){
			return;
		}
		opcodestr[k] = '\0';
		if (isLabel){
			--k;
			opcodestr[k] = '\0';
			table.AddLabel(opcodestr, program->GetCurrentMarker());
			return;
		}
		program->WriteInstructionFirstPass(opcodestr);
	}

	void Assemblehex(char* buffer){
		int i = 0;
		int j = 0;
		int k = 0;
		int count = 0;
		char c = buffer[i];
		char opcodestr[1024];
		char operandBuffer[1024];
		char* operandArray[3];
		bool isLabel = false;

		

		//Read first string
		while (c != '#' && c != '\0' && (isalnum(c) || c == ':')){
			opcodestr[k] = c;
			++k;
			++i;
			c = buffer[i];
			if (c == ':'){
				isLabel = true;
			}
		}
		//If comment do nothing, read next line
		if (c == '#'){
			return;
		}
		opcodestr[k] = '\0';
		//if label do nothing, read next line
		if (isLabel){
			//std::cout << opcodestr <<":"<< program->GetCurrentMarker() << std::endl;
			return;
		}

		unsigned char opcode;
		int instructionLength;
		program->GetOpcodeAndLength(opcodestr, &opcode, &instructionLength);

		for (count = 0; count < 3; count++){
			operandArray[count] = NULL;
		}

		for (count = 0; count < instructionLength-1; count++) {
			++i;
			c = buffer[i];
			k = 0;
			while (c != '\0' && !isspace(c)) {
				operandBuffer[k] = c;
				++k;
				++i;
				c = buffer[i];
			}
			operandBuffer[k] = '\0';
			operandArray[count] = new char[k + 1];
			j = 0;
			while ((operandArray[count][j] = operandBuffer[j]) != '\0') {
				++j;
			}
		}

		int address = 0;
		unsigned char op1 = 0;
		unsigned char op2 = 0;
		unsigned char op3 = 0;
		switch (opcode) {
		case HLT_CODE:
			program->WriteCode1Byte(opcode);
			break;
		case JNZ_CODE:
		case JZ_CODE:
			address = table.Lookup(operandArray[0]);
			if (address >= 0){
				program->WriteCode2Bytes(opcode, address);
			}
			break;
		case INC_CODE:
		case DCR_CODE:
			op1 = atoi(operandArray[0]);
			program->WriteCode2Bytes(opcode, op1);
			break;
		case ADD_CODE:
		case SUB_CODE:
		case MOV_CODE:
		case LOAD_CODE:
		case LDR_CODE:
		case STORE_CODE:
		case STR_CODE:
		case MVI_CODE:
			op1 = atoi(operandArray[0]);
			op2 = atoi(operandArray[1]);
			program->WriteCode3Bytes(opcode, op1, op2);
			break;
		case ADD3_CODE:
		case SUB3_CODE:
			op1 = atoi(operandArray[0]);
			op2 = atoi(operandArray[1]);
			op3 = atoi(operandArray[2]);
			program->WriteCode4Bytes(opcode, op1, op2, op3);
		}

		for (count = 0; count < 3; count++) {
			if (NULL != operandArray[count]) {
				delete operandArray[count];
				operandArray[count] = NULL;
			}
		}

	}
};

#endif