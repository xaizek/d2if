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

#ifndef __MEMORY_HPP__
#define __MEMORY_HPP__

#include "Field.hpp"

class Memory : public Field
{
public:
    Memory() = default;

    // These operations are forbidden.
    Memory(Memory &&rhs) = delete;
    Memory & operator=(Memory &&rhs) = delete;

    virtual void update() override;

private:
    int getMemoryUsage() const;
};

#endif // __MEMORY_HPP__

// vim: set filetype=cpp.cpp11 :
