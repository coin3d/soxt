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

AUTOCONF_VER=2.14.1  # Autoconf from CVS.
if test -z "`autoconf --version | grep \" $AUTOCONF_VER\" 2> /dev/null`"; then
    echo
    echo "You must have autoconf version $AUTOCONF_VER installed to"
    echo "generate configure information and Makefiles for $PROJECT." 
#    echo "Get ftp://ftp.gnu.org/pub/gnu/autoconf-*.tar.gz"
    echo "Get autoconf from CVS: "
    echo "  cvs -d :pserver:anoncvs@anoncvs.cygnus.com:/cvs/autoconf co autoconf"
    DIE=0
fi

LIBTOOL_VER=1.3.4  # Latest release of libtool
if test -z "`libtool --version | grep \" $LIBTOOL_VER \" 2> /dev/null`"; then
    echo
    echo "You must have libtool version $LIBTOOL_VER installed to"
    echo "generate configure information and Makefiles for $PROJECT." 
    echo "Get ftp://ftp.gnu.org/pub/gnu/libtool-*.tar.gz"
    DIE=1
fi

AUTOMAKE_VER=1.4a  # Automake from CVS.
if test -z "`automake --version | grep \" $AUTOMAKE_VER\" 2> /dev/null`"; then
    echo
    echo "You must have automake version $AUTOMAKE_VER installed to"
    echo "generate configure information and Makefiles for $PROJECT." 
#    echo "Get ftp://ftp.gnu.org/pub/gnu/automake-*.tar.gz"
    echo "Get automake from CVS: "
    echo "  cvs -d :pserver:anoncvs@anoncvs.cygnus.com:/cvs/automake co automake"
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

AMBUGFIXES=`find . -name Makefile.in.diff`
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

