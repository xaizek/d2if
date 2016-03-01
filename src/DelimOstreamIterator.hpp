// d2if
// Copyright (C) 2016 xaizek.
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

#ifndef DELIMOSTREAMITERATOR_HPP__
#define DELIMOSTREAMITERATOR_HPP__

#include <iterator>
#include <iosfwd>

/**
 * @brief Output iterator that doesn't output delimiter after last element.
 *
 * @tparam T Type of value that's being printed to a stream.
 */
template <typename T>
class DelimOstreamIterator :
    public std::iterator<std::output_iterator_tag, void, void, void, void>
{
public:
    /**
     * @brief Constructs output iterator for specific stream and delimiter.
     *
     * @param os Stream to print into.
     * @param delim Value delimiter.
     */
    DelimOstreamIterator(std::ostream &os, const char delim[])
        : os(os), delim(delim), first(true)
    {
    }

public:
    /**
     * @brief No-op.
     *
     * @returns @c *this;
     */
    DelimOstreamIterator & operator*() { return *this; }
    /**
     * @brief No-op.
     *
     * @returns @c *this;
     */
    DelimOstreamIterator & operator++() { return *this; }
    /**
     * @brief No-op.
     *
     * @returns @c *this;
     */
    DelimOstreamIterator & operator++(int) { return *this; }

    /**
     * @brief Performs actual work of processing values.
     *
     * @param v Value to print.
     *
     * @returns @c *this.
     */
    DelimOstreamIterator & operator=(const T &v)
    {
        if (first) {
            os << v;
            first = false;
        } else {
            os << delim << v;
        }
        return *this;
    }

private:
    /**
     * @brief Output stream to write values into.
     */
    std::ostream &os;
    /**
     * @brief Delimiter of values.
     */
    const char *const delim;
    /**
     * @brief Whether this is the first value print.
     */
    bool first;
};

#endif // DELIMOSTREAMITERATOR_HPP__
