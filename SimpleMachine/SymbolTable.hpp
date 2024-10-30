#ifndef _SYMBOLTABLE_
#define _SYMBOLTABLE_

#include <map>
#include <cstring>
#include <string>

class SymbolTable
{

private:
	std::map<std::string, int> data;
	int unknown;
public:
	SymbolTable(){
		unknown = -1;
	}

	void AddLabel(std::string label, int address){
		data[label] = address;
	}

	unsigned char Lookup(std::string str){
		int addr = 0;
		std::string keyToFind(str);
		std::map<std::string, int>::iterator it = data.find(keyToFind);
		if (it != data.end()) {
			addr = it->second;
			
		}
		return addr;
	}

};
#endif
