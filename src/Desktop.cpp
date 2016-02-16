// d2if
// Copyright (C) 2015 xaizek.
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

#include "Desktop.hpp"

#include <X11/Xlib.h>

#include <cstdlib>

struct Data
{
    const char *displayName;
    Atom property;
    struct _XDisplay* display;
    Window root;

    Data() : displayName(nullptr), property(None), display(nullptr), root()
    {
    }
};

Desktop::Desktop(const ColorScheme& colorScheme)
    : Field(colorScheme)
{
    data.reset(new Data());

    data->displayName = std::getenv("DISPLAY");
    if (data->displayName == nullptr || data->displayName[0] == '\0') {
        setenv("DISPLAY", ":0", 1);
        data->displayName = std::getenv("DISPLAY");
    }

    if (!tryInit()) {
        setVisible(false);
    }
}

Desktop::~Desktop()
{
    XCloseDisplay(data->display);
}

void Desktop::update()
{
    if (!tryInit()) {
        setVisible(false);
        return;
    }

    unsigned char *value;

    Atom type;
    int format;
    unsigned long nitems;
    unsigned long bytes_after;

    if (XGetWindowProperty(data->display, data->root, data->property, 0,
                           sizeof(char *)/4, 0, AnyPropertyType, &type, &format,
                           &nitems, &bytes_after,
                           (char unsigned **)&value) == Success
        && value != nullptr) {
        setVisible(true);
        Field::setText(reinterpret_cast<char *>(value));
        XFree(value);
    } else {
        setVisible(false);
    }
}

bool Desktop::tryInit()
{
    if (data->display == nullptr) {
        data->display = XOpenDisplay(data->displayName);
        if (data->display == nullptr) {
            return false;
        }
    }

    data->root = DefaultRootWindow(data->display);

    if (data->property == None) {
        data->property = XInternAtom(data->display, "_NET_CURRENT_DESKTOP", 1);
        if (data->property == None) {
            return false;
        }
    }

    return true;
}
