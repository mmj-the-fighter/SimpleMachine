#include "SimpleMachineFacade.hpp"

int main()
{
	SimpleMachineFacade sm;
	sm.SetCell(200, 1);
	sm.SetCell(201, 5);
	sm.ExecuteProgram("program.asm");
	std::cout << sm.GetCell(202) << std::endl;
	return 0;
}