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

#ifndef __TIME_HPP__
#define __TIME_HPP__

#include <string>

#include "Field.hpp"

class Time : public Field
{
public:
    Time() = default;

    // These operations are forbidden.
    Time(Time &&rhs) = delete;
    Time & operator=(Time &&rhs) = delete;

    virtual std::string getText() const override;

private:
    static std::string FORMAT;
};

#endif // __TIME_HPP__