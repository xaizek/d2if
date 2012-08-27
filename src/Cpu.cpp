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

#include "Cpu.hpp"

#include <fstream>
#include <functional>
#include <limits>
#include <sstream>
#include <string>
#include <iostream>

Cpu::Cpu()
{
    last = getCpuUsage();
}

void Cpu::update()
{
    const std::string GRN = "#65A765";
    const std::string RED = "#FF0000";
    const std::string BAR = "#A6F09D";

    const cpuInfo current = getCpuUsage();
    const long totalDiff = std::max(current.first - last.first, 1l);
    const long idleDiff = std::max(current.second - last.second, 1l);
    const long used = 100 - (idleDiff*100)/totalDiff;

    const std::string fgColor = (used >= 95) ? RED : GRN;

    std::stringstream result;

    result << "^fg(white)CPU:"
           << "^p(2;3)"
           << "^fg(" << fgColor << ")"
           << "^r(" << used << "x10)"
           << "^fg(" << BAR << ")"
           << "^r(" << (100 - used) << "x10)";

    Field::setText(result.str());

    last = current;
}

Cpu::cpuInfo Cpu::getCpuUsage() const
{
    std::ifstream stat("/proc/stat");
    if (!stat.is_open()) {
        return {-1, -1};
    }

    long user, nice, system, idle;

    stat.ignore(std::numeric_limits<std::streamsize>::max(), ' ');
    stat >> user >> nice >> system >> idle;

    stat.close();

    return {user + nice + system + idle, idle};
}
