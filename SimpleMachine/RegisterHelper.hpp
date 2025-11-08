#ifndef _REGISTERHELPER_
#define _REGISTERHELPER_
#include "CommonDefs.h"

struct RegisterPair {
	char r1;
	char r2;
};

struct RegisterTriplet {
	char r1;
	char r2;
	char r3;
};

struct RegisterNumPair {
	unsigned char r1;
	unsigned char r2;
};

struct RegisterNumTriplet {
	unsigned char r1;
	unsigned char r2;
	unsigned char r3;
};


class RegisterHelper
{
public:
	inline unsigned char FindRegisterNumber(char r) {
		int offset = r - 'A';
		if (offset < 0 || offset>15) {
			return 0xFF;
		}
		return static_cast<unsigned char>(offset);
	}

	inline bool FindRegisterNumberPair(char buf[][BUFFERLENGTH], RegisterNumPair* regNumPair) {
		int offset1 = buf[0][0] - 'A';
		int offset2 = buf[1][0] - 'A';
		if (offset1 < 0 || offset1 >15 || offset2 < 0 || offset2 >15) {
			return false;
		}
		regNumPair->r1 = static_cast<unsigned char>(offset1);
		regNumPair->r2 = static_cast<unsigned char>(offset2);
		return true;
	}

	inline bool FindRegisterNumberTriplet(char buf[][BUFFERLENGTH], RegisterNumTriplet* regNumTriplet) {
		int offset1 = buf[0][0] - 'A';
		int offset2 = buf[1][0] - 'A';
		int offset3 = buf[2][0] - 'A';
		if (offset1 < 0 || offset1 >15 || offset2 < 0 || offset2 >15 || offset3 < 0 || offset3 >15) {
			return false;
		}
		regNumTriplet->r1 = static_cast<unsigned char>(offset1);
		regNumTriplet->r2 = static_cast<unsigned char>(offset2);
		regNumTriplet->r3 = static_cast<unsigned char>(offset3);
		return true;
	}


	inline char FindRegisterChar(unsigned char n) {
		if (n > 15) {
			return '\0';
		}
		return static_cast<char>('A' + n);
	}

	inline bool FindRegisterChar2(unsigned char n, char* r) {
		if (n > 15) {
			return false;
		}
		*r = static_cast<char>('A' + n);
		return true;
	}

	inline bool FindRegisterPair(unsigned char n1, unsigned char n2, RegisterPair *regPair) {
		if (n1 > 15 || n2 > 15) {
			return false;
		}
		regPair->r1 = static_cast<char>('A' + n1);
		regPair->r2 = static_cast<char>('A' + n2);
		return true;
	}

	inline bool FindRegisterTriplet(unsigned char n1, unsigned char n2, unsigned char n3, RegisterTriplet* regTriplet) {
		if (n1 > 15 || n2 > 15 || n3 > 15) {
			return false;
		}
		regTriplet->r1 = static_cast<char>('A' + n1);
		regTriplet->r2 = static_cast<char>('A' + n2);
		regTriplet->r3 = static_cast<char>('A' + n3);
		return true;
	}



};
#endif
