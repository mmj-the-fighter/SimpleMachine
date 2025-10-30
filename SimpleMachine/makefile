CXX = g++
CXXFLAGS = -Wall -Wextra

SOURCES = sm.cpp SimpleMachineFacade.hpp TextFileLoader.hpp RamFileLoader.hpp SymbolTable.hpp InstructionOpcodeMap.hpp Program.hpp Assembler.hpp Machine.hpp Disassembler.hpp RevSymbolTable.hpp Profiler.hpp RegisterHelper.hpp TextPrinter.hpp
EXECUTABLE = sm

all: $(EXECUTABLE)

$(EXECUTABLE): $(SOURCES)
	$(CXX) $(CXXFLAGS) -o $(EXECUTABLE) $(SOURCES)

clean:
	rm -f $(EXECUTABLE)

.PHONY: all clean
