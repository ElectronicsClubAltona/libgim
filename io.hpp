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
#include "platform.hpp"
#include "nocopy.hpp"

#include <sys/stat.h>

#include <cstdio>
#include <cstdint>
#include <memory>
#include <boost/filesystem/path.hpp>

#ifdef PLATFORM_WIN32
#include <windows.h>
#else
#define O_BINARY 0
#endif

namespace util {
    ///------------------------------------------------------------------------
    /// A simple RAII wrapper for file descriptors
    struct fd {
    public:
        explicit fd (int);
        fd (const char *path, int flags, mode_t mode = 0660);
        fd (const boost::filesystem::path&, int flags);

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


    /// Reads an entire file into memory.
    std::vector<char> slurp (const boost::filesystem::path&);
    std::vector<char> slurp (FILE *);


    //-------------------------------------------------------------------------
    void write (const fd&, const void *restrict data, size_t bytes);

    template <typename T>
    void write (const fd&, const T &data);

    template <typename T>
    void write (const fd&, const T *restrict first, const T *restrict last);

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
        path_error (const boost::filesystem::path &path);

        const char* path (void) const noexcept;

    private:
        const boost::filesystem::path m_path;
    };

    class stream_error : public std::exception {
    public:
        virtual ~stream_error () { ; }

        virtual const char* what (void) const noexcept
        { return "error on C stream"; }
    };
}

#ifdef PLATFORM_WIN32
#include "./io_win32.hpp"
#else
#include "./io_posix.hpp"
#endif

#include "./io.ipp"

#endif
