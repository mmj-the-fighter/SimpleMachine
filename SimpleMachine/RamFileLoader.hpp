#ifndef _RAMFILELOADER_
#define _RAMFILELOADER_

#include <cstdlib>
#include <string>
#include <cstring>
#include <fstream>
#include <iostream>
#include <algorithm>

class RamFileLoader
{
private:
	char* buffer;
	int bufferLength;
public:
	RamFileLoader()
	{
		buffer = NULL;
		bufferLength = 0;
	}

	char GetTextAt(int i) {
		if (i < 0 || i >= bufferLength) {
			return '\0';
		}
		return buffer[i];
	}

	RamFileLoader(const char* filename)
	{
		buffer = NULL;
		bufferLength = 0;
		LoadTextFromFile(filename);
	}
	~RamFileLoader() {
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
		std::ifstream in(filename.c_str());
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
		std::transform(str.begin(), str.end(), buffer, ::tolower);
		buffer[bufferLength] = '\0';

		//std::cout << buffer << std::endl;
		return true;
	}

	bool GetNonEmptyLine(char* buf, int bufLen, int* ssm, int* lineNumber) {
		int i = *ssm;
		int k;
		char c;
		int validCharCount;
		do
		{
			validCharCount = 0;
			k = 0;
			while ((c = GetTextAt(i)) != '\0')
			{
				if (k >= bufLen - 1) {
					return false;
				}
				if (c == '\n') {
					++*lineNumber;
					++i;
					break;
				}
				if (isalnum(c)) {
					++validCharCount;
				}
				buf[k] = c;
				++k;
				++i;
			}
		} while (c != '\0' && validCharCount == 0);
		buf[k] = '\0';
		*ssm = i;
		return c != '\0';
	}

	bool ParseTwoIntegersFromLine(char* line, int* i1, int* i2)
	{
#define MAXDIGITS 32
		char i1Buf[MAXDIGITS];
		char i2Buf[MAXDIGITS];
		int lineLength = strlen(line);
		int k = 0;
		//skip whitespace
		while (k < lineLength && (line[k] == ' ' || line[k] == '\t')) {
			++k;
		}
		if (k >= lineLength) {
			return false;
		}

		//read first buffer
		int index = 0;
		char c;
		while (index < MAXDIGITS && k < lineLength) {
			c = line[k];
			if (c == ' ' || c == '\t' || c == '\0') {
				break;
			}
			if (!isdigit(c)) {
				return false;
			}
			i1Buf[index] = c;
			++index;
			++k;
		}
		i1Buf[index] = '\0';
		if (c == '\0') {
			return false;
		}

		//skip whitespace
		while (k < lineLength && (line[k] == ' ' || line[k] == '\t')) {
			++k;
		}
		if (k >= lineLength) {
			return false;
		}

		//read second buffer
		index = 0;
		while (index < MAXDIGITS && k < lineLength) {
			c = line[k];
			if (c == ' ' || c == '\t' || c == '\0') {
				break;
			}
			if (!isdigit(c)) {
				return false;
			}
			i2Buf[index] = c;
			++index;
			++k;
		}
		i2Buf[index] = '\0';
		*i1 = atoi(i1Buf);
		*i2 = atoi(i2Buf);
		return true;
#undef MAXDIGITS
	}

};


#endif