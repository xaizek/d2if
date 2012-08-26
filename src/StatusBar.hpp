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

#ifndef __STATUSBAR_HPP__
#define __STATUSBAR_HPP__

#include <string>
#include <memory>
#include <vector>

#include "Field.hpp"

class StatusBar
{
public:
    StatusBar() = default;

    // These operations are forbidden.
    StatusBar(StatusBar &&rhs) = delete;
    StatusBar & operator=(StatusBar &&rhs) = delete;

    void addField(std::shared_ptr<Field> field);

    void setFieldDelimiter(const std::string &delimiter);

    std::string getText() const;

private:
    std::vector<std::shared_ptr<Field>> fields;
    std::string fieldDelimiter = " | ";
};

#endif // __STATUSBAR_HPP__