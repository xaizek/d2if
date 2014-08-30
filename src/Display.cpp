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
    const int actual { getDisplayBrightness() };
    const bool visible { actual >= 0 };
    Field::setVisible(visible);
    if (visible) {
        std::ostringstream result;

        result << "DB: "
               << actual << "%";

        Field::setText(result.str());
    }
}

int Display::getDisplayBrightness() const
{
    std::ifstream max {
        "/sys/class/backlight/acpi_video0/max_brightness"
    };
    std::ifstream actual {
        "/sys/class/backlight/acpi_video0/actual_brightness"
    };
    if (max.is_open() && actual.is_open()) {
        int maxValue;
        max >> maxValue;

        int actualValue;
        actual >> actualValue;

        return ((100*actualValue)/maxValue);
    } else {
        return -1;
    }
}
