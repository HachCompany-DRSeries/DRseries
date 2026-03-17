/* This is a generated file, see Makefile.am for its inputs. */
static const char type_strings[] = "a0\0a1\0a2\0a3\0acct\0addr\0arch\0auid\0cap_fi\0cap_fp\0"
	"cap_pe\0cap_pi\0cap_pp\0capability\0cgroup\0cmd\0code\0comm\0cwd\0data\0"
	"device\0dir\0egid\0euid\0exe\0exit\0family\0fe\0fi\0file\0"
	"flags\0fp\0fsgid\0fsuid\0gid\0grp\0icmptype\0id\0igid\0img-ctx\0"
	"inode_gid\0inode_uid\0iuid\0key\0list\0mode\0name\0new-disk\0new-fs\0new-rng\0"
	"new_gid\0new_group\0new_pe\0new_pi\0new_pp\0oauid\0obj\0obj_gid\0obj_uid\0ocomm\0"
	"oflag\0ogid\0old-disk\0old-fs\0old-rng\0old_pe\0old_pi\0old_pp\0old_prom\0ouid\0"
	"path\0per\0perm\0perm_mask\0proctitle\0prom\0proto\0res\0result\0saddr\0"
	"sauid\0scontext\0ses\0sgid\0sig\0sigev_signo\0subj\0suid\0syscall\0tcontext\0"
	"uid\0vm\0vm-ctx\0watch";
static const unsigned type_s2i_s[] = {
	0,3,6,9,12,17,22,27,32,39,
	46,53,60,67,78,85,89,94,99,103,
	108,115,119,124,129,133,138,145,148,151,
	156,162,165,171,177,181,185,194,197,202,
	210,220,230,235,239,244,249,254,263,270,
	278,286,296,303,310,317,323,327,335,343,
	349,355,360,369,376,384,391,398,405,414,
	419,424,428,433,443,453,458,464,468,475,
	481,487,496,500,505,509,521,526,531,539,
	548,552,555,562,
};
static const int type_s2i_i[] = {
	14,15,16,17,6,26,4,1,22,22,
	22,22,22,12,6,6,28,6,6,20,
	6,6,2,1,6,5,23,22,22,6,
	30,22,2,1,2,6,24,1,2,32,
	2,1,1,6,19,8,6,6,6,6,
	2,6,22,22,22,1,32,2,1,6,
	29,2,6,6,6,22,22,22,11,1,
	6,27,7,7,33,11,25,13,13,9,
	1,32,21,2,18,18,32,1,3,32,
	1,6,32,6,
};
static int type_s2i(const char *s, int *value) {
	return s2i__(type_strings, type_s2i_s, type_s2i_i, 94, s, value);
}
