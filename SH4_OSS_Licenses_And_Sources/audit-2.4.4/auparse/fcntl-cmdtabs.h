/* This is a generated file, see Makefile.am for its inputs. */
static const char fcntl_strings[] = "F_ADD_SEALS\0F_CANCELLK\0F_DUPFD\0F_DUPFD_CLOEXEC\0F_GETFD\0F_GETFL\0F_GETLEASE\0F_GETLK\0F_GETLK64\0F_GETOWN\0"
	"F_GETOWNER_UIDS\0F_GETOWN_EX\0F_GETPIPE_SZ\0F_GETSIG\0F_GET_SEALS\0F_NOTIFY\0F_SETFD\0F_SETFL\0F_SETLEASE\0F_SETLK\0"
	"F_SETLK64\0F_SETLKW\0F_SETLKW64\0F_SETOWN\0F_SETOWN_EX\0F_SETPIPE_SZ\0F_SETSIG";
static const int fcntl_i2s_i[] = {
	0,1,2,3,4,5,6,7,8,9,
	10,11,12,13,14,15,16,17,1024,1025,
	1026,1029,1030,1031,1032,1033,1034,
};
static const unsigned fcntl_i2s_s[] = {
	23,47,172,55,180,74,199,217,237,92,
	271,142,82,207,226,246,117,101,188,63,
	163,12,31,258,129,0,151,
};
static const char *fcntl_i2s(int v) {
	return i2s_bsearch__(fcntl_strings, fcntl_i2s_i, fcntl_i2s_s, 27, v);
}
