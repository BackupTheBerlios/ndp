#!/bin/sh
export WANT_AUTOCONF=2.5
export WANT_AUTOMAKE=1.7

echo bootstrapping in $PWD

aclocal -I m4
autoheader
libtoolize -c
automake -a
autoconf
