// xkeyboard.cpp
// Implementation of a class to get keyboard layout information and change layouts
// Copyright (C) 2008 by Jay Bromley <jbromley@gmail.com>
// Copyright (C) 2012 by xaizek <xaize@openmailbox.org>
//
// This program is free software; you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by the Free
// Software Foundation; either version 2 of the License, or (at your option)
// any later version.

#include "XKeyboard.hpp"

#include <X11/XKBlib.h>
#include <X11/Xlib.h>

#include <cstddef>
#include <cstdlib>
#include <cctype>
#include <cstring>

#include <algorithm>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

// XKeyboard -----------------------------------------------------------

XKeyboard::XKeyboard()
    : _display(0), _groupCount(0), _currentGroupNum(0),
      _deviceId(XkbUseCoreKbd)
{

    XkbIgnoreExtension(False);

    char* displayName = getenv("DISPLAY");
    int eventCode;
    int errorReturn;
    int major = XkbMajorVersion;
    int minor = XkbMinorVersion;;
    int reasonReturn;
    _display = XkbOpenDisplay(displayName, &eventCode, &errorReturn, &major,
                              &minor, &reasonReturn);
    switch (reasonReturn) {
    case XkbOD_BadLibraryVersion:
        throw std::runtime_error("Bad XKB library version.");
        break;
    case XkbOD_ConnectionRefused:
        throw std::runtime_error("Connection to X server refused.");
        break;
    case XkbOD_BadServerVersion:
        throw std::runtime_error("Bad X11 server version.");
        break;
    case XkbOD_NonXkbServer:
        throw std::runtime_error("XKB not present.");
        break;
    case XkbOD_Success:
        break;
    }

    if (initializeXkb() != True) {
        throw std::runtime_error("XKB not initialized.");
    }

    XkbSelectEventDetails(_display, XkbUseCoreKbd, XkbStateNotify,
                          XkbAllStateComponentsMask, XkbGroupStateMask);

    XkbStateRec xkbState;
    XkbGetState(_display, _deviceId, &xkbState);
    _currentGroupNum = (_currentGroupNum != xkbState.group) ? xkbState.group : _currentGroupNum;
    accomodateGroupXkb();
}

bool XKeyboard::initializeXkb()
{
    // Initialize the XKB extension.
    int major = XkbMajorVersion;
    int minor = XkbMinorVersion;
    int opCode;
    int _baseEventCode;
    int _baseErrorCode;
    (void)XkbQueryExtension(_display, &opCode, &_baseEventCode, &_baseErrorCode,  &major, &minor);

    static const auto kbdDescDeleter = [] (XkbDescRec *kbd) {
        if (kbd != nullptr) {
            XkbFreeKeyboard(kbd, 0, True);
        }
    };

    std::unique_ptr<XkbDescRec, decltype(kbdDescDeleter)> kbdDescPtr {
        XkbAllocKeyboard(), kbdDescDeleter
    };

    if (kbdDescPtr == NULL) {
        std::cerr << "Failed to get keyboard description." << std::endl;
        return False;
    }

    kbdDescPtr->dpy = _display;
    if (_deviceId != XkbUseCoreKbd) {
        kbdDescPtr->device_spec = _deviceId;
    }

    XkbGetControls(_display, XkbAllControlsMask, kbdDescPtr.get());
    XkbGetNames(_display, XkbSymbolsNameMask, kbdDescPtr.get());
    XkbGetNames(_display, XkbGroupNamesMask, kbdDescPtr.get());

    if (kbdDescPtr->names == NULL) {
        std::cerr << "Failed to get keyboard description." << std::endl;
        return False;
    }

    // Count the number of configured groups.
    const Atom* groupSource = kbdDescPtr->names->groups;
    if (kbdDescPtr->ctrls != NULL) {
        _groupCount = kbdDescPtr->ctrls->num_groups;
    } else {
        _groupCount = 0;
        while (_groupCount < XkbNumKbdGroups &&
               groupSource[_groupCount] != None) {
            _groupCount++;
        }
    }

    // There is always at least one group.
    if (_groupCount == 0) {
        _groupCount = 1;
    }

    // Get the group names.
    const Atom* tmpGroupSource = kbdDescPtr->names->groups;
    Atom curGroupAtom;
    std::string groupName;
    for (int i = 0; i < _groupCount; i++) {
        if ((curGroupAtom = tmpGroupSource[i]) != None) {
            char* groupNameC = XGetAtomName(_display, curGroupAtom);
            if (groupNameC == NULL) {
                _groupNames.push_back("");
            } else {
                groupName = groupNameC;
                std::string::size_type pos = groupName.find('(', 0);
                if (pos != std::string::npos) {
                    groupName = groupName.substr(0, pos + 1);
                }
                _groupNames.push_back(groupName);
            }
            XFree(groupNameC);
        }
    }

    // Get the symbol name and parse it for layout symbols.
    Atom symNameAtom = kbdDescPtr->names->symbols;
    std::string symName;
    if (symNameAtom != None) {
        char* symNameC = XGetAtomName(_display, symNameAtom);
        symName = symNameC;
        XFree(symNameC);
        if (symName.empty()) {
            return False;
        }
    } else {
        return False;
    }

    XkbSymbolParser symParser;
    symParser.parse(symName, _symbolNames);
    int count = _symbolNames.size();
    if (count == 1 && _groupNames[0].empty() && _symbolNames[0] == "jp") {
        _groupCount = 2;
        _symbolNames[1] = _symbolNames[0];
        _symbolNames[0] = "us";
        _groupNames[0] = "US/ASCII";
        _groupNames[1] = "Japanese";
    } else {
        if (count < _groupCount) {
            int j = count;
            int k = _groupCount;
            while (--j >= 0) _symbolNames[--k] = _symbolNames[j];
            while (--k >= 0) _symbolNames[k] = "en_US";
        }
    }

    count = _groupNames.size();
    for (int i = 0; i < count; i++) {
        if (_groupNames[i].empty()) {
            std::string name = getSymbolNameByResNum(i);
            if (name.empty()) {
                name = "U/A";
            }
            std::cerr << "Group Name " << i + 1 << " is undefined, set to '"
                      << name << "'!\n";
            _groupNames[i] = name;
        }
    }

    XkbStateRec xkbState;
    XkbGetState(_display, _deviceId, &xkbState);
    _currentGroupNum = xkbState.group;

    return True;
}

std::string XKeyboard::getSymbolNameByResNum(int groupResNum)
{
    return _symbolNames[groupNumResToXkb(groupResNum)];
}

int XKeyboard::groupNumResToXkb(int groupResNum)
{
    return groupLookup(groupResNum, _groupNames, _symbolNames, _groupCount);
}

int XKeyboard::groupLookup(int srcValue, StringVector fromText, StringVector toText, int count)
{
    const std::string srcText = fromText[srcValue];

    if (!srcText.empty()) {
        std::string targetText;

        for (int i = 0; i < count; i++) {
            targetText = toText[i];
            if (strcasecmp(srcText.c_str(), targetText.c_str()) == 0) {
                srcValue = i;
                break;
            }
        }
    }

    return srcValue;
}

void XKeyboard::accomodateGroupXkb()
{
    XkbStateRec state;
    XkbGetState(_display, _deviceId, &state);
    _currentGroupNum = state.group;
}


XKeyboard::~XKeyboard()
{
    XCloseDisplay(_display);
}

int XKeyboard::currentGroupNum() const
{
    XkbStateRec xkbState;
    XkbGetState(_display, _deviceId, &xkbState);
    return static_cast<int>(xkbState.group);
}

std::size_t XKeyboard::getGroupCount() const
{
    static_cast<void>(const_cast<XKeyboard *>(this)->initializeXkb());
    return static_cast<std::size_t>(_groupNames.size());
}

const std::string & XKeyboard::getCurrentGroupSymbol() const
{
    return _symbolNames[currentGroupNum()];
}


// XkbSymbolParser -----------------------------------------------------

XkbSymbolParser::XkbSymbolParser()
{
    _nonSymbols.push_back("group");
    _nonSymbols.push_back("inet");
    _nonSymbols.push_back("pc");
}

XkbSymbolParser::~XkbSymbolParser()
{
    _nonSymbols.clear();
}

void XkbSymbolParser::parse(const std::string& symbols, StringVector& symbolList)
{
    bool inSymbol = false;
    std::string curSymbol;

    for (size_t i = 0; i < symbols.size(); i++) {
        char ch = symbols[i];
        if (ch == '+') {
            if (inSymbol) {
                if (isXkbLayoutSymbol(curSymbol)) {
                    symbolList.push_back(curSymbol);
                }
                curSymbol.clear();
            } else {
                inSymbol = true;
            }
        } else if (inSymbol && (isalpha(static_cast<int>(ch)) || ch == '_')) {
            curSymbol.append(1, ch);
        } else {
            if (inSymbol) {
                if (isXkbLayoutSymbol(curSymbol)) {
                    symbolList.push_back(curSymbol);
                }
                curSymbol.clear();
                inSymbol = false;
            }
        }
    }

    if (inSymbol && !curSymbol.empty() && isXkbLayoutSymbol(curSymbol)) {
        symbolList.push_back(curSymbol);
    }
}

bool XkbSymbolParser::isXkbLayoutSymbol(const std::string& symbol) {
    StringVectorIter result = find(_nonSymbols.begin(), _nonSymbols.end(), symbol);
    return result == _nonSymbols.end();
}
