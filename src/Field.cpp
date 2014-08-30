// d2if
// Copyright (C) 2012 xaizek.
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

#include "Field.hpp"

#include <memory>
#include <ostream>
#include <string>

#include "ColorScheme.hpp"

Field::Field(const ColorScheme& colorScheme)
    : colorScheme(colorScheme)
{
}

Field::~Field()
{
}

const std::string & Field::getText() const
{
    return text;
}

bool Field::isVisible() const
{
    return visible;
}

void Field::setText(const std::string &newText)
{
    text = newText;
}

void Field::setVisible(bool newVisible)
{
    visible = newVisible;
}

std::string Field::getColor(const std::string &element) const
{
    return colorScheme.getColor(element);
}

std::ostream & operator<<(std::ostream &os, const Field *field)
{
    return (os << field->getText());
}

// vim: set filetype=cpp.cpp11 :
