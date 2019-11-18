/*
 * This is an ecryptfs private directory mount/unmount helper program
 * for non-root users.
 *
 * Copyright (C) 2008 Canonical Ltd.
 *
 * This code was originally written by Dustin Kirkland <kirkland@ubuntu.com>.
 * Enhanced by Serge Hallyn <hallyn@ubuntu.com>.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, see <http://www.gnu.org/licenses/>.
 *
 * On Debian-based systems, the complete text of the GNU General Public
 * License can be found in /usr/share/common-licenses/GPL-2
 *
 */

#include <sys/file.h>
#include <sys/mount.h>
#include <sys/param.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/vfs.h>
#include <fcntl.h>
#include <ctype.h>
#include <errno.h>
#include <keyutils.h>
#include <mntent.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <syslog.h>
#include <values.h>
#include "../include/ecryptfs.h"

/* Perhaps a future version of this program will allow these to be configurable
 * by the system administrator (or user?) at run time.  For now, these are set
 * to reasonable values to reduce the burden of input validation.
 */
#define KEY_BYTES 16
#define KEY_CIPHER "aes"
#define FSTYPE "ecryptfs"
#define TMP "/dev/shm"

int read_config(char *pw_dir, uid_t uid, char *alias, char **s, char **d, char **o) {
/* Read an fstab(5) style config file */
	char *fnam;
	struct stat mstat;
	struct mntent *e;
	FILE *fin;
	if (asprintf(&fnam, "%s/.ecryptfs/%s.conf", pw_dir, alias) < 0) {
		perror("asprintf");
		return -1;
	}
	if (stat(fnam, &mstat)!=0 || (!S_ISREG(mstat.st_mode) || mstat.st_uid!=uid)) {
		fputs("Bad file\n", stderr);
		free(fnam);
		return -1;
	}
	fin = fopen(fnam, "r");
	free(fnam);
	if (!fin) {
		perror("fopen");
		return -1;
	}
	e = getmntent(fin);
	fclose(fin);
	if (!e) {
		perror("getmntent");
		return -1;
	}
	if (strcmp(e->mnt_type, "ecryptfs") != 0) {
		fputs("Bad fs type\n", stderr);
		return -1;
	}
	*o = strdup(e->mnt_opts);
	if (!*o)
		return -2;
	*d = strdup(e->mnt_dir);
	if (!*d)
		return -2;
	*s = strdup(e->mnt_fsname);
	if (!*s)
		return -2;

	return 0;
}

int check_username(char *u) {
/* We follow the username guidelines used by the adduser program.  Quoting its
 * error message:
 *   adduser: To avoid problems, the username should consist only of
 *   letters, digits, underscores, periods, at signs and dashes, and not start
 *   with a dash (as defined by IEEE Std 1003.1-2001). For compatibility with
 *   Samba machine accounts $ is also supported at the end of the username
 */
	int i;
	char c;
	int len;

	if (u == NULL)
		goto empty;
	len = strlen(u);
	if (len == 0)
		goto empty;

	for (i=0; i<len; i++) {
		c = u[i];
		if ( 	!(c>='a' && c<='z') && !(c>='A' && c<='Z') &&
			!(c>='0' && c<='9') &&
			!(c=='_') && !(c=='.') && !(c=='@') &&
			!(c=='-' && i!=0) &&
			!(c=='$' && i==(len-1))
		) {
			fputs("Username has unsupported characters\n", stderr);
			return 1;
		}
	}
	return 0;
empty:
	fputs("Username is empty\n", stderr);
	return 1;
}

char **fetch_sig(char *pw_dir, char *alias, int mounting) {
/* Read ecryptfs signature from file and validate
 * Return signature as a string, or NULL on failure
 */
	char *sig_file;
	FILE *fh = NULL;
	char **sig = NULL;
	unsigned int i, j;
	/* Construct sig file name */
	if (asprintf(&sig_file, "%s/.ecryptfs/%s.sig", pw_dir, alias) < 0) {
		perror("asprintf");
		goto err;
	}
	fh = fopen(sig_file, "r");
	if (fh == NULL) {
		perror("fopen");
		goto err;
	}
	/* Read up to 2 lines from the file */
	if ((sig = malloc(sizeof(char*) * 2)) == NULL) {
		perror("malloc");
		goto err;
	}

	sig[0] = NULL;
	sig[1] = NULL;
	for (i=0; i<2; i++) {
		if ((sig[i] = (char *)malloc(KEY_BYTES*sizeof(char)+2)) == NULL) {
			perror("malloc");
			goto err;
		}
		memset(sig[i], '\0', KEY_BYTES+2);
		/* Read KEY_BYTES characters from line */
		if (fgets(sig[i], KEY_BYTES+2, fh) == NULL) {
			if (i == 0) {
				fputs("Missing file encryption signature", stderr);
				goto err;
			} else {
				/* Filename encryption isn't in use */
				free(sig[i]);
				sig[i] = NULL;
				goto out;
			}
		}
		/* Validate hex signature */
		for (j=0; j<strlen(sig[i]); j++) {
			if (isxdigit(sig[i][j]) == 0 && isspace(sig[i][j]) == 0) {
				fputs("Invalid hex signature\n", stderr);
				goto err;
			}
			if (isspace(sig[i][j]) != 0) {
				/* truncate at first whitespace */
				sig[i][j] = '\0';
			}
		}
		if (strlen(sig[i]) > 0 && strlen(sig[i]) != KEY_BYTES) {
			fputs("Invalid hex signature length\n", stderr);
			goto err;
		}
		/* Validate that signature is in the current keyring,
		 * compile with -lkeyutils
		 */
		if (keyctl_search(KEY_SPEC_USER_KEYRING, "user", sig[i], 0) < 0) {
			if (mounting)
				fputs("Signature not found in user keyring\n"
				      "Perhaps try the interactive "
				      "'ecryptfs-mount-private'\n", stderr);
			goto err;
		}
	}
out:
	if (fh != NULL) {
		fclose(fh);
	}
	return sig;
err:
	if (fh) {
		fclose(fh);
	}
	/* Clean up malloc'd memory if failure */
	if (sig) {
		free(sig[0]);
		free(sig[1]);
		free(sig);
	}
	return NULL;
}

static int check_cwd_f_type()
{
	struct statfs buf;
	/**
	 * This is *not* a list of compatible lower filesystems list for
	 * eCryptfs. This is a list of filesystems that we reasonably expect to
	 * see mount.ecryptfs_private users mounting on top of. In other words,
	 * the filesystem type of the 'target' parameter of mount(2).
	 *
	 * This whitelist is to prevent malicious mount.ecryptfs_private users
	 * from mounting over filesystem types such as PROC_SUPER_MAGIC to
	 * deceive other programs with a crafted /proc/self/*. See
	 * https://launchpad.net/bugs/1530566 for more details.
	 */
	typeof(buf.f_type) f_type_whitelist[] = {
		0x61756673 /* AUFS_SUPER_MAGIC */,
		0x9123683E /* BTRFS_SUPER_MAGIC */,
		0x00C36400 /* CEPH_SUPER_MAGIC */,
		0xFF534D42 /* CIFS_MAGIC_NUMBER */,
		0x0000F15F /* ECRYPTFS_SUPER_MAGIC */,
		0x0000EF53 /* EXT[234]_SUPER_MAGIC */,
		0xF2F52010 /* F2FS_SUPER_MAGIC */,
		0x65735546 /* FUSE_SUPER_MAGIC */,
		0x01161970 /* GFS2_MAGIC */,
		0x3153464A /* JFS_SUPER_MAGIC */,
		0x000072B6 /* JFFS2_SUPER_MAGIC */,
		0x0000564C /* NCP_SUPER_MAGIC */,
		0x00006969 /* NFS_SUPER_MAGIC */,
		0x00003434 /* NILFS_SUPER_MAGIC */,
		0x5346544E /* NTFS_SB_MAGIC */,
		0x794C7630 /* OVERLAYFS_SUPER_MAGIC */,
		0x52654973 /* REISERFS_SUPER_MAGIC */,
		0x73717368 /* SQUASHFS_MAGIC */,
		0x01021994 /* TMPFS_MAGIC */,
		0x24051905 /* UBIFS_SUPER_MAGIC */,
		0x58465342 /* XFS_SB_MAGIC */,
		0x2FC12FC1 /* ZFS_SUPER_MAGIC */,
	};
	size_t i, whitelist_len;

	if (statfs(".", &buf) != 0) {
		fprintf(stderr, "Failed to check filesystem type: %m\n");
		return 1;
	}

	whitelist_len = sizeof(f_type_whitelist) / sizeof(*f_type_whitelist);
	for (i = 0; i < whitelist_len; i++) {
		if (buf.f_type == f_type_whitelist[i]) {
			return 0;
		}
	}

	fprintf(stderr,
		"Refusing to mount over an unapproved filesystem type: %#lx\n",
		buf.f_type);
	return 1;
}

int check_ownership_mnt(uid_t uid, char **mnt) {
/* Check ownership of mount point, chdir into it, and
 * canonicalize the path for use in mtab updating.
 * Return 0 if everything is in order, 1 on error.
 */
	struct stat s;
	char *cwd;

	/* From here on, we'll refer to "." as our mountpoint, to avoid
	 * races.
	 */
	if (chdir(*mnt) != 0) {
		fputs("Cannot chdir into mountpoint.\n", stderr);
		return 1;
	}
	if (stat(".", &s) != 0) {
		fputs("Cannot examine mountpoint.\n", stderr);
		return 1;
	}
	if (!S_ISDIR(s.st_mode)) {
		fputs("Mountpoint is not a directory.\n", stderr);
		return 1;
	}
	if (s.st_uid != uid) {
		fputs("You do not own that mountpoint.\n", stderr);
		return 1;
	}

	/* Canonicalize our pathname based on the current directory to
	 * avoid races.
	 */
	cwd = getcwd(NULL, 0);
	if (!cwd) {
		fputs("Failed to get current directory\n", stderr);
		return 1;
	}
	*mnt = cwd;
	return 0;
}


int check_ownerships(uid_t uid, char *path) {
/* Check ownership of device and mount point.
 * Return 0 if everything is in order, 1 on error.
 */
	struct stat s;
	if (stat(path, &s) != 0) {
		fputs("Cannot examine encrypted directory\n", stderr);
		return 1;
	}
	if (!S_ISDIR(s.st_mode)) {
		fputs("Device or mountpoint is not a directory\n", stderr);
		return 1;
	}
	if (s.st_uid != uid) {
		fputs("You do not own that encrypted directory\n", stderr);
		return 1;
	}
	return 0;
}


int update_mtab(char *dev, char *mnt, char *opt) {
/* Update /etc/mtab with new mount entry unless it is a symbolic link
 * Return 0 on success, 1 on failure.
 */
	char dummy;
	int useMtab;
	/* Check if mtab is a symlink */
	useMtab = (readlink("/etc/mtab", &dummy, 1) < 0);
	if (!useMtab) {
		/* No need updating mtab */
		return 0;
	}

	int fd;
	FILE *old_mtab, *new_mtab;
	struct mntent *old_ent, new_ent;
	mode_t old_umask;

	/* Make an attempt to play nice with other mount helpers
	 * by creating an /etc/mtab~ lock file. Of course this
	 * only works if those other helpers actually check for
	 * this.
	 */
	old_umask = umask(033);
	fd = open("/etc/mtab~", O_RDONLY | O_CREAT | O_EXCL, 0644);
	if (fd < 0) {
		perror("open");
		return 1;
	}
	close(fd);

	old_mtab = setmntent("/etc/mtab", "r");
	if (old_mtab == NULL) {
		perror("setmntent");
		return 1;
	}

	new_mtab = setmntent("/etc/mtab.tmp", "w");
	if (new_mtab == NULL) {
		perror("setmntent");
		goto fail_early;
	}

	while ((old_ent = getmntent(old_mtab))) {
		if (addmntent(new_mtab, old_ent) != 0) {
			perror("addmntent");
			goto fail;
		}
	}
	endmntent(old_mtab);

	new_ent.mnt_fsname = dev;
	new_ent.mnt_dir = mnt;
	new_ent.mnt_type = FSTYPE;
	new_ent.mnt_opts = opt;
	new_ent.mnt_freq = 0;
	new_ent.mnt_passno = 0;

	if (addmntent(new_mtab, &new_ent) != 0) {
		perror("addmntent");
		goto fail;
	}

	if (fchmod(fileno(new_mtab), S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH) < 0) {
		perror("fchmod");
		goto fail;
	}
	endmntent(new_mtab);

	if (rename("/etc/mtab.tmp", "/etc/mtab") < 0) {
		perror("rename");
		goto fail_late;
	}

	unlink("/etc/mtab~");

	umask(old_umask);

	return 0;

fail:
	endmntent(new_mtab);
fail_late:
	unlink("/etc/mtab.tmp");
fail_early:
	endmntent(old_mtab);
	unlink("/etc/mtab~");
	umask(old_umask);
	return 1;
}

FILE *lock_counter(char *u, uid_t uid, char *alias) {
	char *f;
	int fd;
	FILE *fh;
	struct stat s;
	int i = 1;
	/* We expect TMP to exist, be writeable by the user,
	 * and to be cleared on boot */
	if (asprintf(&f, "%s/%s-%s-%s", TMP, FSTYPE, u, alias) < 0) {
		perror("asprintf");
		return NULL;
	}
	/* If the counter path exists, and it's either not a regular
	 * file, or it's not owned by the current user, append iterator
	 * until we find a filename we can use.
	 */
	while (i < 50) {
		if (((fd = open(f, O_RDWR | O_CREAT | O_NOFOLLOW, 0600)) >= 0) &&
		    (fstat(fd, &s)==0 && (S_ISREG(s.st_mode) && s.st_uid==uid))) {
			break;
		} else {
			if (fd >= 0) {
				close(fd);
				fd = -1;
			}
			free(f);
			if (asprintf(&f, "%s/%s-%s-%s-%d", TMP, FSTYPE, u,
			    alias, i++) < 0) {
				perror("asprintf");
				return NULL;
			}
		}
	}

	if (fd < 0) {
		perror("open");
		return NULL;
	}

	flock(fd, LOCK_EX);
	fh = fdopen(fd, "r+");
	if (fh == NULL) {
		perror("fopen");
		close(fd);
		return NULL;
	}
	return fh;
}

void unlock_counter(FILE *fh) {
	if (fh != NULL) {
		/* This should remove the lock too */
		fclose(fh);
	}
}

int bump_counter(FILE *fh, int delta) {
/* Maintain a mount counter
 *   increment on delta = 1
 *   decrement on delta = -1
 *   remove the counter file on delta = 0
 *   return the updated count, negative on error
 */
	int count;
	/* Read the count from file, default to 0 */
	rewind(fh);
	if (fscanf(fh, "%d\n", &count) != 1) {
		count = 0;
	}
	/* Increment/decrement the counter */
	count += delta;
	if (count < 0) {
		/* Never set a count less than 0 */
		count = 0;
	}
	/* Write the count to file */
	rewind(fh);
	fprintf(fh, "%d\n", count);
	fflush(fh);
	return count;
}


int increment(FILE *fh) {
/* Bump counter up */
	return bump_counter(fh, 1);
}


int decrement(FILE *fh) {
/* Bump counter down */
	return bump_counter(fh, -1);
}

int zero(FILE *fh) {
/* Zero the counter file */
	return bump_counter(fh, -MAXINT+1);
}

/* Returns -1 on error, 0 on success, and 1 if the program should exit with 0 */
static int parse_options(int argc, char *argv[], int *mounting, int *force,
			 int *nonzero_decrement_is_error, char **alias)
{
	const char *optstr, *usagestr;
	int opt, usage = 0, rc = -1;

	*force = 0;
	*nonzero_decrement_is_error = 1;
	*alias = NULL;

	/* Determine if mounting or unmounting by looking at the invocation */
	if (strstr(argv[0], "umount") == NULL) {
		*mounting = 1;
		optstr = "h";
		usagestr = "[ALIAS]\n"
			   "Mount the default private directory or ALIAS, if specified.\n"
			   "\n"
			   "  -h        display this help and exit\n";
	} else {
		*mounting = 0;
		optstr = "hfd";
		usagestr = "[-f] [-d] [ALIAS]\n"
			   "Unmount the default private directory or ALIAS, if specified.\n"
			   "\n"
			   "  -h        display this help and exit\n"
			   "  -f        forcibly unmount\n"
			   "  -d        don't treat a non-zero session counter as an error\n";
	}

	while ((opt = getopt(argc, argv, optstr)) != -1) {
		switch (opt) {
			case 'h':
				rc = 1;
				usage = 1;
				goto out;
			case 'f':
				*force = 1;
				break;
			case 'd':
				*nonzero_decrement_is_error = 0;
				break;
			default:
				usage = 1;
				goto out;
		}
	}

	if (optind < (argc - 1)) {
		usage = 1;
		goto out;
	} else if (optind == (argc - 1)) {
		*alias = argv[optind];
	}

	rc = 0;
out:
	if (usage)
		fprintf(stderr, "Usage: %s %s", argv[0], usagestr);
	return rc;
}

/* This program is a setuid-executable allowing a non-privileged user to mount
 * and unmount an ecryptfs private directory.  This program is necessary to
 * keep from adding such entries to /etc/fstab.
 *
 * A single executable is created and hardlinked to two different names.
 * The mode of operation (mounting|unmounting) is determined by examining
 * the name of the executable.  "Mounting" mode is assumed, unless the
 * executable contains the string "umount".
 * Example:
 *   /sbin/mount.ecryptfs_private
 *   /sbin/umount.ecryptfs_private
 *
 * At the moment, this program:
 *  - mounts ~/.Private onto ~/Private
 *    - as an ecryptfs filesystem
 *    - using the AES cipher
 *    - with a key length of 16 bytes
 *    - and using the signature defined in ~/.ecryptfs/Private.sig
 *    - ONLY IF the user
 *      - has the signature's key in his keyring
 *      - owns both ~/.Private and ~/Private
 *      - is not already mounted
 *  - unmounts ~/.Private from ~/Private
 *    - using the signature defined in ~/.ecryptfs/Private.sig
 *    - ONLY IF the user
 *      - owns both ~/.Private and ~/Private
 *      - is currently ecryptfs mounted
 *
 * The only setuid operations in this program are:
 *  a) mounting
 *  b) unmounting
 *  c) updating /etc/mtab
 */
int main(int argc, char *argv[]) {
	uid_t uid;
	gid_t gid;
	int mounting, force, nonzero_decrement_is_error;
	struct passwd *pwd;
	char *alias, *src, *dest, *opt, *opts2;
	char *sig_fekek = NULL, *sig_fnek = NULL;
	char **sigs;
	FILE *fh_counter = NULL;

	uid = getuid();
	gid = getgid();
	/* Non-privileged effective uid is sufficient for all but the code
 	 * that mounts, unmounts, and updates /etc/mtab.
	 * Run at a lower privilege until we need it.
	 */
	if (seteuid(uid)<0 || geteuid()!=uid) {
		perror("setuid");
		goto fail;
	}
	if ((pwd = getpwuid(uid)) == NULL) {
		perror("getpwuid");
		goto fail;
	}

	switch (parse_options(argc, argv, &mounting, &force,
			      &nonzero_decrement_is_error, &alias)) {
		case -1:
			goto fail;
		case 1:
			goto success;
		default:
			break; /* proceed */
	}

	/* If no arguments, default to private dir; but accept at most one
	   argument, an alias for the configuration to read and use.
	 */
	if (alias == NULL) {
		/* Use default source and destination dirs */
		alias = ECRYPTFS_PRIVATE_DIR;
		if ((asprintf(&src, "%s/.%s", pwd->pw_dir, alias) < 0) || src == NULL) {
			perror("asprintf (src)");
			goto fail;
		}
		dest = ecryptfs_fetch_private_mnt(pwd->pw_dir);
		if (dest == NULL) {
			perror("asprintf (dest)");
			goto fail;
		}
	} else {
		/* Read the source and destination dirs from .conf file */
		if (read_config(pwd->pw_dir, uid, alias, &src, &dest, &opts2) < 0) {
			fputs("Error reading configuration file\n", stderr);
			exit(1);
		}
		if (opts2 != NULL && strlen(opts2) != 0 && strcmp(opts2, "none") != 0) {
			fputs("Mount options are not supported here\n", stderr);
			exit(1);
		}
	}

	if (strstr(alias, "..")) {
		fputs("Invalid alias", stderr);
		exit(1);
	}

	/* Lock the counter through the rest of the program */
	fh_counter = lock_counter(pwd->pw_name, uid, alias);
	if (fh_counter == NULL) {
		fputs("Error locking counter\n", stderr);
		goto fail;
	}

	if (check_username(pwd->pw_name) != 0) {
		/* Must protect against a crafted user=john,suid from entering
		 * filesystem options
		 */
		goto fail;
	}

	/* Fetch signatures from file */
	/* First line is the file content encryption key signature */
	/* Second line, if present, is the filename encryption key signature */
	sigs = fetch_sig(pwd->pw_dir, alias, mounting);
	if (!sigs && mounting) {
		/* if umounting, no sig is ok */
		goto fail;
	} else if (sigs) {
		sig_fekek = sigs[0];
		sig_fnek = sigs[1];
	}

	/* Build mount options */
	if (
	    (asprintf(&opt, "ecryptfs_check_dev_ruid,ecryptfs_cipher=%s,ecryptfs_key_bytes=%d,ecryptfs_unlink_sigs%s%s%s%s",
		      KEY_CIPHER,
		      KEY_BYTES,
		      sig_fekek ? ",ecryptfs_sig=" : "",
		      sig_fekek ? sig_fekek : "",
		      sig_fnek ? ",ecryptfs_fnek_sig=" : "",
		      sig_fnek ? sig_fnek : ""
		     ) < 0
	    ) || opt == NULL
	   ) {
		perror("asprintf (opt)");
		goto fail;
	}

	/* Check ownership of the mountpoint. From here on, dest refers
	 * to a canonicalized path, and the mountpoint is the cwd. */
	if (check_ownership_mnt(uid, &dest) != 0) {
 		goto fail;
 	}

	if (check_cwd_f_type() != 0) {
		goto fail;
	}

	if (mounting == 1) {
		/* Increment mount counter, errors non-fatal */
		if (increment(fh_counter) < 0) {
			fputs("Error incrementing mount counter\n", stderr);
		}
		/* Mounting, so exit if already mounted */
		if (ecryptfs_private_is_mounted(src, dest, sig_fekek, mounting) == 1) {
			goto success;
		}
		/* We must maintain our real uid as the user who called this
 		 * program in order to have access to their kernel keyring.
		 * Even though root has the power to mount, only a user with
		 * the correct key in their keyring can mount an ecryptfs
		 * directory correctly.
		 * Root does not necessarily have the user's key, so we need
		 * the real uid to be that of the user.
		 * And we need the effective uid to be root in order to mount.
		 */
		if (setreuid(-1, 0) < 0) {
			perror("setreuid");
			goto fail;
		}
		if (setregid(-1, 0) < 0) {
			perror("setregid");
			goto fail;
		}
 		/* Perform mount */
		if (mount(src, ".", FSTYPE, MS_NOSUID | MS_NODEV, opt) == 0) {
			if (update_mtab(src, dest, opt) != 0) {
				goto fail;
			}
		} else {
			perror("mount");
			/* Drop privileges since the mount did not succeed */
			if (setreuid(uid, uid) < 0) {
				perror("setreuid");
			}
			if (setregid(gid, gid) < 0) {
				perror("setregid");
			}
			goto fail;
		}
	} else {
		int rc = 0;
		/* Decrement counter, exiting if >0, and non-forced unmount */
		if (force == 1) {
			zero(fh_counter);
		} else if (decrement(fh_counter) > 0) {
			if (!nonzero_decrement_is_error)
				goto success;
			fputs("Sessions still open, not unmounting\n", stderr);
			goto fail;
		}
		/* Attempt to clear the user's keys from the keyring,
                   to prevent root from mounting without the user's key.
                   This is a best-effort basis, so we'll just print messages
                   on error. */
		if (sig_fekek != NULL) {
			rc = ecryptfs_remove_auth_tok_from_keyring(sig_fekek);
			if (rc != 0 && rc != ENOKEY)
				fputs("Could not remove key from keyring, try 'ecryptfs-umount-private'\n", stderr);
		}
		if (sig_fnek != NULL) {
			rc = ecryptfs_remove_auth_tok_from_keyring(sig_fnek);
			if (rc != 0 && rc != ENOKEY)
				fputs("Could not remove key from keyring, try 'ecryptfs-umount-private'\n", stderr);
		}
		/* Unmounting, so exit if not mounted */
		if (ecryptfs_private_is_mounted(src, dest, sig_fekek, mounting) == 0) {
			goto fail;
		}
		/* The key is not needed for unmounting, so we set res=0.
		 * Perform umount by calling umount utility.  This execl will
 		 * update mtab for us, and replace the current process.
		 * Do not use the umount.ecryptfs helper (-i).
 		 */
		setresuid(0,0,0);
		setresgid(0,0,0);
		clearenv();

		/* Since we're doing a lazy unmount anyway, just unmount the current
		 * directory. This avoids a lot of complexity in dealing with race
		 * conditions, and guarantees that we're only unmounting a filesystem
		 * that we own. */
		execl("/bin/umount", "umount", "-i", "-l", ".", NULL);
		perror("execl unmount failed");
		goto fail;
	}
success:
	unlock_counter(fh_counter);
	return 0;
fail:
	unlock_counter(fh_counter);
	return 1;
}
