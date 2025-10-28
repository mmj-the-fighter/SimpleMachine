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
		std::ifstream in(filename.c_str());
		if (!in.is_open()) {
			return false;
		}
		std::string str((std::istreambuf_iterator<char>(in)),
			std::istreambuf_iterator<char>());
		DestroyBuffer();

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

	void FilterComments() {
		int i = 0;
		int j = 0;
		bool comment = false;
		char c;

		while ((c = buffer[i]) != '\0') {
			if (c == '#') {
				comment = true;
			}
			else if (c == '\n' && comment) {
				comment = false;
			}
			buffer[j] = buffer[i];
			if (!comment) {
				++j;
			}
			++i;
		}
		buffer[j] = '\0';
	}
};


#endif