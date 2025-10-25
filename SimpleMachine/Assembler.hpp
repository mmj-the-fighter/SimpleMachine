#ifndef _ASSEMBLER_
#define _ASSEMBLER_

#include "InstructionOpcodeMap.hpp"
#include "Program.hpp"
#include "TextFileLoader.hpp"
#include "SymbolTable.hpp"
#include "CommonDefs.h"

class Assembler
{
	TextFileLoader* tfLoader;
	Program* program;
	SymbolTable table;
	SymbolTable registerTable;
public:
	Assembler(TextFileLoader* loader, Program* prog){
		program = prog;
		tfLoader = loader;
		InitRegisterTable();
	}

	Assembler(){
		program = NULL;
		tfLoader = NULL;
		InitRegisterTable();
	}

	void InitRegisterTable()
	{
		registerTable.AddLabel("a", 0);
		registerTable.AddLabel("b", 1);
		registerTable.AddLabel("c", 2);
		registerTable.AddLabel("d", 3);
		registerTable.AddLabel("e", 4);
		registerTable.AddLabel("f", 5);
		registerTable.AddLabel("g", 6);
		registerTable.AddLabel("h", 7);
		registerTable.AddLabel("i", 8);
		registerTable.AddLabel("j", 9);
		registerTable.AddLabel("k", 10);
		registerTable.AddLabel("l", 11);
		registerTable.AddLabel("m", 12);
		registerTable.AddLabel("n", 13);
		registerTable.AddLabel("o", 14);
	}

	void Set(TextFileLoader* loader, Program* prog){
		program = prog;
		tfLoader = loader;
	}

	bool PassForSymbols(){
		int i = 0, k = 0;
		int marker = 0;
		char c, ch;
		char buffer[BUFFERLENGTH];
		unsigned char * byteCode = program->GetByteCodePointer();
		int lineNum = 1;

		while ((c = tfLoader->GetTextAt(i)) != '\0') {
			k = 0;
			while (k < BUFFERLENGTH-1 && c != '\n' && c != '\0'){
				buffer[k] = c;
				++k;
				++i;
				c = tfLoader->GetTextAt(i);
			}
			if (k >= BUFFERLENGTH-1){
				return false;
			}
			buffer[k] = '\0';
			if (!StoreSymbolIfAny(buffer)){
				return false;
			}
			++i;
		}
		return true;
	}


	bool Translate(){
		int i = 0, k = 0;
		int marker = 0;
		char c, ch;
		char buffer[BUFFERLENGTH];
		unsigned char * byteCode = program->GetByteCodePointer();
		program->Clear();
		if (!PassForSymbols()){
			return false;
		}
		program->Clear();
		while ((c = tfLoader->GetTextAt(i)) != '\0'){
			k = 0;
			while (k < BUFFERLENGTH-1 && c != '\n' && c != '\0'){
				buffer[k] = c;
				++k;
				++i;
				c = tfLoader->GetTextAt(i);
			}
			if (k >= BUFFERLENGTH - 1){
				return false;
			}
			buffer[k] = '\0';
			if (!AssembleByteCode(buffer)){
				return false;
			}
			++i;
		}
		return true;
	}
private:
	//returns false if buffer is overflowed
	bool StoreSymbolIfAny(char* buffer){
		int i = 0;
		int k = 0;
		char c = buffer[i];
		char opcodestr[BUFFERLENGTH];
		bool isLabel = false;
		while (k < BUFFERLENGTH-1 && c != '#' && c != '\0' && (isalpha(c) || c == ':')){
			opcodestr[k] = c;
			++k;
			++i;
			if (i < BUFFERLENGTH){
				c = buffer[i];
			}
			else{
				c = '\0';
			}
			
			if (c == ':'){
				isLabel = true;
			}
		}
		if (k >= BUFFERLENGTH - 1){
			return false;
		}
		if (c == '#') {
			return true;
		}
		opcodestr[k] = '\0';
		if (isLabel){
			--k;
			if (k < 0) {
				return false;
			}
			opcodestr[k] = '\0';
			table.AddLabel(opcodestr, program->GetCurrentMarker());
			return true;
		}
		program->WriteInstructionFirstPass(opcodestr);
		return true;
	}

	bool AssembleByteCode(char* buffer){
		int i = 0;
		int j = 0;
		int k = 0;
		int count = 0;
		char c = buffer[i];
		char opcodestr[BUFFERLENGTH];
		char operandBuffer[BUFFERLENGTH];
		char operandArray[3][BUFFERLENGTH];
		bool isLabel = false;

		//Read first string
		while (k<BUFFERLENGTH-1 && c != '#' && c != '\0' && (isalnum(c) || c == ':')){
			opcodestr[k] = c;
			++k;
			++i;
			if (i < BUFFERLENGTH){
				c = buffer[i];
			}
			else{
				c = '\0';
			}
			
			if (c == ':'){
				isLabel = true;
			}
		}
		if (k >= BUFFERLENGTH - 1){
			return false;
		}
		//If comment do nothing, read next line
		if (c == '#'){
			return true;
		}
		opcodestr[k] = '\0';
		//if label do nothing, read next line
		if (isLabel) {
			//std::cout << opcodestr <<":"<< program->GetCurrentMarker() << std::endl;
			return true;
		}

		unsigned char opcode;
		int instructionLength;
		program->GetOpcodeAndLength(opcodestr, &opcode, &instructionLength);
		if (instructionLength < 1 || instructionLength > 4){
			return false;
		}


		for (count = 0; count < instructionLength-1; count++) {
			++i;
			if (i < BUFFERLENGTH){
				c = buffer[i];
			}
			else{
				c = '\0';
			}
			k = 0;
			while (i < BUFFERLENGTH && k < BUFFERLENGTH - 1 && c != '\0' && !isspace(c)) {
				operandBuffer[k] = c;
				++k;
				++i;
				c = buffer[i];
			}
			if (k >= BUFFERLENGTH - 1) {
				return false;
			}
			operandBuffer[k] = '\0';
			j = 0;
			while (j < BUFFERLENGTH && (operandArray[count][j] = operandBuffer[j]) != '\0') {
				++j;
			}
		}

		int address = 0;
		unsigned char op1 = 0;
		unsigned char op2 = 0;
		unsigned char op3 = 0;
		int regAddr1 = 0;
		int regAddr2 = 0;
		int regAddr3 = 0;
		switch (opcode) {
		case HLT_CODE:
			program->WriteCode1Byte(opcode);
			break;
		case JNZ_CODE:
		case JZ_CODE:
			address = table.Lookup(&operandArray[0][0]);
			if (address >= 0){
				program->WriteCode2Bytes(opcode, address);
			}
			break;
		case INC_CODE:
		case DCR_CODE:
		case DISP_CODE:
			regAddr1 = registerTable.Lookup(&operandArray[0][0]);
			program->WriteCode2Bytes(opcode, regAddr1);
			break;
		case ADD_CODE:
		case SUB_CODE:
		case MOV_CODE:
		case LDR_CODE:
		case STR_CODE:
			regAddr1 = registerTable.Lookup(&operandArray[0][0]);
			regAddr2 = registerTable.Lookup(&operandArray[1][0]);
			program->WriteCode3Bytes(opcode, regAddr1, regAddr2);
			break;
		case LOAD_CODE:
		case STORE_CODE:
		case MVI_CODE:
			regAddr1 = registerTable.Lookup(&operandArray[0][0]);
			op2 = atoi(&operandArray[1][0]);
			program->WriteCode3Bytes(opcode, regAddr1, op2);
			break;
		case ADD3_CODE:
		case SUB3_CODE:
			regAddr1 = registerTable.Lookup(&operandArray[0][0]);
			regAddr2 = registerTable.Lookup(&operandArray[1][0]);
			regAddr3 = registerTable.Lookup(&operandArray[2][0]);
			program->WriteCode4Bytes(opcode,regAddr1, regAddr2, regAddr3);
			break;
		}
		return true;
	}
};

#endif
