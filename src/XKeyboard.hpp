// xkeyboard.hpp
// Interface for a class to get keyboard layout information and change layouts
// Copyright (C) 2008 by Jay Bromley <jbromley@gmail.com>
// Copyright (C) 2012 by xaizek <xaize@lavabit.com>
//
// This program is free software; you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by the Free
// Software Foundation; either version 2 of the License, or (at your option)
// any later version.

#ifndef XKEYBOARD_HPP
#define XKEYBOARD_HPP

#include <string>
#include <vector>


// XKeyboard -----------------------------------------------------------

class XKeyboard
{
public:
    XKeyboard();
    ~XKeyboard();

    std::string currentGroupSymbol() const;

private:
    typedef std::vector<std::string> StringVector;

    bool initializeXkb();
    int currentGroupNum() const;
    std::string getSymbolNameByResNum(int groupResNum);
    int groupNumResToXkb(int groupNumRes);
    int groupLookup(int srcValue, StringVector fromText, StringVector toText, int count);
    void accomodateGroupXkb();

    struct _XDisplay* _display;
    int _groupCount;
    StringVector _groupNames;
    StringVector _symbolNames;
    int _currentGroupNum;

    int _deviceId;
};


// XkbSymbolParser -----------------------------------------------------

class XkbSymbolParser
{
public:
    typedef std::vector<std::string>::iterator StringVectorIter;

    XkbSymbolParser();
    ~XkbSymbolParser();
    void parse(const std::string& symbols, std::vector<std::string>& symbolList);

private:
    bool isXkbLayoutSymbol(const std::string& symbol);

    typedef std::vector<std::string> StringVector;
    StringVector _nonSymbols;
};

#endif // XKEYBOARD_HPP
