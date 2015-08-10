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

#ifndef __UTIL_THREADS_CONDITION_HPP
#define __UTIL_THREADS_CONDITION_HPP

#include "mutex.hpp"

#include <chrono>

namespace util { namespace threads {
    class condition {
    public:
        condition ();
        ~condition ();

        void wait (mutex&);
        void wait (mutex&, std::chrono::nanoseconds);

        void signal (void);
        void broadcast (void);
        
        pthread_cond_t native (void);

    private:
        pthread_cond_t m_id;
    };
} }

#endif
