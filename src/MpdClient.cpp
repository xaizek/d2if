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

#include "MpdClient.hpp"

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <netdb.h>
#include <unistd.h>

#include <cerrno>

#include <algorithm>
#include <iterator>
#include <map>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

namespace
{
    class PerLine
    {
    public:
        friend std::istream & operator>>(std::istream &is, PerLine &pl)
        {
            std::string str;
            std::getline(is, str);
            pl.str = str;
            return is;
        }

        operator std::string() const
        {
            return str;
        }

    private:
        std::string str;
    };
}

Sock::Sock()
    : sock(-1)
{
}

Sock::Sock(int sock)
    : sock(sock)
{
}

Sock::~Sock()
{
    reset();
}

Sock::operator int()
{
    return sock;
}

Sock & Sock::operator=(int sock)
{
    reset(sock);
    return *this;
}

void Sock::reset(int newSock)
{
    if (sock != -1) {
        ::close(sock);
    }
    sock = newSock;
}

MpdClient::MpdClient(const std::string &host, int port)
{
    sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock == -1) {
        throw std::runtime_error("Can't create socket.");
    }

    sockaddr_in addr = { };

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(host.c_str());
    addr.sin_port = htons(port);

    if (::connect(sock, reinterpret_cast<sockaddr *>(&addr),
                  sizeof(addr)) != 0) {
        throw std::runtime_error("Can't connect socket.");
    }

    (void)receive();
}

std::string MpdClient::sendAndReceive(const std::string &request)
{
    send(request);
    return receive();
}

std::string MpdClient::getCurrentSong()
{
    colon_data_t metadata = makeRequest("currentsong");

    const std::string artist = metadata["Artist"];
    const std::string title = metadata["Title"];

    if (artist.empty() || title.empty()) {
        return artist.empty() ? title : artist;
    }

    return artist + " - " + title;
}

MpdState MpdClient::getState()
{
    colon_data_t metadata = makeRequest("status");

    const std::string stateStr = metadata["state"];
    if (stateStr == "play") {
        return MpdState::Play;
    } else if(stateStr == "pause") {
        return MpdState::Pause;
    } else {
        return MpdState::Stop;
    }
}

void MpdClient::waitForChanges()
{
    static_cast<void>(sendAndReceive("idle player"));
}

MpdClient::colon_data_t MpdClient::makeRequest(const std::string &cmd)
{
    std::istringstream response(sendAndReceive(cmd));

    std::vector<std::string> lines;
    typedef std::istream_iterator<PerLine> isIt;
    std::copy(isIt(response), isIt(), std::back_inserter(lines));

    if (lines.empty()) {
        throw std::runtime_error("Got empty MPD.");
    }

    if (lines[lines.size() - 1] != "OK") {
        throw std::runtime_error("Got bad MPD response.");
    }
    lines.pop_back();

    colon_data_t colonData;
    typedef std::vector<std::string>::const_iterator it_t;
    for (it_t it = lines.begin(); it != lines.end(); ++it) {
        const std::string &str = *it;

        const std::size_t colonPos = str.find(':');
        if (colonPos == std::string::npos) {
            continue;
        }

        const std::size_t dataPos = str.find_first_not_of(' ', colonPos + 1);
        if (dataPos == std::string::npos) {
            continue;
        }

        const std::string tag(str, 0, colonPos);
        const std::string value(str, dataPos);
        colonData[tag] = value;
    }

    return colonData;
}

void MpdClient::send(std::string request)
{
    if (request.empty()) {
        throw std::invalid_argument("Request can't be empty.");
    }

    if (request[request.size() - 1] != '\n') {
        request += '\n';
    }

    const ssize_t n = request.size();
    if (::send(sock, request.data(), n, 0) != n) {
        throw std::runtime_error("Can't send.");
    }
}

std::string MpdClient::receive()
{
    std::string response;

    int flags = 0;

    while (true) {
        std::string buf(4096, '\0');

        const ssize_t n = ::recv(sock, &buf[0], buf.size(), flags);

        if (n == 0) {
            // mpd exited
            break;
        }

        if (n == -1) {
            if (errno == EAGAIN || errno == EWOULDBLOCK) {
                break;
            }

            throw std::runtime_error("Can't receive: " +
                                     std::string(::strerror(errno)));
        }

        response.append(buf.data(), n);

        flags = MSG_DONTWAIT;
    }

    return response;
}
