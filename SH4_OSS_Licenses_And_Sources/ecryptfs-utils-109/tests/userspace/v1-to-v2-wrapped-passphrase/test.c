/**
 * test.c: Verify the migration from version 1 to version 2 wrapped-passphrase
 * 	   files
 * Author: Tyler Hicks <tyhicks@canonical.com>
 *
 * Copyright (C) 2015 Canonical, Ltd.
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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA
 */

#include <errno.h>
#include <stdio.h>
#include <string.h>
#include "../../src/include/ecryptfs.h"

#define ECRYPTFS_MAX_KEY_HEX_BYTES (ECRYPTFS_MAX_KEY_BYTES * 2)

#define NEW_WRAPPING_PASSPHRASE "The *new* eCryptfs wrapping passphrase."

static void usage(const char *name)
{
	fprintf(stderr,
		"%s FILENAME EXPECTED_PASS WRAPPING_PASS WRAPPING_SALT_HEX\n",
		name);
}

/**
 * Returns 0 if the unwrap operation resulted in the expected decrypted
 * passphrase
 */
static int verify_unwrap(char *expected_decrypted_passphrase, char *filename,
			 char *wrapping_passphrase, char *wrapping_salt)
{
	char decrypted_passphrase[ECRYPTFS_MAX_PASSPHRASE_BYTES + 1];
	int rc;

	memset(decrypted_passphrase, 0, sizeof(decrypted_passphrase));

	rc = ecryptfs_unwrap_passphrase(decrypted_passphrase, filename,
					wrapping_passphrase, wrapping_salt);
	if (rc)
		return 1;

	if (strcmp(decrypted_passphrase, expected_decrypted_passphrase))
		return 1;

	return 0;
}

/**
 * Returns 0 if the *invalid* unwrap operations always fail
 */
static int verify_bad_unwrap(char *expected_decrypted_passphrase, char *filename,
			     char *wrapping_passphrase, char *wrapping_salt)
{
	char *last;
	int rc;

	/* Increment first char in the wrapping_passphrase and verify that the
	 * unwrapping operation fails */
	wrapping_passphrase[0]++;
	rc = verify_unwrap(expected_decrypted_passphrase, filename,
			   wrapping_passphrase, wrapping_salt);
	wrapping_passphrase[0]--;
	if (!rc)
		return 1;

	/* Increment last char in the wrapping_passphrase and verify that the
	 * unwrapping operation fails */
	last = wrapping_passphrase + (strlen(wrapping_passphrase) - 1);
	(*last)++;
	rc = verify_unwrap(expected_decrypted_passphrase, filename,
			   wrapping_passphrase, wrapping_salt);
	(*last)--;
	if (!rc)
		return 1;

	/* Perform a one's complement on the first char in the salt and verify
	 * that the unwrapping operation fails */
	wrapping_salt[0] = ~wrapping_salt[0];
	rc = verify_unwrap(expected_decrypted_passphrase, filename,
			   wrapping_passphrase, wrapping_salt);
	wrapping_salt[0] = ~wrapping_salt[0];
	if (!rc)
		return 1;

	/* Perform a one's complement on the last char in the salt and verify
	 * that the unwrapping operation fails */
	last = wrapping_salt + (ECRYPTFS_SALT_SIZE - 1);
	*last = ~(*last);
	rc = verify_unwrap(expected_decrypted_passphrase, filename,
			  wrapping_passphrase, wrapping_salt);
	*last = ~(*last);
	if (!rc)
		return 1;

	return 0;
}

static int do_rewrap(char *filename, char *old_wrapping_passphrase,
		     char *old_wrapping_salt, char *new_wrapping_passphrase)
{
	char decrypted_passphrase[ECRYPTFS_MAX_PASSPHRASE_BYTES + 1];
	uint8_t version = 0;
	int rc;

	memset(decrypted_passphrase, 0, sizeof(decrypted_passphrase));

	rc = ecryptfs_unwrap_passphrase(decrypted_passphrase, filename,
					old_wrapping_passphrase,
					old_wrapping_salt);
	if (rc)
		return 1;

	rc = ecryptfs_wrap_passphrase(filename, new_wrapping_passphrase, NULL,
				      decrypted_passphrase);
	if (rc)
		return 1;

	rc = __ecryptfs_detect_wrapped_passphrase_file_version(filename,
							       &version);
	if (version != 2)
		return 1;

	return 0;
}

int main(int argc, char *argv[])
{
	char wrapping_salt[ECRYPTFS_SALT_SIZE];
	char *expected_decrypted_passphrase, *filename, *wrapping_passphrase,
	     *wrapping_salt_hex;
	int rc;

	if (argc != 5) {
		usage(argv[0]);
		return EINVAL;
	}

	filename = argv[1];
	expected_decrypted_passphrase = argv[2];
	wrapping_passphrase = argv[3];
	wrapping_salt_hex = argv[4];

	if (strlen(expected_decrypted_passphrase) > ECRYPTFS_MAX_PASSPHRASE_BYTES ||
	    strlen(wrapping_passphrase) > ECRYPTFS_MAX_PASSPHRASE_BYTES ||
	    strlen(wrapping_salt_hex) != ECRYPTFS_SALT_SIZE_HEX) {
		usage(argv[0]);
		return EINVAL;
	}

	from_hex(wrapping_salt, wrapping_salt_hex, ECRYPTFS_SALT_SIZE);

	rc = verify_unwrap(expected_decrypted_passphrase, filename,
			   wrapping_passphrase, wrapping_salt);
	if (rc)
		return 1;

	rc = verify_bad_unwrap(expected_decrypted_passphrase, filename,
			       wrapping_passphrase, wrapping_salt);
	if (rc)
		return 2;

	rc = do_rewrap(filename, wrapping_passphrase, wrapping_salt,
		       NEW_WRAPPING_PASSPHRASE);
	if (rc)
		return 3;

	rc = verify_unwrap(expected_decrypted_passphrase, filename,
			   NEW_WRAPPING_PASSPHRASE, NULL);
	if (rc)
		return 4;

	return 0;
}

