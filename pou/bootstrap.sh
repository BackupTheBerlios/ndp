#!/bin/sh
export WANT_AUTOCONF=2.5
export WANT_AUTOMAKE=1.7

echo bootstrapping in $PWD
aclocal
automake -a
autoconf

(cd libpou ; ./bootstrap.sh)
(cd PoUgui ; ./bootstrap.sh)
