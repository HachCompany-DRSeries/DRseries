AUTOMAKE_OPTIONS = subdir-objects

# Only place tests worth of 'make check' here. All other tests are noinst.
dist_check_SCRIPTS = lfs.sh verify-passphrase-sig.sh wrap-unwrap.sh v1-to-v2-wrapped-passphrase.sh
check_PROGRAMS = lfs/test verify-passphrase-sig/test wrap-unwrap/test v1-to-v2-wrapped-passphrase/test

dist_noinst_DATA = tests.rc \
		   v1-to-v2-wrapped-passphrase/wp01 \
		   v1-to-v2-wrapped-passphrase/wp02 \
		   v1-to-v2-wrapped-passphrase/wp03 \
		   v1-to-v2-wrapped-passphrase/wp04 \
		   v1-to-v2-wrapped-passphrase/wp05

dist_noinst_SCRIPTS = $(dist_check_SCRIPTS)

if ENABLE_TESTS
noinst_PROGRAMS = $(check_PROGRAMS)
endif

lfs_test_SOURCES = lfs/test.c

verify_passphrase_sig_test_SOURCES = verify-passphrase-sig/test.c
verify_passphrase_sig_test_LDADD = $(top_builddir)/src/libecryptfs/libecryptfs.la

wrap_unwrap_test_SOURCES = wrap-unwrap/test.c
wrap_unwrap_test_LDADD = $(top_builddir)/src/libecryptfs/libecryptfs.la

v1_to_v2_wrapped_passphrase_test_SOURCES = v1-to-v2-wrapped-passphrase/test.c
v1_to_v2_wrapped_passphrase_test_LDADD = $(top_builddir)/src/libecryptfs/libecryptfs.la

TESTS = lfs.sh verify-passphrase-sig.sh wrap-unwrap.sh v1-to-v2-wrapped-passphrase.sh

