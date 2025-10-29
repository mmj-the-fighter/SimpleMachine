#ifndef _ASSEMBLER_
#define _ASSEMBLER_

#include "InstructionOpcodeMap.hpp"
#include "Program.hpp"
#include "TextFileLoader.hpp"
#include "SymbolTable.hpp"
#include "CommonDefs.h"
#include "RegisterHelper.hpp"

class Assembler
{
	TextFileLoader* tfLoader;
	Program* program;
	SymbolTable labelTable;
	RegisterHelper registerHelper;
	InstructionOpcodeMap inOpMap;
public:
	Assembler(TextFileLoader* loader, Program* prog){
		program = prog;
	}

	Assembler(){
		program = NULL;
		tfLoader = NULL;
	}

	void Set(TextFileLoader* loader, Program* prog){
		program = prog;
		tfLoader = loader;
	}

	bool PassForSymbols() {
		int ssm = 0;
		char buffer[BUFFERLENGTH];
		int lineNumber = 0;

		while (true) {
			tfLoader->GetNonEmptyLine(buffer, BUFFERLENGTH, &ssm, &lineNumber);
			if (!StoreSymbolIfAny(buffer)) {
				std::cout << "Pass for symbols failed! "<<" at line# "<<lineNumber<<'\n';
				return false;
			}
			if (tfLoader->GetTextAt(ssm) == '\0') {
				break;
			}
		}
		return true;
	}


	bool Translate(){
		int ssm = 0;
		char buffer[BUFFERLENGTH];
		int lineNumber = 0;
		
		if (!PassForSymbols()){
			return false;
		}

		program->Clear();

		while (true) {
			tfLoader->GetNonEmptyLine(buffer, BUFFERLENGTH, &ssm, &lineNumber);
			if (!AssembleByteCode(buffer)) {
				std::cout << "Invalid instruction:" << buffer << "\nat line# " << lineNumber << "\n";
				return false;
			}
			if (tfLoader->GetTextAt(ssm) == '\0') {
				break;
			}
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
		unsigned char opcode;
		int instrLength;
		bool found;
		//read a line
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
		
		if (isLabel) {
			--k;
			if (k < 0) {
				return false;
			}
			opcodestr[k] = '\0';
		}
		else {
			opcodestr[k] = '\0';
		}

		bool isInstr = inOpMap.GetOpcodeAndInstrLength(opcodestr, &opcode, &instrLength);
		if (isInstr) {
			if (isLabel) {
				std::cout << "opcode " << opcodestr << " cannot be label\n";
				return false;
			}
			program->AdvanceMarker(instrLength);
			return true;
		}
		else if (isLabel) {
			labelTable.Lookup(opcodestr, &found);
			if (found) {
				std::cout << "label " << opcodestr << " is already used\n";
				return false;
			}
			labelTable.AddLabel(opcodestr, program->GetCurrentMarker());
			return true;
		}
		else {
			std::cout << "unidentified instruction" << opcodestr << "found\n";
			return false;
		}
	}

	bool AssembleByteCode(char* buffer) {
		//util::ProfilerScope prof(1729);
		int i = 0;
		int j = 0;
		int k = 0;
		int count = 0;
		char c = buffer[i];
		char opcodestr[BUFFERLENGTH];
		char operandBuffer[BUFFERLENGTH];
		char operandArray[3][BUFFERLENGTH];
		bool isLabel = false;
		RegisterNumPair regNumPair;
		RegisterNumTriplet regNumTriplet;

		//Read first string
		while (k < BUFFERLENGTH - 1 && c != '#' && c != '\0' && (isalnum(c) || c == ':')) {
			opcodestr[k] = c;
			++k;
			++i;
			if (i < BUFFERLENGTH) {
				c = buffer[i];
			}
			else {
				c = '\0';
			}

			if (c == ':') {
				isLabel = true;
			}
		}
		if (k >= BUFFERLENGTH - 1) {
			return false;
		}
		//If comment do nothing, read next line
		if (c == '#') {
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
		bool isInstr = inOpMap.GetOpcodeAndInstrLength(opcodestr, &opcode, &instructionLength);
		if (!isInstr) {
			return false;
		}


		for (count = 0; count < instructionLength - 1; count++) {
			++i;
			if (i < BUFFERLENGTH) {
				c = buffer[i];
			}
			else {
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
		bool found;
		switch (opcode) {
		case HLT_CODE:
			program->WriteCode1Byte(opcode);
			break;
		case JNZ_CODE:
		case JZ_CODE:
			address = labelTable.Lookup(&operandArray[0][0], &found);
			if (!found) {
				return false;
			}
			program->WriteCode2Bytes(opcode, address);
			break;
		case INC_CODE:
		case DCR_CODE:
		case DISP_CODE:
			regAddr1 = registerHelper.FindRegisterNumber(operandArray[0][0]);
			if (0xFF == regAddr1) {
				return false;
			}
			program->WriteCode2Bytes(opcode, regAddr1);
			break;
		case ADD_CODE:
		case SUB_CODE:
		case MOV_CODE:
		case LDR_CODE:
		case STR_CODE:
			if (!registerHelper.FindRegisterNumberPair(
				operandArray, &regNumPair)
				) {
				return false;
			}
			program->WriteCode3BytesFromOpcodeRegNumPair(opcode, &regNumPair);
			break;
		case LOAD_CODE:
		case STORE_CODE:
		case MVI_CODE:
			regAddr1 = registerHelper.FindRegisterNumber(operandArray[0][0]);
			if (0xFF == regAddr1) {
				return false;
			}
			op2 = atoi(&operandArray[1][0]);
			program->WriteCode3Bytes(opcode, regAddr1, op2);
			break;
		case ADD3_CODE:
		case SUB3_CODE:
			if (!registerHelper.FindRegisterNumberTriplet(
				operandArray, &regNumTriplet)
				) {
				return false;
			}
			program->WriteCode4BytesFromOpcodeRegNumTriplet(opcode, &regNumTriplet);
			break;
		}
		return true;
	}
};

#endif
