#! /bin/sh
# **************************************************************************
# Regenerate all files which are constructed by the autoconf, automake
# and libtool tool-chain. Note: only developers should need to use this
# script.

# Authors:
#   Morten Eriksen <mortene@sim.no>
#   Lars J. Aas <larsa@sim.no>
#   Ralph Levien (original autogen.sh picked up from Gnome source archive).

wd=`echo $0 | sed 's,[^\\/]*$,,g'`;
cd $wd
if test ! -f autogen.sh; then
  echo "unexpected problem with your shell - bailing out"
  exit 1
fi

AUTOCONF_VER=2.49b   # Autoconf from CVS
AUTOMAKE_VER=1.4a    # CVS development version
LIBTOOL_VER=1.3.5

PROJECT=SoXt
GUI=Xt
MACRODIR=cfg/m4

SUBPROJECTS="$MACRODIR data"
SUBPROJECTNAMES="SoXtMacros SoXtData"
AUTOMAKE_ADD=

if test "$1" = "--clean"; then
  rm -f config.h.in configure stamp-h*
  find . -name Makefile.in -print | xargs rm
  exit 0
elif test "$1" = "--add"; then
  AUTOMAKE_ADD=""
fi

echo "Checking the installed configuration tools..."

if test -z "`autoconf --version | grep \" $AUTOCONF_VER\" 2> /dev/null`"; then
  cat <<EOF

  Invalid Version of Autoconf
  ---------------------------
  You must use the CVS development version of autoconf ($AUTOCONF_VER)
  to generate configure information and Makefiles for $PROJECT.
  You can find the pre-release snapshot at:

  ftp://alpha.gnu.org/gnu/autoconf/autoconf-2.49a.tar.gz

EOF
  DIE=true
fi

if test -z "`automake --version | grep \" $AUTOMAKE_VER\" 2> /dev/null`"; then
  cat <<EOF

  Invalid Version of Automake
  ---------------------------
  You must use the CVS development version of automake to ($AUTOMAKE_VER)
  to generate configure information and Makefiles for $PROJECT.

  The CVS automake repository can be fetched by running the following
  set of commands:

  $ cvs -d :pserver:anoncvs@anoncvs.cygnus.com:/cvs/automake login
  $ cvs -d :pserver:anoncvs@anoncvs.cygnus.com:/cvs/automake co automake

EOF
  DIE=true
fi

if test -z "`libtool --version | grep \" $LIBTOOL_VER \" 2> /dev/null`"; then
  cat <<EOF

  Invalid Version of Libtool
  --------------------------
  You must have libtool version $LIBTOOL_VER installed to generate
  configure information and Makefiles for $PROJECT.

  Get ftp://ftp.gnu.org/pub/gnu/libtool/libtool-1.3.5.tar.gz

EOF
  DIE=true
fi

set $SUBPROJECTNAMES
num=1
for project in $SUBPROJECTS; do
  test -d $project || {
    echo "Could not find subdirectory '$project'."
    echo "It was probably added after you initially fetched $PROJECT."
    echo "To add the missing module, run 'cvs co $1' from the $PROJECT"
    echo "base directory."
    echo ""
    echo "To do a completely fresh cvs checkout of the whole $PROJECT module,"
    echo "(if all else fails), remove $PROJECT and run:"
    echo ""
    echo "  cvs -z3 -d :pserver:cvs@cvs.sim.no:/export/cvsroot co -P $PROJECT"
    echo ""
    DIE=true
  }
  num=`expr $num + 1`
  shift
done

# abnormal exit?
${DIE=false} && echo "" && exit 1

# generate aux/aclocal.m4
echo "Running aclocal..."
aclocal -I $MACRODIR

# generate config.h.in
echo "Running autoheader..."
autoheader

# generate Makefile.in templates
echo "Running automake..."
echo "[ignore any \"directory should not contain '/'\" warning]"
automake $AUTOMAKE_ADD

AMBUGFIXES=`find . \( -name Makefile.in.diff \) | egrep -v '^\./data'`
for bugfix in $AMBUGFIXES; do
  : ${fixmsg=true}
  $fixmsg && {
    echo "[correcting automake bugs]"
    fixmsg=false
  }
  patch -p0 < $bugfix
done

# generate configure
echo "Running autoconf..."
autoconf

echo "Done."

