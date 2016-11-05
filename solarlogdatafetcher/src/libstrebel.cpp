/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
#include <cctype>
#include <iostream>
#include <sstream>
#include <string>
#include <algorithm>
#include <vector>
#include "utf8.h"

#include "libstrebel.h"
namespace strebel {

std::string trim(const std::string &s) {
    auto wsfront = std::find_if_not(s.begin(), s.end(), [](int c)
    {
        return std::isspace(c);
    });
    auto wsback = std::find_if_not(s.rbegin(), s.rend(), [](int c)
    {
        return std::isspace(c);
    }).base();
    return (wsback <= wsfront ? std::string() : std::string(wsfront, wsback));
}

void decode_utf8(const std::string& bytes, std::wstring& wstr) {
    utf8::utf8to32(bytes.begin(), bytes.end(), std::back_inserter(wstr));
}

void encode_utf8(const std::wstring& wstr, std::string& bytes) {
    utf8::utf32to8(wstr.begin(), wstr.end(), std::back_inserter(bytes));
}

size_t iso8859_1_to_utf8(char *content, size_t max_size) {
    char *src, *dst;

    //first run to see if there's enough space for the new bytes
    for (src = dst = content; *src; src++, dst++) {
        if (*src & 0x80) {
            // If the high bit is set in the ISO-8859-1 representation, then
            // the UTF-8 representation requires two bytes (one more than usual).
            ++dst;
        }
    }

    if (dst - content + 1 > max_size) {
        // Inform caller of the space required
        return dst - content + 1;
    }

    while (dst > src) {
        if (*src & 0x80) {
            *dst-- = 0x80 | (*src & 0x3f); // trailing byte
            *dst-- = 0xc0 | (*((unsigned char *) src--) >> 6); // leading byte
        } else {
            *dst-- = *src--;
        }
    }
    return 0; // SUCCESS
}

void removeCharsFromString(std::string &str, char* charsToRemove) {
    for (unsigned int i = 0; i < strlen(charsToRemove); ++i) {
        str.erase(remove(str.begin(), str.end(), charsToRemove[i]), str.end());
    }
}

std::vector<std::string> splitString(std::string &str, char &token) {
    std::istringstream split(str);
    std::vector<std::string> tokens;
    for (std::string each; std::getline(split, each, token); tokens.push_back(each));
    return tokens;
}
}
