AM_LDFLAGS  = $(COMMON_LDFLAGS)
AM_CXXFLAGS = $(COMMON_CXXFLAGS) -I$(top_srcdir)

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
	point		\
	pool		\
	rand		\
	range 		\
	region		\
	ripemd		\
	signal		\
	sha1		\
	version

AM_DEFAULT_SOURCE_EXT = .cpp
LDADD = $(builddir)/../libutil.la

if PLATFORM_WIN32
hton_LDFLAGS = -lws2_32
endif

TESTS = $(TEST_BIN) json.pl
check_PROGRAMS = $(TEST_BIN)
EXTRA_DIST = json.pl
