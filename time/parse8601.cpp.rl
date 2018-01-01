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
 * Copyright 2017 Danny Robson <danny@nerdcruft.net>
 */

#include "time/parse.hpp"
#include "posix/except.hpp"

#include <stdexcept>
#include <iostream>


///////////////////////////////////////////////////////////////////////////////
%%{
    # based off rfc3339 rather than iso8601 because the former is public

    machine iso8601;

    date_fullyear = digit{4};
    date_month    = digit{2};
    date_mday     = digit{2};

    time_hour     = digit{2};
    time_minute   = digit{2};
    time_second   = digit{2};

    time_secfrac  = '.' digit{1,} ${ frac *= 10; frac += fc - '0'; };
    time_numoffset = ('+' %{dir=1;} | '-' %{dir=-1;})
                         time_hour   ${ offset.tm_hour *= 10; offset.tm_hour += fc - '0'; }
                     ':' time_minute ${ offset.tm_min  *= 10; offset.tm_min  += fc - '0'; };
    time_offset    = 'Z' | time_numoffset;

    partial_time =     time_hour    ${ parts.tm_hour *= 10; parts.tm_hour += fc - '0'; }
                   ':' time_minute  ${ parts.tm_min  *= 10; parts.tm_min  += fc - '0'; }
                   ':' time_second  ${ parts.tm_sec  *= 10; parts.tm_sec  += fc - '0'; }
                       time_secfrac?;

    full_date =     date_fullyear ${ parts.tm_year *= 10; parts.tm_year += fc - '0'; }
                '-' date_month    ${ parts.tm_mon  *= 10; parts.tm_mon  += fc - '0'; }
                '-' date_mday     ${ parts.tm_mday *= 10; parts.tm_mday += fc - '0'; };

    full_time = partial_time time_offset;

    date_time := (
        full_date 'T' full_time
    )
    >{ success = false; }
    %{ success = true;  };

    write data;
}%%


///////////////////////////////////////////////////////////////////////////////
template <>
bool
util::debug::validator<tm>::is_valid (const tm &val) noexcept
{
    // we don't test tm_year anywhere here because there isn't a valid range
    // for years, only that they are expressed as offsets from 1900;
    return val.tm_sec  >= 0 && val.tm_sec  <=  60 &&
           val.tm_min  >= 0 && val.tm_min  <   60 &&
           val.tm_hour >= 0 && val.tm_hour <   24 &&
           val.tm_mday >  0 && val.tm_mday <=  31 &&
           val.tm_mon  >= 0 && val.tm_mon  <   12 &&
           val.tm_wday >= 0 && val.tm_wday <    7 &&
           val.tm_yday >= 0 && val.tm_yday <= 365;
}


//-----------------------------------------------------------------------------
std::ostream&
operator<< (std::ostream &os, const tm&)
{
    return os << "{}";
}


//-----------------------------------------------------------------------------
std::chrono::seconds
to_epoch (const tm &t)
{
    // TODO: it's assumed the user isn't passing in oddities like 36 months or
    // similar. in the future we can account for this
    CHECK_SANITY (t);


    constexpr int
    cumulative_days [12] = {
        0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334
    };

    constexpr int epoch_year = 1970;
    const int year = 1900 + t.tm_year;

    // find the number of days since 1970. careful of leap years.
    time_t secs;
    secs  = (year - epoch_year) * 365 + cumulative_days[t.tm_mon % 12];
    secs += (year - epoch_year + epoch_year %   4) /   4;
    secs -= (year - epoch_year + epoch_year % 100) / 100;
    secs += (year - epoch_year + epoch_year % 400) / 400;

    const bool is_leap_year = (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0));
    if (is_leap_year && t.tm_mon < 2)
        secs--;

    secs += t.tm_mday - 1;

    // hours
    secs *= 24;
    secs += t.tm_hour;

    // minutes
    secs *= 60;
    secs += t.tm_min;

    // seconds
    secs *= 60;
    secs += t.tm_sec;

    if (t.tm_isdst)
        secs -= 60 * 60;

    return std::chrono::seconds {secs};
}



//-----------------------------------------------------------------------------
std::chrono::nanoseconds
util::time::iso8601::parse (util::view<const char*> str)
{
    int cs;
    const char *p   = std::begin (str);
    const char *pe  = std::end    (str);
    const char *eof = pe;

    bool success = false;

    int dir = 0;
    int64_t frac = 0;
    struct tm parts, offset;
    memset (&parts, 0, sizeof (parts));
    memset (&offset, 0, sizeof (offset));

    %%write init;
    %%write exec;

    if (!success)
        throw std::invalid_argument ("invalid date string");

    parts.tm_year -= 1900;
    parts.tm_mon  -= 1;

    // compute the timezone offset
    std::chrono::seconds diff {
        dir * (offset.tm_hour * 60 * 60 + offset.tm_min * 60)
    };

    // fractional part
    auto nano_digits = util::digits10 (std::nano::den-1);
    auto frac_digits = util::digits10 (frac);
    auto shift = util::pow (10, unsigned(nano_digits - frac_digits));

    // sum the time_t, timezone offset, and fractional components
    return to_epoch (parts) - diff + std::chrono::nanoseconds (frac * shift);
}

