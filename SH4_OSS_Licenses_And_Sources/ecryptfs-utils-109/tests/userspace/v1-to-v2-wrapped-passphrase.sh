#!/bin/bash
#
# v1-to-v2-wrapped-passphrase.sh: Verify that v1 wrapped passphrase files can
#				   be unwrapped and then rewrapped as v2 files.
# Author: Tyler Hicks <tyhicks@canonical.com>
#
# Copyright (C) 2015 Canonical Ltd.
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
	etl_remove_test_dir $test_dir
	exit $rc
}
trap test_cleanup 0 1 2 3 15

do_test()
{
	${test_script_dir}/v1-to-v2-wrapped-passphrase/test "$@"
	rc=$?
	if [ "$rc" -ne 0 ]; then
		exit
	fi
}

test_dir_parent="$TMPDIR"
if [ -z "$test_dir_parent"]; then
	test_dir_parent="/tmp"
fi

test_dir=$(etl_create_test_dir "$test_dir_parent") || exit
cp "${test_script_dir}/v1-to-v2-wrapped-passphrase/wp"* "$test_dir"

do_test "${test_dir}/wp01" "This is test #1" "Wrapping pass" "0011223344556677"

do_test "${test_dir}/wp02" "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa" "zzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz" "0011223344556677"

do_test "${test_dir}/wp03" "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa" "zzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz" "5a175a175a175a17"

do_test "${test_dir}/wp04" "!" "*" "0011223344556677"

do_test "${test_dir}/wp05" "!" "*" "0123456789abcdef"

rc=0
exit
