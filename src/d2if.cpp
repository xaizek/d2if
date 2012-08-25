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

#include <chrono>
#include <iostream>

#include "Battery.hpp"
#include "Cpu.hpp"
#include "Display.hpp"
#include "Memory.hpp"
#include "Network.hpp"
#include "StatusBar.hpp"
#include "Time.hpp"
#include "Timer.hpp"
#include "Volume.hpp"

int main(void)
{
    StatusBar statusBar;
    statusBar.addField(std::make_shared<Time>());
    statusBar.addField(std::make_shared<Memory>());
    statusBar.addField(std::make_shared<Cpu>());
    statusBar.addField(std::make_shared<Display>());
    statusBar.addField(std::make_shared<Volume>());
    statusBar.addField(std::make_shared<Battery>());
    statusBar.addField(std::make_shared<Network>());

    Timer timer {
        [&]() {
            std::cout << statusBar.getText() << std::endl;
        }
    };

    timer.run(std::chrono::seconds(1));

    return (0);
}

// vim: set filetype=cpp.cpp11 :
