AM_CPPFLAGS =                   \
        $(COMMON_CXXFLAGS)      \
        -I$(top_srcdir)         \
        -I$(top_srcdir)/include \
        -I$(top_srcdir)/src/    \
        -I$(top_srcdir)/lib/

AM_LDFLAGS  = $(COMMON_LDFLAGS)

TEST_BIN = backtrace float range maths matrix version
TESTS = $(TEST_BIN) json.pl
check_PROGRAMS = $(TEST_BIN) json-check
EXTRA_DIST = json.pl

backtrace_SOURCES  = backtrace.cpp
backtrace_CPPFLAGS = $(COMMON_CXXFLAGS)
backtrace_LDADD    = $(builddir)/../libutil.la $(BOOST_SYSTEM_LIB)

float_SOURCES      = float.cpp
float_LDADD        = $(builddir)/../libutil.la $(BOOST_SYSTEM_LIB)

range_SOURCES 	   = range.cpp
range_LDADD        = $(builddir)/../libutil.la $(BOOST_SYSTEM_LIB)

maths_SOURCES      = maths.cpp
maths_LDADD        = $(builddir)/../libutil.la $(BOOST_SYSTEM_LIB)

matrix_SOURCES     = matrix.cpp
matrix_LDADD       = $(builddir)/../libutil.la $(BOOST_SYSTEM_LIB)

version_SOURCES    = version.cpp
version_LDADD      = $(builddir)/../libutil.la $(BOOST_SYSTEM_LIB)

json_check_SOURCES = json-check.cpp
json_check_LDADD   = $(builddir)/../libutil.la $(BOOST_FILESYSTEM_LIB)
