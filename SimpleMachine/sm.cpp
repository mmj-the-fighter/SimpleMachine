#include "SimpleMachineFacade.hpp"

//int main1()
//{
//	SimpleMachineFacade sm;
//	sm.SetCell(200, 1);
//	sm.SetCell(201, 5);
//	sm.ExecuteProgram("program.asm");
//	std::cout << sm.GetCell(202) << std::endl;
//	return 0;
//}
//
//int main()
//{
//	SimpleMachineFacade sm;
//	sm.SetCell(199, 5);
//	sm.SetCell(200, 1);
//	sm.SetCell(201, 2);
//	sm.SetCell(202, 3);
//	sm.SetCell(203, 4);
//	sm.SetCell(204, 5);
//	sm.ExecuteProgram("program.asm");
//	std::cout << "\n" << "Content of Memory Cell #255: "<< sm.GetCell(255) << std::endl;
//	return 0;
//}

static void string_copy(char* dst, const char* src) {
	while ((*dst = *src) != '\0') {
		++src;
		++dst;
	}
}

int main(int argc, char* argv[])
{
	char asmFile[256];
	char ramFile[256];
	if (argc != 3) {
		string_copy(asmFile, "program.asm");
		string_copy(ramFile, "mem.ram");
	}
	else {
		string_copy(asmFile, argv[1]);
		string_copy(ramFile, argv[2]);
	}
	SimpleMachineFacade sm;
	sm.InitMemoryFromRamFile(ramFile);
	sm.TranslateAssembly(asmFile);
	sm.ExecuteProgram();
	sm.ShowMachine();
	return 0;
}