AM_CPPFLAGS =                   \
        $(COMMON_CXXFLAGS)      \
        -I$(top_srcdir)         \
        -I$(top_srcdir)/include \
        -I$(top_srcdir)/src/    \
        -I$(top_srcdir)/lib/

AM_LDFLAGS  = $(COMMON_LDFLAGS)

TEST_BIN = 		\
	backtrace 	\
	float		\
	hton		\
	ip 		\
	maths 		\
	matrix 		\
	range 		\
	signal		\
	version

TESTS = $(TEST_BIN) json.pl
check_PROGRAMS = $(TEST_BIN) json-check
EXTRA_DIST = json.pl

backtrace_CPPFLAGS = $(COMMON_CXXFLAGS)
backtrace_LDADD    = $(builddir)/../libutil.la
backtrace_SOURCES  = backtrace.cpp

float_LDADD        = $(builddir)/../libutil.la
float_SOURCES      = float.cpp

hton_LDADD	   = $(builddir)/../libutil.la
hton_SOURCES	   = hton.cpp

ip_LDADD      	   = $(builddir)/../libutil.la
ip_SOURCES         = ip.cpp

json_check_LDADD   = $(builddir)/../libutil.la $(BOOST_FILESYSTEM_LIB)
json_check_SOURCES = json-check.cpp

maths_LDADD        = $(builddir)/../libutil.la
maths_SOURCES      = maths.cpp

matrix_LDADD       = $(builddir)/../libutil.la
matrix_SOURCES     = matrix.cpp

range_LDADD        = $(builddir)/../libutil.la
range_SOURCES 	   = range.cpp

signal_LDADD	   = $(builddir)/../libutil.la
signal_SOURCES     = signal.cpp

version_LDADD      = $(builddir)/../libutil.la
version_SOURCES    = version.cpp
