// d2if
// Copyright (C) 2012-2013 xaizek.
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA

#ifndef D2IF__COLORSCHEME_HPP__
#define D2IF__COLORSCHEME_HPP__

#include <map>
#include <string>

class ColorScheme
{
    typedef std::map<std::string, std::string> colors_t;

public:
    ColorScheme(colors_t &&colors);

    // These operations are forbidden.
    ColorScheme(ColorScheme &rhs) = delete;
    ColorScheme & operator=(ColorScheme &rhs) = delete;

    std::string getColor(const std::string &element) const;

private:
    colors_t::const_iterator findColor(const std::string &element) const;

private:
    colors_t colors;
};

#endif // D2IF__COLORSCHEME_HPP__
