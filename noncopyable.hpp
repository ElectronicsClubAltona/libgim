//  Derived from boost::noncopyable
//
//  (C) Copyright Beman Dawes 1999-2003. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//  Contributed by Dave Abrahams

#ifndef __UTIL_NONCOPYABLE_HPP
#define __UTIL_NONCOPYABLE_HPP

class noncopyable
{
    protected:
      noncopyable()  {}
      ~noncopyable() {}

    private:  // emphasize the following members are private
      noncopyable( const noncopyable& );
      const noncopyable& operator=( const noncopyable& );
};

#endif
