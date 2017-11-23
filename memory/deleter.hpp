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
 * Copyright 2015 Danny Robson <danny@nerdcruft.net>
 */

#ifndef __UTIL_MEMORY_DELETER_HPP
#define __UTIL_MEMORY_DELETER_HPP

#include <functional>

namespace util::memory {
    template <typename T>
    class func_deleter {
    public:
        using func_t = std::function<void(T*)>;

        explicit func_deleter (func_t _func):
            m_func (_func)
        { ; }

        inline void operator() (T *t)
        { m_func (t); }

    private:
        func_t m_func;
    };


    // dispatch object deletion to a known member function.
    //
    // XXX: Generates a "sorry, unimplemented" under GCC (which is
    // effectively an ICE). Their bug tracker seems to indicate they don't
    // give a fuck, so we can't use this except under clang.
    template <
        typename ValueT,
        typename OwnerT,
        void (OwnerT::*Func)(ValueT*)
    >
    class owner_deleter {
    public:
        owner_deleter (OwnerT &owner):
            m_owner (owner)
        { ; }

        inline void operator() (ValueT *t)
        {
            (m_owner.*Func) (t);
        }

    private:
        OwnerT& m_owner;
    };
}

#endif
