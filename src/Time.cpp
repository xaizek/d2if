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

#include "Time.hpp"

#include <chrono>
#include <sstream>
#include <string>

std::string Time::FORMAT { "%Y.%m.%d %H:%M:%S" };

void Time::update()
{
    const std::time_t rawTime { std::time(nullptr) };
    const std::tm *timeInfo { std::localtime(&rawTime) };

    std::stringstream result;

    char buffer[80];
    if (std::strftime(buffer, sizeof(buffer), FORMAT.c_str(), timeInfo) > 0) {
        result << "^fg(white)" << buffer;
    }

    Time::setText(result.str());
}

// vim: set filetype=cpp.cpp11 :
