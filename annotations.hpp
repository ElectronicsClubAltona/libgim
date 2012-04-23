/*
 * This file is part of libgim.
 *
 * libgim is free software: you can redistribute it and/or modify it under the
 * terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later
 * version.
 * 
 * ligim is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with libgim.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Copyright 2010-2012 Danny Robson <danny@nerdcruft.net>
 */


#ifndef __ANNOTATIONS_HPP
#define __ANNOTATIONS_HPP

// Don't use the name 'noreturn' as it interferes with other headers which
// may use __attribute__((noreturn)) explicitly.
#define terminal __attribute__ ((noreturn)) 

#if GCC_VERSION >= 40601
#define nonnull  __attribute__ ((nonnull))
#endif

#define mustuse  __attribute__ ((warn_unused_result))

#if GCC_VERSION >= 40601
#define unused   __attribute__ ((unused))
#endif

#define pure     __attribute__ ((pure))

#define   likely(X) (__builtin_expect((X), 1))
#define unlikely(X) (__builtin_expect((X), 0))

#endif // __ANNOTATIONS_HPP

