#ifndef _SIMPLEMACHINEFACADE_
#define _SIMPLEMACHINEFACADE_

#include "TextFileLoader.hpp"
#include "RamFileLoader.hpp"
#include "InstructionOpcodeMap.hpp"
#include "Program.hpp"
#include "Machine.hpp"
#include "Assembler.hpp"

class SimpleMachineFacade
{
	TextFileLoader textFileLoader;
	RamFileLoader ramFileLoader;
	InstructionOpcodeMap opcodeMap;
	Machine machine;
	Program program;
	Assembler assembler;

public:
	SimpleMachineFacade(){
		program.SetInstructionOpcodeMap(&opcodeMap);
		assembler.Set(&textFileLoader, &program);
	}

	void TranslateAssembly(const char* asmFileName) {
		bool res = textFileLoader.LoadTextFromFile(asmFileName);

		if (!res) {
			std::cout << asmFileName << ": File Not Found";
			return;
		}
		textFileLoader.FilterComments();
		program.Clear();
		if (!assembler.Translate()) {
			std::cout << "Translation failed\n";
		}

	}
	void ExecuteProgram() {
		machine.LoadProgram(0, &program);
		machine.Execute();
	}

	void SetCell(unsigned int address, unsigned int content){
		machine.memory[address] = content;
	}

	unsigned int GetCell(unsigned int address){
		return machine.memory[address];
	}

	bool InitMemoryFromRamFile(const char* ramFile) {
		bool res = ramFileLoader.LoadTextFromFile(ramFile);
		if (!res) {
			std::cout << ramFile << ": File Not Found";
			return false;
		}
		int i = 0;
		int lineNumber = 0;
		int loc;
		int value;
		char buffer[BUFFERLENGTH];


		while (true) {
			ramFileLoader.GetNonEmptyLine(buffer, BUFFERLENGTH, &i, &lineNumber);
			ramFileLoader.ParseTwoIntegersFromLine(buffer, &loc, &value);
			if (loc >= 0 && loc < 256 && value >= 0 && value < 256) {
				SetCell(loc, value);
			}
			if (ramFileLoader.GetTextAt(i) == '\0') {
				break;
			}
		}
		return true;
	}

	void ShowMachine() {
		machine.ShowRegisters();
		machine.ShowMemory();
	}
};

#endif