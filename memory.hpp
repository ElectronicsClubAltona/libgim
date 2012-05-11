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
 * Copyright 2011 Danny Robson <danny@nerdcruft.net>
 */

#ifndef __MEMORY_HPP
#define __MEMORY_HPP

template <typename T>
class scoped_malloc {
public:
    scoped_malloc (T *_data):
        m_data (_data)
    { ; }

    scoped_malloc (scoped_malloc &&rhs):
        m_data (rhs.m_data)
    {
        rhs.m_data = nullptr;
    }

    ~scoped_malloc () {
        free (m_data);
    }

          T* get (void)        { return m_data; }
    const T* get (void) const  { return m_data; }

    bool operator! (void) { return !m_data; }

protected:
    T* m_data;
};

#endif
