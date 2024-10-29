#ifndef _SIMPLEMACHINEFACADE_
#define _SIMPLEMACHINEFACADE_

#include "TextFileLoader.hpp"
#include "InstructionOpcodeMap.hpp"
#include "Program.hpp"
#include "Machine.hpp"
#include "Assembler.hpp"

class SimpleMachineFacade
{
	TextFileLoader textFileLoader;
	InstructionOpcodeMap opcodeMap;
	Machine machine;
	Program program;
	Assembler assembler;

public:
	SimpleMachineFacade(){
		program.SetInstructionOpcodeMap(&opcodeMap);
		assembler.Set(&textFileLoader, &program);
	}
	void ExecuteProgram(const char* programName){
		
		textFileLoader.LoadTextFromFile(programName);
		
		program.Clear();

		assembler.Translate();
		
		machine.LoadProgram(0, &program);
		machine.Execute();
	}

	void SetCell(unsigned int address, unsigned int content){
		machine.memory[address] = content;
	}

	unsigned int GetCell(unsigned int address){
		return machine.memory[address];
	}
};

#endif