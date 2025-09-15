#ifndef _TEXTFILELOADER_
#define _TEXTFILELOADER_

#include <cstdlib>
#include <string>
#include <fstream>
#include <iostream>
#include <algorithm>

class TextFileLoader
{
private:
	char *buffer;
	int bufferLength;
public:
	TextFileLoader()
	{
		buffer = NULL;
		bufferLength = 0;
	}

	char GetTextAt(int i){
		if (i < 0 || i >= bufferLength){
			return '\0';
		}
		return buffer[i];
	}

	TextFileLoader(const char* filename)
	{
		buffer = NULL;
		bufferLength = 0;
		LoadTextFromFile(filename);
	}
	~TextFileLoader() {
		DestroyBuffer();
	}

	void DestroyBuffer() {
		if (NULL != buffer) {
			delete[] buffer;
			buffer = NULL;
			bufferLength = 0;
		}
	}

	bool LoadTextFromFile(std::string filename) {
		if (filename.compare("") == 0) {
			return false;
		}
		std::ifstream in(filename);
		if (!in.is_open()) {
			return false;
		}
		std::string str((std::istreambuf_iterator<char>(in)),
			std::istreambuf_iterator<char>());
		DestroyBuffer();
		bufferLength = str.size();
		buffer = new char[bufferLength + 1];
		const char* pc = str.c_str();
		int i = 0;

		bufferLength = str.size();
		buffer = new char[bufferLength + 1];
		std::transform(str.begin(), str.end(), buffer,
			[](unsigned char c) { return std::tolower(c); });
		buffer[bufferLength] = '\0';

		std::cout << buffer << std::endl;
		return true;
	}
};


#endif