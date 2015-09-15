/*
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
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


    ///------------------------------------------------------------------------
    /// A simple RAII wrapper for file descriptors
    struct fd {
    public:
        explicit fd (int);
        explicit fd (const boost::filesystem::path&, access_t);

        ~fd ();

        operator int (void) const { return m_fd; }

    private:
        int m_fd;
    };


    //-------------------------------------------------------------------------
#ifdef PLATFORM_WIN32
    struct handle : util::nocopy {
    public:
        explicit handle (HANDLE);
        explicit handle ();
        ~handle ();

        void reset (HANDLE);

        operator HANDLE (void) const;

        HANDLE fd;
    };
#endif


    /// Reads an entire file into memory. Caller frees the result. Guarantees a
    /// null trailing byte.
    std::vector<char>
    slurp [[gnu::warn_unused_result]] (const boost::filesystem::path&);


    template <typename T>
    void write (const fd&, const T *restrict data, size_t count);

    template <typename T>
    void write (const fd&, const T *restrict first, const T *restrict last);

    template <typename T>
    void write (const boost::filesystem::path &, const T *restrict first, const T *restrict last);


    //-------------------------------------------------------------------------
    class indenter : public std::streambuf {
    protected:
        std::streambuf* m_dest;
        bool            m_line_start;
        std::string     m_indent;
        std::ostream*   m_owner;

    protected:
        virtual int overflow (int ch) override;

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
            std::string m_original;
    };


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
