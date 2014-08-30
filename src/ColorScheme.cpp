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

#include "ColorScheme.hpp"

#include <map>
#include <stdexcept>
#include <string>
#include <utility>

namespace
{
    const char SEPARATOR = '-';
}

ColorScheme::ColorScheme(colors_t &&colors)
    :colors(std::move(colors))
{
}

std::string ColorScheme::getColor(const std::string &element) const
{
    const colors_t::const_iterator &color = findColor(element);
    if (color == colors.cend()) {
        throw std::invalid_argument {
            "Wrong color scheme element name: " + element + '.'
        };
    }
    return color->second;
}

ColorScheme::colors_t::const_iterator ColorScheme::findColor(
    const std::string &element) const
{
    colors_t::const_iterator color { colors.find(element) };

    if (color == colors.cend()) {
        std::string::size_type sepPos { element.rfind(SEPARATOR) };
        if (sepPos != std::string::npos) {
            std::string subElement { element, 0U, sepPos };
            while (sepPos != std::string::npos &&
                   (color = colors.find(subElement)) == colors.end()) {
                sepPos = subElement.rfind(SEPARATOR);
                subElement.erase(sepPos);
            }
        }
    }

    return color;
}

// vim: set filetype=cpp.cpp11 :
