AM_CPPFLAGS =                   \
        $(COMMON_CXXFLAGS)      \
        -I$(top_srcdir)         \
        -I$(top_srcdir)/include \
        -I$(top_srcdir)/src/    \
        -I$(top_srcdir)/lib/

AM_LDFLAGS  = $(COMMON_LDFLAGS)

TEST_BIN = 		\
	backtrace 	\
	bitwise		\
	checksum	\
	float		\
	hton		\
	ip 		\
	json		\
	maths 		\
	matrix 		\
	md2		\
	option		\
	pool		\
	range 		\
	region		\
	signal		\
	version

TESTS = $(TEST_BIN) json.pl
check_PROGRAMS = $(TEST_BIN)
EXTRA_DIST = json.pl

backtrace_CPPFLAGS = $(COMMON_CXXFLAGS)
backtrace_LDADD    = $(builddir)/../libutil.la
backtrace_SOURCES  = backtrace.cpp

bitwise_LDADD      = $(builddir)/../libutil.la
bitwise_SOURCES    = bitwise.cpp

checksum_LDADD	   = $(builddir)/../libutil.la
checksum_SOURCES   = checksum.cpp

float_LDADD        = $(builddir)/../libutil.la
float_SOURCES      = float.cpp

hton_LDADD	   = $(builddir)/../libutil.la
hton_SOURCES	   = hton.cpp

ip_LDADD      	   = $(builddir)/../libutil.la
ip_SOURCES         = ip.cpp

json_LDADD   	   = $(builddir)/../libutil.la
json_SOURCES       = json.cpp

maths_LDADD        = $(builddir)/../libutil.la
maths_SOURCES      = maths.cpp

matrix_LDADD       = $(builddir)/../libutil.la
matrix_SOURCES     = matrix.cpp

md2_LDADD          = $(builddir)/../libutil.la
md2_SOURCES        = md2.cpp

option_LDADD	   = $(builddir)/../libutil.la
option_SOURCES     = options/success.cpp

pool_LDADD         = $(builddir)/../libutil.la
pool_SOURCES	   = pool.cpp

range_LDADD        = $(builddir)/../libutil.la
range_SOURCES 	   = range.cpp

region_LDADD       = $(builddir)/../libutil.la
region_SOURCES     = region.cpp

signal_LDADD	   = $(builddir)/../libutil.la
signal_SOURCES     = signal.cpp

version_LDADD      = $(builddir)/../libutil.la
version_SOURCES    = version.cpp
