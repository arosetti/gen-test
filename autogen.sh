#!/bin/sh

aclocal
autoheader
hg log > ChangeLog
automake --add-missing --copy
autoconf
