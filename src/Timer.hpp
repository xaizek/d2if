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

#ifndef __TIMER_HPP__
#define __TIMER_HPP__

#include <chrono>
#include <functional>

class Timer
{
public:
    typedef std::function<void()> handler_t;

    Timer(handler_t handler);

    // These operations are forbidden.
    Timer(Timer &rhs) = delete;
    Timer & operator=(Timer &rhs) = delete;

    void run(std::chrono::milliseconds period);

private:
    const handler_t handler;
};

#endif // __TIMER_HPP__
