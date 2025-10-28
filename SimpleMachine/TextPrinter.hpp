#ifndef _TEXTPRINTER_
#define _TEXTPRINTER_

#include <vector>
#include <string>
#include <iostream>
#include <sstream>

class TextPrinter
{
private:
	std::vector<char> buffer;
	std::ostringstream ss;
public:
	TextPrinter(int capacity = 1024) {
		buffer.reserve(capacity);
	}

	inline void AddChar(char c) {
		buffer.push_back(c);
	}

	inline void AddString(const std::string& s) {
		buffer.insert(buffer.end(), s.begin(), s.end());
	}

	inline void AddConstCString(const char* cstr) {
		while (*cstr != '\0') {
			buffer.push_back(*cstr);
			++cstr;
		}
	}

	inline void AddInt(int n) {
		ss.str("");
		ss.clear();
		ss << n;
		AddString(ss.str());
	}

	inline void PrintAndFlush() {
		std::cout.write(&buffer[0], buffer.size());
		std::cout.flush();
	}
};



#endif