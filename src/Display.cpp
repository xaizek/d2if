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

#include "Display.hpp"

#include <fstream>
#include <sstream>
#include <string>

void Display::update()
{
    const std::string BAR = "#A6F09D";

    const int actual = getDisplayBrightness();

    std::stringstream result;

    result << "^fg(white)DB: "
           << "^fg(" << BAR << ")"
           << actual << "%";

    Field::setText(result.str());
}

int Display::getDisplayBrightness() const
{
    int maxValue;
    std::ifstream max("/sys/class/backlight/acpi_video0/max_brightness");
    max >> maxValue;
    max.close();

    int actualValue;
    std::ifstream actual("/sys/class/backlight/acpi_video0/actual_brightness");
    actual >> actualValue;
    actual.close();

    return ((100*actualValue)/maxValue);
}

// vim: set filetype=cpp.cpp11 :
