/*
 * Copyright(c) 2018, Markus Schmidt <schmidt@boomshop.net>
 * All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation; either version 3.0 of the
 * License, or (at your option) any later version.
 *
 * CTK is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public
 * License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with CTK; if not, see <http://www.gnu.org/licenses/>.
 */

#include <string>
#include <fstream>
#include <iostream>
#include <regex>

#include "typedef.hxx"
#include "style.hxx"

using namespace CTK;


const char* ws = " \t\n\r\f\v";
inline std::string& rtrim (std::string& s, const char* t = ws)
{
    return s.erase(s.find_last_not_of(t) + 1);
}
inline std::string& ltrim (std::string& s, const char* t = ws)
{
    return s.erase(0, s.find_first_not_of(t));
}
inline std::string& trim(std::string& s, const char* t = ws)
{
    return ltrim(rtrim(s, t), t);
}

void Style::loadStyle (std::string fname)
{
    std::ifstream file (fname);
    std::stringstream stream;
    stream << file.rdbuf();
    std::string buffer = stream.str();
    std::string selector;
    std::string block;
    std::string property;
    std::string val;
    std::smatch rule_match;
    std::smatch decl_match;
    std::regex rule_ex ("\\s*([a-zA-Z0-9:* \\t]*)\\s*\\{([^\\}]*)\\}", std::regex::ECMAScript);
    std::regex decl_ex ("\\s*([a-zA-Z0-9\\-_]+)\\s*=\\s*([^;]+)\\s*;", std::regex::ECMAScript);
    
    while (regex_search(buffer, rule_match, rule_ex)) {
        selector = rule_match[1];
        block = rule_match[2];
        std::map<std::string, std::string> declarations;
        while (regex_search(block, decl_match, decl_ex)) {
            property = decl_match[1];
            property = trim(property);
            val = decl_match[2];
            val = trim(val);
            
            
            
            block = decl_match.suffix();
        }
        rules[selector].insert(declarations.begin(), declarations.end());
        std::cout << property << " = " << declarations[property] << "\n";
        buffer = rule_match.suffix();
    }
}

std::string Style::parseValue (std::string value)
{
    return "";
}
