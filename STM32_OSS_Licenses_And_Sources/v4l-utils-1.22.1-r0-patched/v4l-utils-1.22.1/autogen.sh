#!/bin/bash - 

test -n "$srcdir" || srcdir=`dirname "$0"`
test -n "$srcdir" || srcdir=.

olddir=`pwd`
(
  cd "$srcdir" &&
  autoreconf --force -v --install
) || exit
test -n "$NOCONFIGURE" || "$srcdir/configure" "$@"
cd $olddir
