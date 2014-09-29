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

#include "annotations.hpp"
#include "types.hpp"
#include "memory.hpp"

#include <cstdio>
#include <cstdint>
#include <memory>
#include <boost/filesystem/path.hpp>


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
    slurp (const boost::filesystem::path&) mustuse;

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


#if defined(HAVE_MMAP)
    /// Wraps a mechanism to map a file into memory. Read only.
    class mapped_file {
        private:
            fd_ref   m_fd;
            uint8_t *m_data;
            size_t   m_size;

            void load_fd (access_t);

        protected:
            int access_to_flags (access_t);

        public:
            mapped_file (const boost::filesystem::path &path, access_t access = ACCESS_READ);

            mapped_file (const mapped_file&) = delete;
            mapped_file& operator= (const mapped_file&) = delete;

            ~mapped_file ();

            const uint8_t* data (void) const;
            uint8_t*       data (void);
            size_t         size (void) const;

            uint8_t*       begin (void);
            uint8_t*       end   (void);

            const uint8_t* cbegin (void) const;
            const uint8_t* cend   (void) const;
    };
#endif

    
    class path_error : public std::runtime_error {
        public:
            path_error (const boost::filesystem::path &path):
                runtime_error ("Invalid path " + path.string ())
            { ; }
    };
}

#include "io.ipp"

#endif
