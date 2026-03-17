#!/bin/bash
#
# lfs.sh: Test for large file support enabled builds
# Author: Tyler Hicks <tyhicks@canonical.com>
#
# Copyright (C) 2013 Canonical, Ltd.
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
	exit $rc
}
trap test_cleanup 0 1 2 3 15

# Run the test program. See its source for the details of this test.
${test_script_dir}/lfs/test

rc=$?
exit
