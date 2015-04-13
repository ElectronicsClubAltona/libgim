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
 * Copyright 2010 Danny Robson <danny@nerdcruft.net>
 */

#ifndef __UTIL_STATS_HPP
#define __UTIL_STATS_HPP

#include <iostream>


namespace util {
    namespace stats {
        // Undefined until at least one value has been added.

        template <typename T>
        struct accumulator {
            accumulator ();

            void reset (void);

            void add (T);
            void add (const accumulator<T> &);

            size_t count;

            T min;
            T max;
            T sum;

            T range (void) const;
            T mean  (void) const;
        };

        template <typename T>
        std::ostream& operator<< (std::ostream&, const accumulator<T>&);
    }
}

#endif
