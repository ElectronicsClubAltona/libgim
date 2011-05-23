#include "stream.hpp"

std::ostream&
nullstream::put (char c)
    { return *this; }


bool
nullstream::good (void) const
    { return !bad () && !eof () && !fail (); }


bool nullstream::bad  (void) const { return false; }
bool nullstream::eof  (void) const { return false; }
bool nullstream::fail (void) const { return false; }


