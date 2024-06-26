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

#include <cassert>
#include <cstdint>

#include <fstream>
#include <limits>
#include <sstream>
#include <string>
#include <utility>

void Memory::update()
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

    const int used { getMemoryUsage() };
    const std::string &fgColor { (used >= 95) ? fgBadColor : fgGoodColor };

    std::ostringstream result;

    result << "MEM:"
           << "^p(2;3)"
           << fgColor
           << "^r(" << used << "x10)"
           << fgBarColor
           << "^r(" << (100 - used) << "x10)";

    Field::setText(result.str());
}

template <std::size_t N>
bool startsWith(const std::string &str, const char (&prefix)[N])
{
    return str.compare(0, N - 1, prefix) == 0;
}

template <std::size_t N>
bool extractUint64(const std::string &str, const char (&prefix)[N],
                   std::uint64_t &value)
{
    if (!startsWith(str, prefix)) {
        return false;
    }

    value = std::strtoull(str.c_str() + N, nullptr, 10);
    return true;
}

int Memory::getMemoryUsage() const
{
    // Mimicking behaviour of `free` tool from `procps`, see `man free`.

    std::ifstream meminfo("/proc/meminfo");
    if (!meminfo.is_open()) {
        return (-1);
    }

    std::uint64_t total = 0, free = 0, buffers = 0, cached = 0;
    for (std::string buf(64, '\0'); meminfo.getline(&buf[0], buf.size()); ) {
        std::uint64_t value;
        if (extractUint64(buf, "MemTotal:", value)) {
            total = value;
        } else if (extractUint64(buf, "MemFree:", value)) {
            free = value;
        } else if (extractUint64(buf, "Buffers:", value)) {
            buffers = value;
        } else if (extractUint64(buf, "Cached:", value)) {
            cached += value;
        } else if (extractUint64(buf, "SReclaimable:", value)) {
            cached += value;
        }
    }

    const std::uint64_t usage {
        100 - ((free + buffers + cached)*100)/total
    };

    assert(usage >= 0 && "Usage can't be negative.");
    assert(usage <= 100 && "Usage can't be greater than 100%.");

    return usage;
}
