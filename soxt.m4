############################################################################
# Usage:
#   SIM_AC_HAVE_SOXT_IFELSE( IF-FOUND, IF-NOT-FOUND )
#
# Description:
#   This macro locates the SoXt development system.  If it is found,
#   the set of variables listed below are set up as described and made
#   available to the configure script.
#
#   The $sim_ac_soxt_desired variable can be set to false externally to
#   make SoXt default to be excluded.
#
# Autoconf Variables:
# > $sim_ac_soxt_desired     true | false (defaults to true)
# < $sim_ac_soxt_avail       true | false
# < $sim_ac_soxt_cppflags    (extra flags the preprocessor needs)
# < $sim_ac_soxt_ldflags     (extra flags the linker needs)
# < $sim_ac_soxt_libs        (link library flags the linker needs)
# < $sim_ac_soxt_datadir     (location of SoXt data files)
# < $sim_ac_soxt_version     (the libSoXt version)
#
# Authors:
#   Lars J. Aas, <larsa@sim.no>
#   Morten Eriksen, <mortene@sim.no>
#
# TODO:
#

AC_DEFUN([SIM_AC_HAVE_SOXT_IFELSE], [
AC_PREREQ([2.14a])

# official variables
sim_ac_soxt_avail=false
sim_ac_soxt_cppflags=
sim_ac_soxt_ldflags=
sim_ac_soxt_libs=
sim_ac_soxt_datadir=
sim_ac_soxt_version=

# internal variables
: ${sim_ac_soxt_desired=true}
sim_ac_soxt_extrapath=

AC_ARG_WITH([soxt], AC_HELP_STRING([--without-soxt], [disable use of SoXt]))
AC_ARG_WITH([soxt], AC_HELP_STRING([--with-soxt], [enable use of SoXt]))
AC_ARG_WITH([soxt],
  AC_HELP_STRING([--with-soxt=DIR], [give prefix location of SoXt]),
  [ case $withval in
    no)  sim_ac_soxt_desired=false ;;
    yes) sim_ac_soxt_desired=true ;;
    *)   sim_ac_soxt_desired=true
         sim_ac_soxt_extrapath=$withval ;;
    esac],
  [])

if $sim_ac_soxt_desired; then
  sim_ac_path=$PATH
  test -z "$sim_ac_soxt_extrapath" ||   ## search in --with-soxt path
    sim_ac_path=$sim_ac_soxt_extrapath/bin:$sim_ac_path
  test x"$prefix" = xNONE ||     ## search in --prefix path
    sim_ac_path=$sim_ac_path:$prefix/bin

  AC_PATH_PROG(sim_ac_soxt_configcmd, soxt-config, false, $sim_ac_path)
  if $sim_ac_soxt_configcmd; then
    sim_ac_soxt_cppflags=`$sim_ac_soxt_configcmd --cppflags`
    sim_ac_soxt_ldflags=`$sim_ac_soxt_configcmd --ldflags`
    sim_ac_soxt_libs=`$sim_ac_soxt_configcmd --libs`
    sim_ac_soxt_datadir=`$sim_ac_soxt_configcmd --datadir`
    sim_ac_soxt_version=`$sim_ac_soxt_configcmd --version`
    AC_CACHE_CHECK(
      [whether libSoXt is available],
      sim_cv_soxt_avail,
      [sim_ac_save_cppflags=$CPPFLAGS
      sim_ac_save_ldflags=$LDFLAGS
      sim_ac_save_libs=$LIBS
      CPPFLAGS="$CPPFLAGS $sim_ac_soxt_cppflags"
      LDFLAGS="$LDFLAGS $sim_ac_soxt_ldflags"
      LIBS="$sim_ac_soxt_libs $LIBS"
      AC_LANG_PUSH(C++)
      AC_TRY_LINK(
        [#include <Inventor/Xt/SoXt.h>],
        [(void)SoXt::init((const char *)0L);],
        [sim_cv_soxt_avail=true],
        [sim_cv_soxt_avail=false])
      AC_LANG_POP
      CPPFLAGS=$sim_ac_save_cppflags
      LDFLAGS=$sim_ac_save_ldflags
      LIBS=$sim_ac_save_libs
    ])
    sim_ac_soxt_avail=$sim_cv_soxt_avail
  else
    locations=`IFS=:; for p in $sim_ac_path; do echo " -> $p/soxt-config"; done`
    AC_MSG_WARN([cannot find 'soxt-config' at any of these locations:
$locations])
  fi
fi

if $sim_ac_soxt_avail; then
  ifelse([$1], , :, [$1])
else
  ifelse([$2], , :, [$2])
fi
]) # SIM_AC_HAVE_SOXT_IFELSE()

