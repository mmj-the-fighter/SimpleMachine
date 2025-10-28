#ifndef _REGISTERHELPER_
#define _REGISTERHELPER_

struct RegisterPair {
	char r1;
	char r2;
};

struct RegisterTriplet {
	char r1;
	char r2;
	char r3;
};


class RegisterHelper
{
public:
	inline unsigned char FindRegisterNumber(char r) {
		int offset = r - 'a';
		unsigned char v = 0xFF;
		if (offset < 0 || offset>15) {
			return 0xFF;
		}
		return offset;
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
