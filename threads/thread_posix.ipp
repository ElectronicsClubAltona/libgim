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

#ifdef __UTIL_THREAD_IPP
#error
#endif
#define __UTIL_THREAD_IPP

#include "tuple.hpp"
#include "except.hpp"


///////////////////////////////////////////////////////////////////////////////
template <typename Func, typename ...Args>
util::threads::thread::thread (Func &&_func, Args&&..._args):
    m_func ([
        func = std::forward<Func> (_func),
        args = std::make_tuple (std::forward<Args> (_args)...)
    ] (void) {
        return tuple::call (func, args);
    })
{
    auto err = pthread_create (&m_id, nullptr, dispatch, &m_func);
    errno_error::try_code (err);
}
