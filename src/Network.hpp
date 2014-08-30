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

#ifndef __NETWORK_HPP__
#define __NETWORK_HPP__

#include <map>
#include <string>
#include <tuple>

#include "Field.hpp"

struct ifaddrs;

class ColorScheme;

class Network : public Field
{
public:
    Network(const ColorScheme& colorScheme);

    virtual void update() override;

private:
    typedef std::tuple<std::string, bool, int, int> ifaceInfo;
    typedef std::map<std::string, ifaceInfo> ifaceList;

    ifaceList last;

    ifaceList getIfacesInfo() const;

    typedef std::unique_ptr<ifaddrs, std::function<void(ifaddrs*)>> IfacesHead;
    IfacesHead getIfacesHead() const;
};

#endif // __NETWORK_HPP__

// vim: set filetype=cpp.cpp11 :
