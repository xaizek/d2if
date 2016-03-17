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
#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include "DelimOstreamIterator.hpp"
#include "Field.hpp"

StatusBar::StatusBar(std::vector<std::unique_ptr<Field>> fields)
    : fields(std::move(fields))
{
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

    // reset delimeter to update its cache
    setFieldDelimiter(fieldDelimiter);
}

std::string StatusBar::getText() const
{
    std::vector<Field*> visibleFields;
    visibleFields.reserve(fields.size());

    for (const std::unique_ptr<Field> &field : fields) {
        field->update();
        if (field->isVisible()) {
            visibleFields.push_back(field.get());
        }
    }

    std::ostringstream result;
    result << "^pa(2;0)";
    std::copy(visibleFields.begin(), visibleFields.end(),
              DelimOstreamIterator<Field*>(result, sepCache.c_str()));

    return result.str();
}
