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

#include "format.hpp"

#include <utility>


namespace util { namespace format { namespace detail {
    //-------------------------------------------------------------------------
    std::ostream&
    operator<< (std::ostream &os, specifier::repr r)
    {
        switch (r) {
        case specifier::repr::FIXED:      return os << "FIXED";
        case specifier::repr::SCIENTIFIC: return os << "SCIENTIFIC";
        case specifier::repr::AUTO:       return os << "AUTO";
        }

        unreachable ();
    }


    //-------------------------------------------------------------------------
    std::ostream&
    operator<< (std::ostream &os, specifier::kind t)
    {
        switch (t) {
        case specifier::kind::UNSIGNED:  return os << "UNSIGNED";
        case specifier::kind::SIGNED:    return os << "SIGNED";
        case specifier::kind::REAL:      return os << "REAL";
        case specifier::kind::STRING:    return os << "STRING";
        case specifier::kind::POINTER:   return os << "POINTER";
        case specifier::kind::CHARACTER: return os << "CHARACTER";
        case specifier::kind::ESCAPE:    return os << "ESCAPE";
        case specifier::kind::OSTREAM:   return os << "OSTREAM";
        }

        unreachable ();
    }


    //-------------------------------------------------------------------------
    std::ostream&
    operator<< (std::ostream &os, const specifier &s)
    {
        return os << "specifier {"
            "alternate_form: "     << s.alternate_form      << ", "
            "left_adjusted: "      << s.left_adjusted       << ", "
            "thousands_grouping: " << s.thousands_grouping  << ", "
            "padding_char: '"      << s.padding_char        << "', "
            "positive_char: '"     << s.positive_char       << "', "
            "uppercase: "          << s.uppercase           << ", "
            "base: "               << s.base                << ", "
            "repr: "               << s.r                   << ", "
            "kind: "               << s.k                   << ", "
            "width: "              << s.width               << ", "
            "precision: "          << s.precision           << ", "
            "length: "             << s.length              <<
        " }";
    }
} } }
