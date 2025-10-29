#ifndef _SYMBOLTABLE_
#define _SYMBOLTABLE_

#include <map>
#include <cstring>
#include <string>

class SymbolTable
{
private:
	std::map<std::string, int> data;

public:
	inline void AddLabel(std::string label, int address){
		data[label] = address;
	}

	inline unsigned char Lookup(std::string str, bool* found){
		int addr = 0;
		std::string keyToFind(str);
		std::map<std::string, int>::iterator it = data.find(keyToFind);
		if (it != data.end()) {
			addr = it->second;
			*found = true;
		}
		else {
			*found = false;
		}
		return (unsigned char)addr;
	}
};
#endif
