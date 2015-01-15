AM_LDFLAGS  = $(COMMON_LDFLAGS) $(BOOST_LDFLAGS) $(BOOST_FILESYSTEM_LIB) $(BOOST_SYSTEM_LIB)
AM_CXXFLAGS = $(COMMON_CXXFLAGS) -I$(top_srcdir)

TEST_BIN = 		\
	backtrace 	\
	bitwise		\
	checksum	\
	colour		\
	float		\
	hton		\
	ip 		\
	json/types	\
	maths/maths 	\
	maths/matrix 	\
	matrix		\
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
	sha1		\
	sha2		\
	signal		\
	stringid	\
	version

AM_DEFAULT_SOURCE_EXT = .cpp
LDADD = $(builddir)/../libutil.a

if PLATFORM_WIN32
hton_LDFLAGS = -lws2_32
endif

TEST_LOG_DRIVER = env AM_TAP_AWK='$(AWK)' $(SHELL) $(top_srcdir)/build-aux/tap-driver.sh
TESTS = static.test json.test
check_PROGRAMS = $(TEST_BIN)
EXTRA_DIST = static.test json.test json
