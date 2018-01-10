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
 * Copyright:
 *      2014-2015, Danny Robson <danny@nerdcruft.net>
 */


#ifdef __UTIL_IO_IPP
#error "multiple inclusions"
#else
#define __UTIL__IO_IPP
#endif

namespace util {
    //-------------------------------------------------------------------------
    template <typename T>
    void
    write (const posix::fd &_fd, const T *restrict first, const T *restrict last)
    {
        write (_fd, first, (last - first) * sizeof (T));
    }

    //-------------------------------------------------------------------------
    template <typename T>
    indented<T>::indented (const T &_data):
        data (_data)
    { ; }


    //-------------------------------------------------------------------------
    template <typename T>
    indented<T>
    make_indented (const T &_data) {
        return indented<T> (_data);
    }


    //-------------------------------------------------------------------------
    template <typename T>
    std::ostream&
    operator<< (std::ostream &os, const util::indented<T> &&v) {
        util::indenter raii (os);
        os << v.data;
        return os;
    }
}
