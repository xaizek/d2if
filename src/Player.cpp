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

#include <atomic>
#include <chrono>
#include <memory>
#include <stdexcept>
#include <string>
#include <thread>
#include <utility>

#include "MpdClient.hpp"

static std::string getStateMark(MpdState state);

Player::Player(const ColorScheme& colorScheme, std::string host, int port)
    : Field(colorScheme)
{
    std::thread([=]()
    {
        while (true) {
            try {
                MpdClient mpdClient(host, port);

                while (true) {
                    const std::string song = mpdClient.getCurrentSong();
                    const MpdState state = mpdClient.getState();

                    std::string newStatus = getStateMark(state) + " " + song;
                    std::atomic_exchange(
                        &status, std::make_shared<std::string>(newStatus));

                    mpdClient.waitForChanges();
                }
            } catch (std::runtime_error &) {
                std::atomic_exchange(&status, {});
            }

            // Do not consume CPU for nothing while there is no connection.
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }
    }).detach();
}

static std::string getStateMark(MpdState state)
{
    switch (state) {
        case MpdState::Stop: return "▷▷";
        case MpdState::Play: return "▶▶";
        case MpdState::Pause: return "▶▷";

        default: return std::string();
    }
}

void Player::update()
{
    if (std::shared_ptr<std::string> text = std::atomic_exchange(&status, {})) {
        Field::setText(*text);
    }
}
