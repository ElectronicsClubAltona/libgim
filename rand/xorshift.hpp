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
 * Copyright 2015-2016 Danny Robson <danny@nerdcruft.net>
 */

#ifndef __UTIL_RAND_XORSHIFT_HPP
#define __UTIL_RAND_XORSHIFT_HPP

namespace util { namespace rand {
    // implements a naive xorshift random generator.
    //
    // * does not comply with the c++11 rng class requirements
    // * users may not rely on identical output across executions or library
    //   updates. internal constants may change across releases
    template <typename T>
    struct xorshift {
    public:
        xorshift (T seed);

        T operator() (void);

    private:
        T m_state;
    };
} }

#endif
