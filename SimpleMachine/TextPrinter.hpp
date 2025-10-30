#ifndef _TEXTPRINTER_
#define _TEXTPRINTER_

#include <vector>
#include <iostream>
#include <cstring>

//This file is generated using AI, see exception clause at the LICENSE before using and redistributing it.
class TextPrinter
{
private:
    std::vector<char> buffer;

    inline void AppendIntToBuffer(int n) {
        char temp[16];
        char* p = temp + sizeof(temp);
        bool negative = false;

        if (n == 0) {
            buffer.push_back('0');
            return;
        }

        if (n < 0) {
            negative = true;
            unsigned int un = static_cast<unsigned int>(-(n + 1)) + 1;
            while (un > 0) {
                *--p = char('0' + (un % 10));
                un /= 10;
            }
        }
        else {
            unsigned int un = static_cast<unsigned int>(n);
            while (un > 0) {
                *--p = char('0' + (un % 10));
                un /= 10;
            }
        }

        if (negative)
            *--p = '-';

        buffer.insert(buffer.end(), p, temp + sizeof(temp));
    }

public:
    explicit TextPrinter(size_t capacity = 1024) {
        buffer.reserve(capacity);
    }

    inline void AddChar(char c) { buffer.push_back(c); }

    inline void AddString(const std::string& s) {
        buffer.insert(buffer.end(), s.begin(), s.end());
    }

    inline void AddConstCString(const char* cstr) {
        buffer.insert(buffer.end(), cstr, cstr + std::strlen(cstr));
    }

    inline void AddInt(int n) {
        AppendIntToBuffer(n);
    }

    inline void PrintAndFlush() {
        if (!buffer.empty()) {
            std::cout.write(&buffer[0], buffer.size());
            std::cout.flush();
            buffer.clear();
        }
    }

    inline void Clear() {
        if (!buffer.empty()) {
            buffer.clear();
        }
    }
};



#endif