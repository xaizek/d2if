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

#include "Memory.hpp"

#include <unistd.h>

#include <fstream>
#include <limits>
#include <sstream>
#include <string>
#include <utility>

void Memory::update()
{
    const std::string GRN = "#65A765";
    const std::string RED = "#FF0000";
    const std::string BAR = "#A6F09D";

    const int used = getMemoryUsage();
    const std::string fgColor = (used >= 95) ? RED : GRN;

    std::stringstream result;

    result << "^fg(white)MEM:^p(2;3)"
           << "^fg(" << fgColor << ")"
           << "^r(" << used << "x10)"
           << "^fg(" << BAR << ")"
           << "^r(" << (100 - used) << "x10)";

    Field::setText(result.str());
}

int Memory::getMemoryUsage() const
{
    std::ifstream meminfo("/proc/meminfo");
    if (!meminfo.is_open()) {
        return (-1);
    }

    int total, free, buffers, cached;

    meminfo.ignore(std::numeric_limits<std::streamsize>::max(), ':') >> total;
    meminfo.ignore(std::numeric_limits<std::streamsize>::max(), ':') >> free;
    meminfo.ignore(std::numeric_limits<std::streamsize>::max(), ':') >> buffers;
    meminfo.ignore(std::numeric_limits<std::streamsize>::max(), ':') >> cached;

    meminfo.close();

    return (100 - ((free + buffers + cached)*100)/total);
}
