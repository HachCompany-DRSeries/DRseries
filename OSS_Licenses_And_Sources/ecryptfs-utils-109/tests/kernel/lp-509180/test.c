/*
 * Author: Colin King <colin.king@canonical.com>
 *
 * Copyright (C) 2012 Canonical, Ltd.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA
 */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <attr/xattr.h>

#define TEST_ERROR	(2)

#define OFFSET		(9)

#define OPT_INC		(0x0001)
#define OPT_DEC		(0x0002)
#define OPT_XATTR	(0x0004)

void usage(char *name)
{
	fprintf(stderr, "Usage: [-i | -d] [-x] file\n");
}

static int test_with_metadata_in_header(int fd, int flags)
{
	unsigned char buffer[1];

	if ((lseek(fd, (off_t)OFFSET, SEEK_SET)) < 0) {
		fprintf(stderr, "Cannot lseek to offset %d: %s\n",
			OFFSET, strerror(errno));
		return TEST_ERROR;
	}

	if (read(fd, buffer, sizeof(buffer)) != sizeof(buffer)) {
		fprintf(stderr, "Failed to read\n");
		return TEST_ERROR;
	}

	if (flags & OPT_INC)
		buffer[0]++;

	if (flags & OPT_DEC)
		buffer[0]--;

	if ((lseek(fd, (off_t)OFFSET, SEEK_SET)) < 0) {
		fprintf(stderr, "Cannot lseek to offset %d: %s\n",
			OFFSET, strerror(errno));
		return TEST_ERROR;
	}

	if (write(fd, buffer, sizeof(buffer)) != sizeof(buffer)) {
		fprintf(stderr, "Failed to write\n");
		return TEST_ERROR;
	}

	return 0;
}

static int test_with_metadata_in_xattr(int fd, int flags)
{
	const char *name = "user.ecryptfs";
	unsigned char *value = NULL;
	ssize_t nread, size = 0;
	int rc = TEST_ERROR;

	size = fgetxattr(fd, name, value, size);
	if (size < 0) {
		fprintf(stderr, "Cannot retrieve xattr size: %s\n",
			strerror(errno));
		goto out;
	}

	value = malloc(size);
	if (!value) {
		fprintf(stderr,
			"Cannot allocate memory to store the xattr value\n");
		goto out;
	}

	nread = fgetxattr(fd, name, value, size);
	if (nread != size) {
		if (nread < 0)
			fprintf(stderr, "Cannot read xattr: %s\n",
				strerror(errno));
		else
			fprintf(stderr, "Partial xattr read: %zu < %zu\n",
				nread, size);
		goto out;
	}

	if (flags & OPT_INC)
		value[OFFSET]++;

	if (flags & OPT_DEC)
		value[OFFSET]--;

	if (fsetxattr(fd, name, value, size, XATTR_REPLACE) < 0) {
		fprintf(stderr, "Cannot write xattr: %s\n", strerror(errno));
		goto out;
	}

	rc = 0;
out:
	free(value);

	return rc;
}


/*
 *  https://bugs.launchpad.net/ecryptfs/+bug/509180
 *  Increment/Decrement 9th byte in lower file
 */
int main(int argc, char **argv)
{
	int fd;
	int opt, flags = 0;
	int rc = 0;
	char *file;

	if (argc < 3) {
		usage(argv[0]);
		exit(TEST_ERROR);
	}

	while ((opt = getopt(argc, argv, "idx")) != -1) {
		switch (opt) {
		case 'i':
			flags |= OPT_INC;
			break;
		case 'd':
			flags |= OPT_DEC;
			break;
		case 'x':
			flags |= OPT_XATTR;
			break;
		default:
			usage(argv[0]);
			exit(TEST_ERROR);
		}
	}

	if ((flags == 0) || (flags == (OPT_INC | OPT_DEC))) {
		fprintf(stderr, "Need to specify -i or -d\n");
		exit(TEST_ERROR);
	}

	file = argv[optind];

	if ((fd = open(file, O_RDWR, 0700)) < 0) {
		fprintf(stderr, "Cannot open %s : %s\n", file, strerror(errno));
		exit(TEST_ERROR);
	}

	if (flags & OPT_XATTR)
		rc = test_with_metadata_in_xattr(fd, flags);
	else
		rc = test_with_metadata_in_header(fd, flags);

tidy:
	if (close(fd) < 0) {
		fprintf(stderr, "Close failed: %s\n", strerror(errno));
		exit(TEST_ERROR);
	}

	exit(rc);
}
