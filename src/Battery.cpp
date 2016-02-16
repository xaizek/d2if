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

#include "Battery.hpp"

#include <fstream>
#include <limits>
#include <sstream>
#include <utility>

void Battery::update()
{
    static const std::string fgBarColor {
        "^fg(" + getColor("fg-bar") + ")"
    };
    static const std::string fgGoodColor {
        "^fg(" + getColor("fg-bar-good") + ")"
    };
    static const std::string fgBadColor {
        "^fg(" + getColor("fg-bar-bad") + ")"
    };

    const std::pair<bool, int> state = getBatteryState();
    const bool charging = state.first;
    const int remaining = state.second;

    if (remaining < 0) {
        setVisible(false);
        return;
    }

    const std::string &fgColor = charging ? fgGoodColor : fgBadColor;

    std::ostringstream result;

    result << "^fg(white)BAT:"
           << "^p(2;3)"
           << fgBarColor
           << "^r(" << remaining << "x10)"
           << fgColor
           << "^r(" << (100 - remaining) << "x10)";

    Field::setText(result.str());
}

std::pair<bool, int> Battery::getBatteryState() const
{
    std::ifstream info { "/proc/acpi/battery/BAT0/info" };
    std::ifstream state { "/proc/acpi/battery/BAT0/state" };

    if (info.is_open() && state.is_open()) {
        static const auto max = std::numeric_limits<std::streamsize>::max();

        int full;
        std::string charged;
        int remaining;

        info.ignore(max, '\n')
            .ignore(max, '\n')
            .ignore(max, ':')
            >> full;

        state.ignore(max, '\n')
             .ignore(max, '\n')
             .ignore(max, ':')
             >> charged;

        state.ignore(max, ':')
            .ignore(max, ':')
            >> remaining;

        return {
            charged == "charged" || charged == "charging", (remaining*100)/full
        };
    } else {
        return { false, -1 };
    }
}
