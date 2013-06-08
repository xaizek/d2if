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

#include "StatusBar.hpp"

#include <algorithm>
#include <functional>
#include <iostream>
#include <iterator>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include "Field.hpp"

StatusBar::StatusBar(std::initializer_list<std::shared_ptr<Field>> fields)
    :fields(fields.begin(), fields.end())
{
}

void StatusBar::addField(std::shared_ptr<Field> field)
{
    fields.push_back(field);
}

void StatusBar::setFieldDelimiter(const std::string &delimiter)
{
    fieldDelimiter = delimiter;
    sepCache = "^pa(;0)" + colorCache + fieldDelimiter;
}

void StatusBar::setFieldDelimiterColor(const std::string &delimiterColor)
{
    fieldDelimiterColor = delimiterColor;
    colorCache = "^fg(" + fieldDelimiterColor + ")";
}

std::string StatusBar::getText() const
{
    std::vector<Field*> visibleFields;
    visibleFields.reserve(fields.size());

    for (const std::shared_ptr<Field> &field : fields) {
        field->update();
        if (field->isVisible()) {
            visibleFields.push_back(field.get());
        }
    }

    std::ostringstream result;
    result << "^pa(2;0)";

    std::ostream_iterator<Field*> outIt { result, sepCache.c_str() };
    if (visibleFields.size() > 1) {
        std::copy(visibleFields.begin(), visibleFields.end() - 1, outIt);
    }
    if (!visibleFields.empty()) {
        result << visibleFields.back();
    }

    return (result.str());
}

// vim: set filetype=cpp.cpp11 :
