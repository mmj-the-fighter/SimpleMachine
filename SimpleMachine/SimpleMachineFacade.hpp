#ifndef _SIMPLEMACHINEFACADE_
#define _SIMPLEMACHINEFACADE_

#include "TextFileLoader.hpp"
#include "RamFileLoader.hpp"
#include "InstructionOpcodeMap.hpp"
#include "Program.hpp"
#include "Machine.hpp"
#include "Assembler.hpp"
#include "Disassembler.hpp"
#include "Profiler.hpp"

class SimpleMachineFacade
{
	TextFileLoader textFileLoader;
	RamFileLoader ramFileLoader;
	Machine machine;
	Program program;
	Assembler assembler;
	Disassembler disassembler;
	unsigned char loadingOffset;

public:
	SimpleMachineFacade(){
		assembler.Set(&textFileLoader, &program);
		loadingOffset = 0;
	}

	bool TranslateAssembly(const char* asmFileName) {
		util::ProfilerScope prof(1000);
		bool res = textFileLoader.LoadTextFromFile(asmFileName);

		if (!res) {
			std::cout << asmFileName << ": File Not Found";
			return false;
		}
		textFileLoader.FilterComments();
		program.Clear();
		if (!assembler.Translate()) {
			std::cout << "Translation failed\n";
			return false;
		}
		return true;
	}
	bool InterpretProgram() {
		util::ProfilerScope prof(2000);
		loadingOffset = 8;
		machine.LoadProgram(loadingOffset, &program);
		return machine.InterpretProgram();
	}

	bool Disassemble() {
		util::ProfilerScope prof(3000);
		disassembler.Set(&machine, loadingOffset);
		return disassembler.Translate();
	}

	void SetCell(unsigned char address, unsigned char content){
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
				SetCell((unsigned char)loc, (unsigned char)value);
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