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

namespace util { namespace memory {
    template <typename T, typename U, void (U::*F)(T*)>
    class object_deleter {
    public:
        static_assert (F != nullptr, "destroy function must be non-null");

        object_deleter (U &owner):
            m_owner (owner)
        { ; }

        void operator() (T *t)
        {
            (m_owner.*F) (t);
        }

    private:
        U& m_owner;
    };
} }

#endif
