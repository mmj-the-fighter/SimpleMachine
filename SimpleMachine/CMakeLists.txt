cmake_minimum_required(VERSION 3.10)

project(SimpleMachine LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCES
    sm.cpp
)

set(HEADERS
    SimpleMachineFacade.hpp
    TextFileLoader.hpp
	RamFileLoader.hpp
    SymbolTable.hpp
    InstructionOpcodeMap.hpp
    Program.hpp
    Assembler.hpp
    Machine.hpp
	Disassembler.hpp
    OpcodeInstructionMap.hpp
	RevSymbolTable.hpp
)

add_executable(sm ${SOURCES} ${HEADERS})
