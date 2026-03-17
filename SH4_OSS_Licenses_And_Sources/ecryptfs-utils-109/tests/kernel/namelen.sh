#!/bin/bash
#
# namelen.sh: Test for validating namelen reported by eCryptfs
# Author: Tyler Hicks <tyhicks@canonical.com>
#
# Copyright (C) 2014 Canonical Ltd.
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License
# as published by the Free Software Foundation version 2
# of the License.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA

test_script_dir=$(dirname $0)
rc=1

. ${test_script_dir}/../lib/etl_funcs.sh

test_cleanup()
{
	etl_remove_test_dir "$test_dir"
	etl_umount
	etl_lumount
	etl_unlink_keys
	exit $rc
}
trap test_cleanup 0 1 2 3 15

etl_add_keys || exit
etl_lmount || exit
etl_mount_i || exit
test_dir=$(etl_create_test_dir) || exit
test_file="$test_dir/a"

namelen=$(getconf NAME_MAX "$test_dir")
[ $namelen -le 0 ] && exit

# Make sure we can create any file up to namelen chars long
for ((i=1; i <= $namelen; i++)); do
	touch "$test_file" || exit
	rm $test_file
	test_file+=a
done

# Throw an error if creating a file (namelen + 1) chars long succeeds
test_file+=a
touch "$test_file" 2>/dev/null && exit

rc=0
exit
