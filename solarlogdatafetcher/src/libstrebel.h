/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   libstrebel.h
 * Author: pick
 *
 * Created on 31. Juli 2016, 10:05
 */

#ifndef LIBSTREBEL_H
#define LIBSTREBEL_H

namespace strebel {

std::string trim(const std::string &s);
void decode_utf8(const std::string& bytes, std::wstring& wstr);
void encode_utf8(const std::wstring& wstr, std::string& bytes);
size_t iso8859_1_to_utf8(char *content, size_t max_size);
void removeCharsFromString(std::string &str, char* charsToRemove);
std::vector<std::string> splitString(std::string &str, char &token);

}
#endif /* LIBSTREBEL_H */

