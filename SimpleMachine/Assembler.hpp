#ifndef _ASSEMBLER_
#define _ASSEMBLER_

#include "InstructionOpcodeMap.hpp"
#include "Program.hpp"
#include "TextFileLoader.hpp"

class Assembler
{
	TextFileLoader* tfLoader;
	Program* program;
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
	

	void Translate(){
		int i = 0, k = 0;
		int marker = 0;
		char c, ch;
		char buffer[1024];
		unsigned char * hexcode = program->GetHexcodePointer();

		while ((c = tfLoader->GetTextAt(i)) != '\0'){
			k = 0;
			while (c != '\n' && c!='\0'){
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
	void Assemblehex(char* buffer){
		int i = 0;
		int k = 0;
		char c = buffer[i];
		char opcodestr[1024];
		char operand1[1024];
		char operand2[1024];
		bool isLabel = false;
		while (c != '\0' && (isalpha(c) || c==':')){
			opcodestr[k] = c;
			++k;
			++i;
			c = buffer[i];
			if (c == ':'){
				isLabel = true;
			}
		}
		opcodestr[k] = '\0';
		if (isLabel){
			std::cout << opcodestr <<":"<< program->GetCurrentMarker() << std::endl;
			return;
		}

		if (program->WriteIfHalt(opcodestr)){
			return;
		}

		if (c == '\0'){
			return;
		}
		
		++i;
		c = buffer[i];
		k = 0;
		while (c != '\0' && isdigit(c)){
			operand1[k] = c;
			++k;
			++i;
			c = buffer[i];
		}
		if (c == '\0'){
			return;
		}
		operand1[k] = '\0';
		++i;
		c = buffer[i];
		k = 0;
		while (c != '\0' && isdigit(c)){
			operand2[k] = c;
			++k;
			++i;
			c = buffer[i];
		}
		operand2[k] = '\0';

		program->WriteInstruction(opcodestr, operand1, operand2);
		
	}
};

#endif