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
	md4		\
	md5		\
	options		\
	pool		\
	range 		\
	region		\
	signal		\
	sha1		\
	version

AM_DEFAULT_SOURCE_EXT = .cpp
LDADD = $(builddir)/../libutil.la

TESTS = $(TEST_BIN) json.pl
check_PROGRAMS = $(TEST_BIN)
EXTRA_DIST = json.pl
