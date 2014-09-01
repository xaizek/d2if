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

#ifndef D2IF__MPDCLIENT_HPP__
#define D2IF__MPDCLIENT_HPP__

#include <map>
#include <string>

class Sock
{
public:
    Sock();
    Sock(int sock);
    ~Sock();

private:
    // these operations are forbidden
    Sock(const Sock &rhs);
    Sock & operator=(const Sock &rhs);

public:
    operator int();
    Sock & operator=(int sock);

private:
    void reset(int newSock = -1);

private:
    int sock;
};

enum class MpdState
{
    Stop,
    Play,
    Pause,
};

class MpdClient
{
public:
    MpdClient(const std::string &host, int port);

    std::string sendAndReceive(const std::string &request);

    std::string getCurrentSong();

    MpdState getState();

    void waitForChanges();

private:
    typedef std::map<std::string, std::string> colon_data_t;

    colon_data_t makeRequest(const std::string &cmd);

    void send(std::string request);

    std::string receive();

private:
    Sock sock;
};

#endif // D2IF__MPDCLIENT_HPP__
