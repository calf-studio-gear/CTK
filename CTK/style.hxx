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

#ifndef CTK_STYLE_HXX
#define CTK_STYLE_HXX

#include <string>
#include <map>

namespace CTK {

class Style
{
protected:
    std::map<std::string, std::map<std::string, std::string>> rules;
    
    void loadStyle (std::string fname);
    std::string parseValue (std::string value);
};

}; // namespace CTK

#endif // CTK_STYLE_HXX
