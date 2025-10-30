#ifndef _REVSYMBOLTABLE_
#define _REVSYMBOLTABLE_

#include <map>
#include <cstring>
#include <string>

class RevSymbolTable
{
private:
	std::map<int, std::string> data;
public:
	void AddKeyValue(int key, std::string value) {
		data[key] = value;
	}

	std::string* Lookup(int key, bool* found) {
		std::map<int, std::string>::iterator it = data.find(key);
		if (it != data.end()) {
			*found = true;
			return &it->second;
		}
		else {
			*found = false;
			return nullptr;
		}
	}

};
#endif
