/*
 * Author: Tyler Hicks <tyhicks@canonical.com>
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

/*
 * This test is to ensure that off_t is the same size as off64_t in all
 * ecryptfs-utils builds. On 64 bit machines, this is the default. On 32 bit
 * machines, the build system must define special variables to enable what is
 * known as Large File Support (LFS).
 *
 * In configure.ac, we enable LFS by using the AC_SYS_LARGEFILE autoconf macro.
 * It defines _FILE_OFFSET_BITS=64, when needed by the target architecture, in
 * config.h. Also in configure.ac, we force all .c files to #include config.h
 * so the entire ecryptfs-utils build should always have LFS enabled.
 *
 * IMPORTANT: We intentionally do not include config.h in this file, because it
 * is expected that the build system automatically does it for us. This test
 * verifies that the inclusion on config.h happens.
 *
 * We must define _LARGEFILE64_SOURCE in this file so that off64_t is
 * available. However, defining it does not enable large file support.
 */

#define _LARGEFILE64_SOURCE 1
#include <sys/types.h>

int main(void)
{
	return sizeof(off_t) == sizeof(off64_t) ? 0 : 1;
}
