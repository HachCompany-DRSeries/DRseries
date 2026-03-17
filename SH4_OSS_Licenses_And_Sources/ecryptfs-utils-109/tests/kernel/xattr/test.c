/*
 * Author: Colin King <colin.king@canonical.com>
 *
 * Copyright (C) 2013 Canonical, Ltd.
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

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/xattr.h>

#define XATTR_METADATA_NAME	"user.ecryptfs"

static const char *names[]  = {
	"user.test1",
	"user.test2",
	"user.test3",
	NULL
};

static const char *values[] = {
	"test value #1",
	"test value #2",
	"test value #3",
	NULL
};

static void usage(const char *name)
{
	fprintf(stderr, "Usage: %s [-x] file\n", name);
}

int main(int argc, char **argv)
{
	ssize_t len, names_len = 0;
	int i, rc, xattr_metadata = 0;
	char buffer[1024];
	char *file, *ptr = buffer;

	if (argc < 2 || argc > 3) {
		usage(argv[0]);
		exit(EXIT_FAILURE);
	}

	file = argv[1];

	if (argc == 3) {
		if (strcmp(argv[1], "-x")) {
			usage(argv[0]);
			exit(EXIT_FAILURE);
		}
		file = argv[2];

		/*
		 * The XATTR_METADATA_NAME xattr is set. Account for it in
		 * future sanity checks.
		 */
		xattr_metadata = 1;
		names_len = 1 + strlen(XATTR_METADATA_NAME);
	}

	for (i = 0; names[i]; i++) {
		if (setxattr(file, names[i], values[i], strlen(values[i]), 0) < 0)
			exit(EXIT_FAILURE);
		names_len += 1 + strlen(names[i]);
	}

	/*
	 *  Sanity check that listxattr returns correct length
	 */
	len = listxattr(file, NULL, 0);
	if (len != names_len || len > sizeof(buffer))
		exit(EXIT_FAILURE);

	memset(buffer, 0, sizeof(buffer));

	len = listxattr(file, buffer, sizeof(buffer));
	if (len != names_len)
		exit(EXIT_FAILURE);

	/*  
	 *  Check listxattr names match what has been just set
	 */
	for (ptr = buffer; *ptr; ptr += strlen(ptr) + 1) {
		int matched = 0;

		if (xattr_metadata && !strcmp(XATTR_METADATA_NAME, ptr))
			continue;

		for (i = 0; names[i]; i++) {
			if (strcmp(names[i], ptr))
				continue;

			matched = 1;
			break;
		}

		if (!matched)
			exit(EXIT_FAILURE);
	}

	/*
	 *  Check contents of xattr
	 */
	for (i = 0; names[i]; i++) {
		len = getxattr(file, names[i], buffer, sizeof(buffer));
		if (len < 0)
			exit(EXIT_FAILURE);
		buffer[len] = '\0';

		if (strcmp(values[i], buffer))
			exit(EXIT_FAILURE);
	}
	
	/*
	 *  Remove xattr
	 */
	for (i = 0; names[i]; i++) {
		rc = removexattr(file, names[i]);
		if (rc < 0)
			exit(EXIT_FAILURE);
		names_len -= 1 + strlen(names[i]);
	}

	/*
	 *  ..and the only xattrs that should be left are those that were
	 *  already there when the test started
	 */
	len = listxattr(file, NULL, 0);
	if (len != names_len)
		exit(EXIT_FAILURE);
		
	exit(EXIT_SUCCESS);
}
