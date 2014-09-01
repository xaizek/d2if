// d2if
// Copyright (C) 2014 xaizek.
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

#include "Player.hpp"

#include <unistd.h>

#include <chrono>
#include <stdexcept>
#include <string>
#include <thread>
#include <utility>

#include "MpdClient.hpp"

static std::string getStateMark(MpdState state);

Player::Player(const ColorScheme& colorScheme, std::string host, int port)
    : Field(colorScheme)
    , host(std::move(host))
    , port(port)
{
    std::thread([this]()
    {
        while (true) {
            try {
                MpdClient mpdClient(this->host, this->port);

                while (true) {
                    const std::string song = mpdClient.getCurrentSong();
                    const MpdState state = mpdClient.getState();

                    // XXX: this might be non thread safe
                    status = getStateMark(state) + ' ' + song;

                    mpdClient.waitForChanges();
                }
            } catch (std::runtime_error &e) {
                using namespace std::chrono;

                auto ms = duration_cast<microseconds>(
                    milliseconds(500)
                ).count();

                usleep(ms);

                // XXX: this might be non thread safe
                status.clear();
            }
        }
    }).detach();
}

static std::string getStateMark(MpdState state)
{
    switch (state) {
        case MpdState::Stop: return "[-]";
        case MpdState::Play: return "[>]";
        case MpdState::Pause: return "[=]";

        default: return std::string();
    }
}

void Player::update()
{
    Field::setText(status);
}
