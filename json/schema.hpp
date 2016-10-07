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

#ifndef __UTIL_JSON_SCHEMA_HPP
#define __UTIL_JSON_SCHEMA_HPP

#include "./fwd.hpp"

#include <memory>
#include <experimental/filesystem>

namespace json { namespace schema {
    // Validate the json tree using the provide schema object or path.
    //
    // Note that the data object being validated may be altered in the process
    // of validation. If a value is not present but the schema specifies a
    // default, it will be realised in the data object.
    void validate (json::tree::node &data, const json::tree::object &schema);
    void validate (json::tree::node &data, const std::experimental::filesystem::path &schema);
} }

#endif
