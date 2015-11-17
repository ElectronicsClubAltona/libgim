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

#include "./deleter.hpp"

using util::memory::object_deleter;


///////////////////////////////////////////////////////////////////////////////
// This instantiation isn't required but it allows us to catch compilation
// errors early within this library rather than at a call site in another
// library.

class owner {
    void dealloc (void*);
};


template class util::memory::object_deleter<void,owner,&owner::dealloc>;
