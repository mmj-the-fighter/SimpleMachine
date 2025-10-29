#include "Profiler.hpp"
#include "SimpleMachineFacade.hpp"

static void string_copy(char* dst, const char* src, int dstBufSize) {
	int i = 0;
	while (i < dstBufSize-1 && (*dst = *src) != '\0') {
		++i;
		++src;
		++dst;
	}
	*dst = '\0';
}

void Run(char* asmFile, char* ramFile) {
	char c;
	SimpleMachineFacade sm;
	sm.InitMemoryFromRamFile(ramFile);
	if (!sm.TranslateAssembly(asmFile)) {
		return;
	}
	std::cout << "Translation: success\n";
	
	if (!sm.InterpretProgram()) {
		return;
	}
	std::cout << "Interpretation: success\n";

	if (!sm.Disassemble()) {
		return;
	}
	std::cout << "Disassembly: success\n";

	std::cout << "Do you want to see machine info? (y/n): ";
	std::cin >> c;
	if(c=='y' || c== 'Y'){
		std::cout << "\n";
		sm.ShowMachine();
		std::cout << "\n";
	}
}

int main(int argc, char* argv[])
{
	char asmFile[256];
	char ramFile[256];
	if (argc != 3) {
		string_copy(asmFile, "program.asm", 256);
		string_copy(ramFile, "mem.ram", 256);
	}
	else {
		string_copy(asmFile, argv[1], 256);
		string_copy(ramFile, argv[2], 256);
	}
	Run(asmFile, ramFile);
	util::Profiler::GetInstance().Print();
	return 0;
}