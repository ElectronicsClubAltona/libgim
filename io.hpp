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
 * Copyright 2010-2014 Danny Robson <danny@nerdcruft.net>
 */

#ifndef __UTIL_IO_HPP
#define __UTIL_IO_HPP

#include "types.hpp"
#include "memory.hpp"
#include "platform.hpp"
#include "nocopy.hpp"

#include <cstdio>
#include <cstdint>
#include <memory>
#include <boost/filesystem/path.hpp>

#ifdef PLATFORM_WIN32
#include <windows.h>
#endif

namespace util {
    /// Specifies bitwise combinations of IO access rights.
    enum access_t {
        ACCESS_READ      = 1 << 0,
        ACCESS_WRITE     = 1 << 1,
        ACCESS_READWRITE = ACCESS_READ | ACCESS_WRITE
    };
    
    /// Reads an entire file into memory. Caller frees the result. Guarantees a
    /// null trailing byte.
    std::unique_ptr<char []>
    slurp [[gnu::warn_unused_result]] (const boost::filesystem::path&);

    void
    write (const boost::filesystem::path &, const char *data, size_t len);


    ///------------------------------------------------------------------------
    /// A simple RAII wrapper for file descriptors
    struct fd_ref {
        public:
            int fd;

            explicit fd_ref  (int _fd);
            explicit fd_ref  (const boost::filesystem::path&, access_t access);
            ~fd_ref ();

            operator int (void) const;
    };


#ifdef PLATFORM_WIN32
    struct handle_ref : util::nocopy {
    public:
        explicit handle_ref (HANDLE);
        explicit handle_ref ();
        ~handle_ref ();

        void reset (HANDLE);

        operator HANDLE (void) const;

        HANDLE handle;
    };
#endif


    //-------------------------------------------------------------------------
    class indenter : public std::streambuf {
    protected:
        std::streambuf* m_dest;
        bool            m_line_start;
        std::string     m_indent;
        std::ostream*   m_owner;

    protected:
        virtual int overflow (int ch);

    public:
        explicit indenter (std::streambuf* _dest, size_t _indent = 4);
        explicit indenter (std::ostream&   _dest, size_t _indent = 4);

        virtual ~indenter ();
    };

    //-------------------------------------------------------------------------
    template <typename T>
    struct indented {
        indented (const T &_data);
        const T &data;
    };


    //-------------------------------------------------------------------------
    template <typename T>
    indented<T>
    make_indented (const T &_data);


    //-------------------------------------------------------------------------
    template <typename T>
    std::ostream&
    operator<< (std::ostream &os, const util::indented<T> &v);


    //-------------------------------------------------------------------------
    class scoped_cwd {
        public:
            scoped_cwd ();
            ~scoped_cwd ();

        protected:
            boost::filesystem::path m_original;
    };


    void set_cwd (const boost::filesystem::path &);

    
    //-------------------------------------------------------------------------
    class path_error : public std::runtime_error {
        public:
            path_error (const boost::filesystem::path &path):
                runtime_error ("Invalid path " + path.string ())
            { ; }
    };
}

#ifdef PLATFORM_WIN32
#include "io_win32.hpp"
#else
#include "io_posix.hpp"
#endif

#include "io.ipp"

#endif
