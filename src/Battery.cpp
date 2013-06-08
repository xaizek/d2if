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
    const std::string GRN = "#65A765";
    const std::string RED = "#FF0000";
    const std::string BAR = "#A6F09D";

    const std::pair<bool, int> state = getBatteryState();
    const bool charging = state.first;
    const int remaining = state.second;

    const std::string fgColor = charging ? GRN : RED;

    std::ostringstream result;

    result << "^fg(white)BAT:"
           << "^p(2;3)"
           << "^fg(" << BAR << ")"
           << "^r(" << remaining << "x10)"
           << "^fg(" << fgColor << ")"
           << "^r(" << (100 - remaining) << "x10)";

    Field::setText(result.str());
}

std::pair<bool, int> Battery::getBatteryState() const
{
    std::ifstream info("/proc/acpi/battery/BAT0/info");

    info.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    info.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    info.ignore(std::numeric_limits<std::streamsize>::max(), ':');

    int full;
    info >> full;

    info.close();

    std::ifstream state("/proc/acpi/battery/BAT0/state");

    state.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    state.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    state.ignore(std::numeric_limits<std::streamsize>::max(), ':');

    std::string charged;
    state >> charged;

    state.ignore(std::numeric_limits<std::streamsize>::max(), ':');
    state.ignore(std::numeric_limits<std::streamsize>::max(), ':');

    int remaining;
    state >> remaining;

    state.close();

    return {
        charged == "charged" || charged == "charging", (remaining*100)/full
    };
}

// vim: set filetype=cpp.cpp11 :
