/*
 * This file is part of libgim.
 *
 * libgim is free software: you can redistribute it and/or modify it under the
 * terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later
 * version.
 *
 * libgim is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License
 * along with libgim.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Copyright 2010 Danny Robson <danny@nerdcruft.net>
 */


#ifndef __EXCEPT_HPP
#define __EXCEPT_HPP

#include <stdexcept>


class input_error : public std::runtime_error {
    public:
        input_error (const std::string &_what):
                runtime_error (_what)
        { ; }
};


class unavailable_error : public std::runtime_error {
    public:
        unavailable_error (const std::string &_what):
                runtime_error (_what)
        { ; }
};


/// An exception class used for reporting errors signalled by errno.
class errno_error : public std::runtime_error {
    public:
        int id;
        errno_error (int _errno);
        errno_error ();

        static void try_code (void);
        static void try_code (int code);
};


#endif 
