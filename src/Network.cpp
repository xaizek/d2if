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

#include "Network.hpp"

#include <sys/types.h>
#include <ifaddrs.h>

#include <fstream>
#include <iomanip>
#include <map>
#include <memory>
#include <sstream>
#include <string>

template<class T>
T readFromFile(const std::string &filename);

Network::Network()
{
    last = getIfacesInfo();
}

void Network::update()
{
    std::stringstream result;

    const Network::ifaceList ifaces = getIfacesInfo();
    for (const std::pair<std::string, ifaceInfo> &iface : ifaces) {
        const auto prevIt = last.find(iface.first);
        const ifaceInfo prev {
            (prevIt == last.cend()
             || !std::get<1>(iface.second)
             || !std::get<1>(prevIt->second))
                ? iface.second
                : prevIt->second
        };
        const long rx { (std::get<2>(iface.second) - std::get<2>(prev))/1024 };
        const long tx { (std::get<3>(iface.second) - std::get<3>(prev))/1024 };

        const std::string fgColor {
            std::get<1>(iface.second)
                ? "^fg(white)"
                : "^fg(black)"
        };

        result << "^fg(white)" << std::get<0>(iface.second) << ": "
               << fgColor
               << std::setw(3) << rx << " k↓ "
               << std::setw(3) << tx << " k↑";
    }

    Field::setText(result.str());

    for (const std::pair<std::string, ifaceInfo> &iface : ifaces) {
        auto it = last.find(iface.first);
        if (it != last.end()) {
            last.erase(it);
        }
    }
    last.insert(ifaces.begin(), ifaces.end());
}

Network::ifaceList Network::getIfacesInfo() const
{
    const IfacesHead head = getIfacesHead();

    Network::ifaceList result;
    const ifaddrs *runner = head.get();
    while ((runner = runner->ifa_next) != nullptr) {
        const std::string path {
            std::string("/sys/class/net/") + runner->ifa_name + "/operstate"
        };

        const std::string state { readFromFile<std::string>(path) };

        if (state == "up") {
            if (runner->ifa_addr->sa_family == AF_INET) {
                const auto p = result.find(runner->ifa_name);
                if (p != result.end()) {
                    result.erase(p);
                }

                const std::string rxPath {
                    std::string("/sys/class/net/") + runner->ifa_name + "/statistics/rx_bytes"
                };
                const long rx { readFromFile<long>(rxPath) };

                const std::string txPath {
                    std::string("/sys/class/net/") + runner->ifa_name + "/statistics/tx_bytes"
                };
                const long tx { readFromFile<long>(txPath) };

                result.insert({runner->ifa_name, ifaceInfo{runner->ifa_name, true, rx, tx}});
            } else if (result.find(runner->ifa_name) == result.end()) {
                result.insert({runner->ifa_name, ifaceInfo{runner->ifa_name, false, 0, 0}});
            }
        }
    }

    return result;
}

Network::IfacesHead Network::getIfacesHead() const
{
    static const auto ifAddrsDeleter = [] (ifaddrs *head) {
        freeifaddrs(head);
    };

    ifaddrs *head;
    if (getifaddrs(&head) != 0) {
        return {};
    }

    return {head, ifAddrsDeleter};
}

template<class T>
T readFromFile(const std::string &filename)
{
    std::ifstream stream(filename);

    T result;
    stream >> result;

    return result;
}

// vim: set filetype=cpp.cpp11 :
