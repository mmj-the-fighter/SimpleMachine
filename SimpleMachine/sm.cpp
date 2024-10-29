#include "SimpleMachineFacade.hpp"

int main1()
{
	SimpleMachineFacade sm;
	sm.SetCell(200, 1);
	sm.SetCell(201, 5);
	sm.ExecuteProgram("program.asm");
	std::cout << sm.GetCell(202) << std::endl;
	return 0;
}

int main()
{
	SimpleMachineFacade sm;
	sm.SetCell(199, 5);
	sm.SetCell(200, 1);
	sm.SetCell(201, 2);
	sm.SetCell(202, 3);
	sm.SetCell(203, 4);
	sm.SetCell(204, 5);
	sm.ExecuteProgram("program.asm");
	std::cout << sm.GetCell(255) << std::endl;
	return 0;
}