#ifndef _PROGRAM_
#define _PROGRAM_
#include <string>
#include "InstructionOpcodeMap.hpp"
#include "SymbolTable.hpp"
#include "CommonDefs.h"
#include "RegisterHelper.hpp"

class Program
{
private:
	unsigned char byteCode[MAXMEMBYTES];
	int currentMarker;
	int loadingOffset;
public:
	Program()
	{
		loadingOffset = 0;
		currentMarker = 0;
		for (int i = 0; i < MAXMEMBYTES; i++) {
			byteCode[i] = 0;
		}
	}

	void Clear(){
		currentMarker = 0;
	}

	inline bool AdvanceMarker(int instrLength){
		if (currentMarker+ instrLength > MAXMEMBYTES - 1){
			return false;
		}
		currentMarker = currentMarker + instrLength;
		return true;
	}

	inline void WriteCode1Byte(unsigned char byte1){
		if (currentMarker >= MAXMEMBYTES - 1){
			return;
		}
		byteCode[currentMarker] = byte1;
		currentMarker += 1;
	}

	inline void WriteCode2Bytes(unsigned char byte1, unsigned char byte2){
		if (currentMarker >= MAXMEMBYTES - 2){
			return;
		}
		byteCode[currentMarker] = byte1;
		byteCode[currentMarker + 1] = byte2;
		currentMarker += 2;
	}

	inline void WriteCode3Bytes(unsigned char byte1, unsigned char byte2, unsigned char byte3){
		if (currentMarker >= MAXMEMBYTES - 3){
			return;
		}
		byteCode[currentMarker] = byte1;
		byteCode[currentMarker + 1] = byte2;
		byteCode[currentMarker + 2] = byte3;
		currentMarker += 3;
	}

	inline void WriteCode3BytesFromOpcodeRegNumPair(unsigned char opcode, RegisterNumPair* r) {
		if (currentMarker >= MAXMEMBYTES - 3) {
			return;
		}
		byteCode[currentMarker] = opcode;
		byteCode[currentMarker + 1] = r->r1;
		byteCode[currentMarker + 2] = r->r2;
		currentMarker += 3;
	}

	inline void WriteCode4Bytes(unsigned char byte1, unsigned char byte2, unsigned char byte3, unsigned char byte4){
		if (currentMarker >= MAXMEMBYTES - 4){
			return;
		}
		byteCode[currentMarker] = byte1;
		byteCode[currentMarker + 1] = byte2;
		byteCode[currentMarker + 2] = byte3;
		byteCode[currentMarker + 3] = byte4;
		currentMarker += 4;
	}

	inline void WriteCode4BytesFromOpcodeRegNumTriplet(unsigned char opcode, RegisterNumTriplet* r) {
		if (currentMarker >= MAXMEMBYTES - 4) {
			return;
		}
		byteCode[currentMarker] = opcode;
		byteCode[currentMarker + 1] = r->r1;
		byteCode[currentMarker + 2] = r->r2;
		byteCode[currentMarker + 3] = r->r3;
		currentMarker += 4;
	}
	
	inline int GetCurrentMarker() {
		return currentMarker;
	}

	inline unsigned char* GetByteCodePointer() {
		return &byteCode[0];
	}

	inline void SetLoadingOffset(int offset) {
		loadingOffset = offset;
	}

	inline int GetLoadingOffset() {
		return loadingOffset;
	}
};

#endif