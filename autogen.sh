#!/bin/sh

# Regenerate all files which are constructed by the autoconf, automake
# and libtool tool-chain. Note: only developers should need to use
# this script.

# Author: Morten Eriksen, <mortene@sim.no>. Loosely based on Ralph
# Levien's script for Gnome.

DIE=0

GUI=Xt

PROJECT=So$GUI

MACRODIR=conf-macros
SUBPROJECTS="$MACRODIR examples"

echo "Checking the installed configuration tools..."

AUTOCONF_VER=2.14.1  # Autoconf from CVS @ 2000-01-13.
if test -z "`autoconf --version | grep \" $AUTOCONF_VER\" 2> /dev/null`"; then
    echo
    echo "You must have autoconf version $AUTOCONF_VER installed to"
    echo "generate configure information and Makefiles for $PROJECT."
    echo ""
    echo "The Autoconf version we are using is a development version"
    echo "\"frozen\" from the CVS repository at 2000-01-13. You can get"
    echo "it here:"
    echo ""
    echo "   ftp://ftp.sim.no/pub/coin/autoconf-2.14.1-coin.tar.gz"
    echo ""
    DIE=1
fi

LIBTOOL_VER=1.3.4  # Latest release of libtool
if test -z "`libtool --version | grep \" $LIBTOOL_VER \" 2> /dev/null`"; then
    echo
    echo "You must have libtool version $LIBTOOL_VER installed to"
    echo "generate configure information and Makefiles for $PROJECT."
    echo ""
    echo "Get ftp://ftp.gnu.org/pub/gnu/libtool/libtool-1.3.4.tar.gz"
    echo ""
    DIE=1
fi

AUTOMAKE_VER=1.4a  # Automake from CVS @ 2000-01-13.
if test -z "`automake --version | grep \" $AUTOMAKE_VER\" 2> /dev/null`"; then
    echo
    echo "You must have automake version $AUTOMAKE_VER installed to"
    echo "generate configure information and Makefiles for $PROJECT."
    echo ""
    echo "The Automake version we are using is a development version"
    echo "\"frozen\" from the CVS repository at 2000-01-13. You can get"
    echo "it here:"
    echo ""
    echo "   ftp://ftp.sim.no/pub/coin/automake-1.4a-coin.tar.gz"
    echo ""
    DIE=1
fi


for project in $SUBPROJECTS; do
  test -d $project || {
    echo
    echo "The CVS sub-project '$project' was not found."
    echo "It was probably added after you initially checked out $PROJECT."
    echo "Do a fresh 'cvs checkout' to correct this problem - the $PROJECT build system"
    echo "will probably not work properly otherwise.  For a fresh 'cvs checkout',"
    echo "run 'cvs -d :pserver:cvs@cvs.sim.no:/export/cvsroot co -P $PROJECT'."
    echo
  }
done

if test "$DIE" -eq 1; then
        exit 1
fi


echo "Running aclocal (generating aclocal.m4)..."
aclocal -I $MACRODIR

echo "Running autoheader (generating config.h.in)..."
autoheader

echo "Running automake (generating the Makefile.in files)..."
echo "[ignore any \"directory should not contain '/'\" warning]"
automake

AMBUGFIXES=`find . -name Makefile.in.diff | grep -v examples`
fixmsg=0
for bugfix in $AMBUGFIXES; do
  if test $fixmsg -eq 0; then
    echo "[correcting automake bugs]"
    fixmsg=1
  fi
  patch --no-backup-if-mismatch -p0 < $bugfix
done

echo "Running autoconf (generating ./configure)..."
autoconf

echo "Done: Now run './configure' and 'make install' to build $PROJECT."

