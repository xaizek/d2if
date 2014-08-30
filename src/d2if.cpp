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

#include <cstdlib>

#include <chrono>
#include <iostream>

#include "Battery.hpp"
#include "ColorScheme.hpp"
#include "Cpu.hpp"
#include "Display.hpp"
#include "Layout.hpp"
#include "Memory.hpp"
#include "Network.hpp"
#include "StatusBar.hpp"
#include "Time.hpp"
#include "Timer.hpp"
#include "Volume.hpp"

int main(void)
{
    const ColorScheme colorScheme = {
        {
            { "bg",          "black"   },
            { "fg",          "white"   },
            { "fg-bad",      "black"   },
            { "fg-bar",      "#A6F09D" },
            { "fg-bar-good", "#65A765" },
            { "fg-bar-bad",  "#FF0000" },
        }
    };

    StatusBar statusBar = {
        std::make_shared<Time>(colorScheme),
        std::make_shared<Layout>(colorScheme),
        std::make_shared<Memory>(colorScheme),
        std::make_shared<Cpu>(colorScheme),
        std::make_shared<Display>(colorScheme),
        std::make_shared<Volume>(colorScheme, "hw:1"),
        std::make_shared<Battery>(colorScheme),
        std::make_shared<Network>(colorScheme),
    };

    statusBar.setFieldDelimiter(" | ");
    statusBar.setFieldDelimiterColor(colorScheme.getColor("fg"));

    Timer timer {
        [&]() {
            std::cout << statusBar.getText() << std::endl;
        }
    };

    timer.run(std::chrono::seconds(1));

    return EXIT_SUCCESS;
}

// vim: set filetype=cpp.cpp11 :
