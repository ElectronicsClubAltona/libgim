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
	rand		\
	range 		\
	region		\
	signal		\
	sha1		\
	version

AM_DEFAULT_SOURCE_EXT = .cpp
LDADD = $(builddir)/../libutil.la

hton_LDFLAGS = -lws2_32
ip_LDFLAGS   = -lws2_32

TESTS = $(TEST_BIN) json.pl
check_PROGRAMS = $(TEST_BIN)
EXTRA_DIST = json.pl
