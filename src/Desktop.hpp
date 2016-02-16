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

#ifndef D2IF__DESKTOP_HPP__
#define D2IF__DESKTOP_HPP__

#include <memory>

#include "Field.hpp"

class ColorScheme;

class Desktop : public Field
{
public:
    Desktop(const ColorScheme& colorScheme);
    ~Desktop();

public:
    virtual void update() override;

private:
    bool tryInit();

private:
    std::unique_ptr<struct Data> data;
};

#endif // D2IF__DESKTOP_HPP__
