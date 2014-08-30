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

#ifndef __FIELD_HPP__
#define __FIELD_HPP__

#include <iosfwd>
#include <memory>
#include <string>

class ColorScheme;

class Field
{
public:
    Field(const ColorScheme& colorScheme);
    virtual ~Field();

    // These operations are forbidden.
    Field(Field &rhs) = delete;
    Field & operator=(Field &rhs) = delete;

    virtual void update() = 0;
    const std::string & getText() const;
    bool isVisible() const;

protected:
    void setText(const std::string &newText);
    void setVisible(bool newVisible);
    std::string getColor(const std::string &element) const;

private:
    std::string text;
    bool visible = true;
    const ColorScheme& colorScheme;
};

std::ostream & operator<<(std::ostream &os, const Field *field);

#endif // __FIELD_HPP__
