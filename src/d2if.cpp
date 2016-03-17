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
#include <utility>

#include "Battery.hpp"
#include "ColorScheme.hpp"
#include "Cpu.hpp"
#include "Desktop.hpp"
#include "Display.hpp"
#include "Layout.hpp"
#include "Memory.hpp"
#include "Network.hpp"
#include "Player.hpp"
#include "StatusBar.hpp"
#include "Time.hpp"
#include "Timer.hpp"
#include "Volume.hpp"

template <typename T, typename... Args>
inline std::unique_ptr<T>
make_unique(Args &&...args)
{
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}

template <typename T, typename... Args>
inline void
make_unique_vector(std::vector<std::unique_ptr<T>> &)
{
}

template <typename T, typename... Args>
inline void
make_unique_vector(std::vector<std::unique_ptr<T>> &v, std::unique_ptr<T> &&x,
                   Args &&...args)
{
    v.emplace_back(std::move(x));
    make_unique_vector<T>(v, std::forward<Args>(args)...);
}

template <typename T, typename... Args>
inline std::vector<std::unique_ptr<T>>
make_unique_vector(std::unique_ptr<T> &&x, Args &&...args)
{
    std::vector<std::unique_ptr<T>> v;
    make_unique_vector<T>(v, std::move(x), std::forward<Args>(args)...);
    return v;
}

int main(void)
{
    const ColorScheme colorScheme = {{
        { "bg",          "black"   },
        { "fg",          "white"   },
        { "fg-bad",      "black"   },
        { "fg-bar",      "#A6F09D" },
        { "fg-bar-good", "#65A765" },
        { "fg-bar-bad",  "#FF0000" },
    }};

    StatusBar statusBar(make_unique_vector<Field>(
        make_unique<Time>(colorScheme),
        make_unique<Desktop>(colorScheme),
        make_unique<Layout>(colorScheme),
        make_unique<Memory>(colorScheme),
        make_unique<Cpu>(colorScheme),
        make_unique<Display>(colorScheme),
        make_unique<Volume>(colorScheme, "default"),
        make_unique<Battery>(colorScheme),
        make_unique<Network>(colorScheme),
        make_unique<Player>(colorScheme, "127.0.0.1", 6600)
    ));

    statusBar.setFieldDelimiter(" | ");
    statusBar.setFieldDelimiterColor(colorScheme.getColor("fg"));

    Timer timer = {
        [&]() {
            std::cout << statusBar.getText() << std::endl;
        }
    };

    timer.run(std::chrono::seconds(1));

    return EXIT_SUCCESS;
}
