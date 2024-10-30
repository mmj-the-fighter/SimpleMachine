CXX = g++
CXXFLAGS = -Wall -Wextra

SOURCES = sm.cpp SimpleMachineFacade.hpp TextFileLoader.hpp SymbolTable.hpp InstructionOpcodeMap.hpp Program.hpp Assembler.hpp Machine.hpp
EXECUTABLE = sm

all: $(EXECUTABLE)

$(EXECUTABLE): $(SOURCES)
	$(CXX) $(CXXFLAGS) -o $(EXECUTABLE) $(SOURCES)

clean:
	rm -f $(EXECUTABLE)

.PHONY: all clean
