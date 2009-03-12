# **************************************************************************
# SoXt/configure.in

# FIXME: I believe there has been numerous API additions since the
# 1.2.2 release. Diff all header files against that release, and
# check what has been done. If there are only a few, consider taking them
# out and releasing as 1.2.1 -- if not, release as 1.3.0. 20050701 mortene.
m4_define([SOXT_MAJOR], [1])
m4_define([SOXT_MINOR], [3])
m4_define([SOXT_MICRO], [0])
m4_define([SOXT_BETA], [a])

m4_define([VERSION_STRING], [SOXT_MAJOR.SOXT_MINOR.SOXT_MICRO[]SOXT_BETA])

m4_define([SOXT_ABI_CURRENT], [m4_eval((SOXT_MAJOR*20)+SOXT_MINOR)])
m4_define([SOXT_ABI_REVISION], [SOXT_MICRO])
m4_define([SOXT_ABI_AGE], [SOXT_MINOR])

# only used for Debian Linux (and not even there yet)
m4_define([SO_NUMBER], [m4_eval(SOXT_MAJOR*20)])

m4_define([MAC_FRAMEWORK_NAME_DEFAULT], [SoXt])
m4_define([MAC_FRAMEWORK_PREFIX_DEFAULT], [/Library/Frameworks])
m4_define([MAC_FRAMEWORK_VERSION_DEFAULT], [A])

# **************************************************************************

AC_INIT([SoXt], [VERSION_STRING], [coin-support@coin3d.org])

AC_CONFIG_AUX_DIR([cfg])
AC_CONFIG_SRCDIR([src/Inventor/Xt/SoXt.cpp])

# **************************************************************************
# Library versioning. For information about our versioning strategy, see the
# document ``HACKING'' in the Coin CVS module.

SOXT_MAJOR_VERSION=SOXT_MAJOR
SOXT_MINOR_VERSION=SOXT_MINOR
SOXT_MICRO_VERSION=SOXT_MICRO
SOXT_INTERFACE_AGE=0
SOXT_BINARY_AGE=0
SOXT_VERSION=$SOXT_MAJOR_VERSION.$SOXT_MINOR_VERSION.$SOXT_MICRO_VERSION

AC_SUBST(SOXT_MAJOR_VERSION)
AC_SUBST(SOXT_MINOR_VERSION)
AC_SUBST(SOXT_MICRO_VERSION)
AC_SUBST(SOXT_VERSION)
AC_SUBST(SOXT_INTERFACE_AGE)
AC_SUBST(SOXT_BINARY_AGE)

LT_RELEASE=$SOXT_MAJOR_VERSION.$SOXT_MINOR_VERSION
LT_CURRENT=`expr $SOXT_MICRO_VERSION - $SOXT_INTERFACE_AGE`
LT_REVISION=$SOXT_INTERFACE_AGE
LT_AGE=`expr $SOXT_BINARY_AGE - $SOXT_INTERFACE_AGE`
AC_SUBST(LT_RELEASE)
AC_SUBST(LT_CURRENT)
AC_SUBST(LT_REVISION)
AC_SUBST(LT_AGE)

VERSION=$SOXT_VERSION

AC_DEFINE_UNQUOTED(SOXT_MAJOR_VERSION, $SOXT_MAJOR_VERSION,
  [SoXt major version])
AC_DEFINE_UNQUOTED(SOXT_MINOR_VERSION, $SOXT_MINOR_VERSION,
  [SoXt minor version])
AC_DEFINE_UNQUOTED(SOXT_MICRO_VERSION, $SOXT_MICRO_VERSION,
  [SoXt micro version])
if test x"$SOXT_BETA_VERSION" = x""; then :; else
  AC_DEFINE_UNQUOTED([SOXT_BETA_VERSION], [$SOXT_BETA_VERSION],
    [define this to the SoXt beta version letter])
fi
AC_DEFINE_UNQUOTED(SOXT_VERSION, "$SOXT_VERSION",
  [Version string for SoXt (major.minor.micro)])

# **************************************************************************

AC_CANONICAL_SYSTEM

BUILD_WITH_MSVC=false  # no support for building under MSWin (yet?)

# cross_compiling: On MKS, configure will open a window when trying to run 
# the executable a.out in the cross-compilation test because it is not 
# named *.exe, halting the configure script until it has been closed, even 
# resulting in a faulty conclusion for the test (cross_compiling=yes).  
# By setting the $cross_compiling variable to yes in advance, this test is 
# skipped.  We are of course not cross-compiling, so we correct the variable 
# afterwards.  2003-06-25 larsa

SIM_AC_SETUP_MKS

case $build in
*-mks ) cross_compiling=yes ;;
esac

AC_PROG_CC

case $build in
*-mks ) cross_compiling=no ;;
esac

AC_PROG_CPP
AC_PROG_CXX
AC_PROG_CXXCPP
AC_LANG(C++)

AM_INIT_AUTOMAKE(SoXt, $SOXT_VERSION)
AM_CONFIG_HEADER([src/config.h])

AM_DISABLE_STATIC
AM_MAINTAINER_MODE

# case "$host_os" in
#   irix*)
#     if test x"${AR+set}" != x"set"; then
#       AR="CC -ar"
#       AR_FLAGS=
#       export AR AR_FLAGS
#       echo "overriding the libtool '\$AR' setting with 'CC -ar'"
#     fi
#     ;;
#   *) ;;
# esac

AC_PROG_LIBTOOL

# These are used for constructing the soxt-config file.
AC_SUBST(SOGUI_EXTRA_CPPFLAGS, $CPPFLAGS)
AC_SUBST(SOGUI_EXTRA_CFLAGS, $CFLAGS)
AC_SUBST(SOGUI_EXTRA_CXXFLAGS, $CXXFLAGS)
AC_SUBST(SOGUI_EXTRA_LDFLAGS, $LDFLAGS)
AC_SUBST(SOGUI_EXTRA_LIBS, $LIBS)

AH_TOP([
#ifndef SOXT_INTERNAL
#error this is a private header file
#endif /* !SOXT_INTERNAL */
])

AC_CHECK_HEADERS([windows.h])

# **************************************************************************
# If you just want to generate docs, ignore all the build-related tests

sim_ac_build_library=true
AC_ARG_ENABLE(
  [build],
  AC_HELP_STRING([--disable-build], [disable configuring for library build]), [
  case $enableval in
    no) sim_ac_build_library=false ;;
    *)  ;;
  esac])

AM_CONDITIONAL(BUILD_LIBRARY, $sim_ac_build_library)

# *******************************************************************
# * Provide this nasty hack to help building SoXt on platforms
# * where one can bump into the "Arg list too long" problem
# * (IBM AIX, SGI IRIX 6.2 and IRIX 6.5 with standard settings are
# * known to fail), or where the build process is unbearably slow.

AC_ARG_ENABLE(compact,
  AC_HELP_STRING([--enable-compact], [enable hack for compact compilation [[defa
ult=no]]]),
  [case "${enableval}" in
    yes | true)  enable_compact=yes ;;
    no | false)  enable_compact=no ;;
    *) SIM_AC_ENABLE_ERROR([--enable-compact]) ;;
  esac],
  enable_compact=no)

AM_CONDITIONAL(HACKING_COMPACT_BUILD, test x"$enable_compact" = x"yes")

# *******************************************************************
# Configure an alternate installation?

AC_ARG_WITH(
  [alternate],
  [AC_HELP_STRING([--with-alternate=string],
                  [specify name of alternate configuration])],
  [CONFIG=$withval],
  [CONFIG=default])

AC_ARG_WITH(
  [suffix],
  [AC_HELP_STRING([--with-suffix=string],
                  [specify lirary name suffix (e.g. "_g")])],
  [SUFFIX=$withval],
  [SUFFIX=])

AC_SUBST(CONFIG)
AC_SUBST(SUFFIX)

# **************************************************************************
# This macro drags in the necessary setup stuff for Doxygen.
SIM_AC_SOGUI_SETUP_DOXYGEN(SoXt)

AM_CONDITIONAL([BUILD_METAFILES], [false])
AC_SUBST([meta_prefix], [bogus])

# **************************************************************************

if $sim_ac_build_library; then

# **************************************************************************

# AC_ARG_ENABLE([default-visual],
#   [AC_HELP_STRING([--enable-default-visual], [don't change from default visual])],
#   [sim_ac_default_visual=true],
#   [sim_ac_default_visual=false])
# 
# if $sim_ac_default_visual; then
#   AC_DEFINE([USE_DEFAULT_VISUAL], [1], [define to only use default visual])
# fi

# **************************************************************************

SIM_AC_RTTI_SUPPORT
SIM_AC_EXCEPTION_HANDLING
SIM_AC_CHECK_VAR_FUNCTIONNAME
SIM_AC_PROFILING_SUPPORT
SIM_AC_COMPILE_DEBUG(
  [CPPFLAGS="$CPPFLAGS -DSOXT_DEBUG=1"],
  [CPPFLAGS="$CPPFLAGS -DSOXT_DEBUG=0"])
CPPFLAGS="$CPPFLAGS -DSOXT_INTERNAL"
SIM_AC_COMPILER_WARNINGS

# *******************************************************************
# Instead of getenv() on most Win32 platforms (not Windows CE).

AC_MSG_CHECKING([for GetEnvironmentVariable() function])
AC_TRY_LINK([#include <windows.h>],
            [(void)GetEnvironmentVariable("FOOBAR", NULL, 0);],
  [AC_MSG_RESULT([yes])
  AC_DEFINE([HAVE_GETENVIRONMENTVARIABLE],1, [Define to use GetEnvironmentVariable() instead of getenv()])],
  [AC_MSG_RESULT([no])])

# **************************************************************************

SIM_AC_MATHLIB_READY_IFELSE(,[
  SIM_AC_CHECK_MATHLIB(SOGUI_EXTRA_LIBS="$SOGUI_EXTRA_LIBS $sim_ac_libm",
                       AC_MSG_ERROR(could not find a way to use math library))
])

# **************************************************************************
# Search for and set up stuff we depend on.

SIM_AC_CHECK_DL([
  SOGUI_EXTRA_CPPFLAGS="$SOGUI_EXTRA_CPPFLAGS $sim_ac_dl_cppflags"
  SOGUI_EXTRA_LDFLAGS="$SOGUI_EXTRA_LDFLAGS $sim_ac_dl_ldflags"
  SOGUI_EXTRA_LIBS="$sim_ac_dl_libs $SOGUI_EXTRA_LIBS"
])

SIM_AC_CHECK_X11([
  SOGUI_EXTRA_CPPFLAGS="$SOGUI_EXTRA_CPPFLAGS $sim_ac_x11_cppflags"
  SOGUI_EXTRA_LDFLAGS="$SOGUI_EXTRA_LDFLAGS $sim_ac_x11_ldflags"
  SOGUI_EXTRA_LIBS="$sim_ac_x11_libs $SOGUI_EXTRA_LIBS"
])

if test x"$sim_ac_x11_avail" = xyes; then
  SIM_AC_CHECK_LIBXPM([
    SOGUI_EXTRA_LIBS="$sim_ac_xpm_libs $SOGUI_EXTRA_LIBS"
    AC_DEFINE(HAVE_LIBXPM, 1, [Define this to use libXpm])
    sim_ac_xpm_status="Yes"
    SIM_AC_CONFIGURATION_SETTING([Xpm Library], [Yes])
  ], [
    SIM_AC_CONFIGURATION_SETTING([Xpm Library], [Yes])
    SIM_AC_CONFIGURATION_WARNING(
      [Without libXpm support you will not get graphics on the viewer buttons.])
  ])
  SIM_AC_CHECK_X11SHMEM([
    SOGUI_EXTRA_LIBS="$sim_ac_x11shmem_libs $SOGUI_EXTRA_LIBS"
  ])
  SIM_AC_CHECK_X11MU([
    AC_DEFINE([HAVE_LIBXMU],, [define this if -lXmu can be used])
    SOGUI_EXTRA_CPPFLAGS="$sim_ac_x11mu_cppflags $SOGUI_EXTRA_CPPFLAGS"
    SOGUI_EXTRA_LDFLAGS="$sim_ac_x11mu_ldflags $SOGUI_EXTRA_LDFLAGS"
    SOGUI_EXTRA_LIBS="$sim_ac_x11mu_libs $SOGUI_EXTRA_LIBS"
  ])
  SIM_AC_CHECK_X11XID([
    SOGUI_EXTRA_LIBS="$sim_ac_x11xid_libs $SOGUI_EXTRA_LIBS"
  ])
  # Open Group Motif depends on the Xp printing library (at least
  # on some platforms).
  SIM_AC_CHECK_X11_XP([
    SOGUI_EXTRA_LIBS="$sim_ac_xp_libs $SOGUI_EXTRA_LIBS"
  ])
  SIM_AC_CHECK_X_INTRINSIC([
    SOGUI_EXTRA_LIBS="$sim_ac_xt_libs $SOGUI_EXTRA_LIBS"
  ])
else
  AC_MSG_ERROR(need to able to compile and link against X11 libraries)
fi

AM_CONDITIONAL([USE_LIBXPM], [test x"$sim_ac_xpm_status" = x"Yes"])

if test "${with_inventor+set}" = set; then
  SIM_AC_CHECK_OPENGL([
    SOGUI_EXTRA_CPPFLAGS="$SOGUI_EXTRA_CPPFLAGS $sim_ac_ogl_cppflags"
    SOGUI_EXTRA_LDFLAGS="$SOGUI_EXTRA_LDFLAGS $sim_ac_ogl_ldflags"
    SOGUI_EXTRA_LIBS="$sim_ac_ogl_libs $SOGUI_EXTRA_LIBS"
  ], [AC_MSG_ERROR(needs an OpenGL-compatible development system installation)])

  SIM_AC_CHECK_GLU([
    SOGUI_EXTRA_CPPFLAGS="$SOGUI_EXTRA_CPPFLAGS $sim_ac_glu_cppflags"
    SOGUI_EXTRA_LDFLAGS="$SOGUI_EXTRA_LDFLAGS $sim_ac_glu_ldflags"
    SOGUI_EXTRA_LIBS="$sim_ac_glu_libs $SOGUI_EXTRA_LIBS"
  ], [AC_MSG_ERROR(needs the OpenGL utility library)])

  # check for SGI or TGS Open Inventor
  SIM_AC_HAVE_INVENTOR_IFELSE([
    CPPFLAGS="$CPPFLAGS $sim_ac_inventor_cppflags"
    LDFLAGS="$LDFLAGS $sim_ac_inventor_ldflags"
    LIBS="$sim_ac_inventor_libs $LIBS"
    SOGUI_EXTRA_CPPFLAGS="$SOGUI_EXTRA_CPPFLAGS $sim_ac_inventor_cppflags"
    SOGUI_EXTRA_LDFLAGS="$SOGUI_EXTRA_LDFLAGS $sim_ac_inventor_ldflags"
    SOGUI_EXTRA_LIBS="$sim_ac_inventor_libs $SOGUI_EXTRA_LIBS"
  ], [AC_MSG_ERROR([couldn't compile and link against Open Inventor])])

  SIM_AC_CONFIGURATION_SETTING([Open Inventor brand], [SGI/TGS Inventor])

else

  SIM_AC_CHECK_HEADER_GL([
    CPPFLAGS="$CPPFLAGS $sim_ac_gl_cppflags"
    SOGUI_EXTRA_CPPFLAGS="$SOGUI_EXTRA_CPPFLAGS $sim_ac_gl_cppflags"
  ], [AC_MSG_ERROR([could not detect a gl.h header file])])

  # check for Coin library and its dependencies
  SIM_AC_HAVE_COIN_IFELSE([
    CPPFLAGS="$CPPFLAGS $sim_ac_coin_cppflags"
    LDFLAGS="$LDFLAGS $sim_ac_coin_ldflags"
    LIBS="$sim_ac_coin_libs $LIBS"
    SOGUI_EXTRA_CPPFLAGS="$SOGUI_EXTRA_CPPFLAGS $sim_ac_coin_cppflags"
    SOGUI_EXTRA_LDFLAGS="$SOGUI_EXTRA_LDFLAGS $sim_ac_coin_ldflags"
    SOGUI_EXTRA_LIBS="$sim_ac_coin_libs $SOGUI_EXTRA_LIBS"
  ], [AC_MSG_ERROR([couldn't compile and link against Coin])])

  SIM_AC_CONFIGURATION_SETTING([Open Inventor brand], [Coin])
fi

# **************************************************************************
# Check for miscellaneous "optional" Open Inventor features.

SIM_AC_HAVE_INVENTOR_NODE(SoPolygonOffset)
SIM_AC_HAVE_INVENTOR_VRMLNODE(SoVRMLMaterial)

SIM_AC_HAVE_INVENTOR_FEATURE(
  [for SoKeyboardEvent::DELETE enum],
  [#include <Inventor/events/SoKeyboardEvent.h>], [
   /* This is either DELETE or KEY_DELETE */
   SoKeyboardEvent::Key key = SoKeyboardEvent::DELETE;],
  [HAVE_SOKEYBOARDEVENT_DELETE])

SIM_AC_HAVE_INVENTOR_FEATURE(
  [for SoMouseButtonEvent::BUTTON5 enum],
  [#include <Inventor/events/SoMouseButtonEvent.h>], [
   /* mouse wheel support needs BUTTON4 and BUTTON5 */
   SoMouseButtonEvent::Button button = SoMouseButtonEvent::BUTTON5;],
  [HAVE_SOMOUSEBUTTONEVENT_BUTTON5])

SIM_AC_HAVE_INVENTOR_FEATURE(
  [for SoCamera::setStereoMode() method],
  [#include <Inventor/nodes/SoPerspectiveCamera.h>], [
   /* SoCamera is abstract, so test with SoPerspectiveCamera */
   SoPerspectiveCamera * c = new SoPerspectiveCamera;
   c->setStereoMode(SoCamera::MONOSCOPIC);],
  [HAVE_SOCAMERA_SETSTEREOMODE])

SIM_AC_HAVE_INVENTOR_FEATURE(
  [for SoGLRenderAction::SORTED_LAYERS_BLEND transparency mode],
  [#include <Inventor/actions/SoGLRenderAction.h>], [
  /* try to get SORTED_LAYERS_BLEND transparency mode value */
  int num = (int) SoGLRenderAction::SORTED_LAYERS_BLEND;],
  [HAVE_SORTED_LAYERS_BLEND])

# **************************************************************************

SIM_AC_CHECK_JOYSTICK_LINUX([
  AC_DEFINE(HAVE_JOYSTICK_LINUX, ,
    [Define to enable Linux Joystick driver support])
  AC_DEFINE_UNQUOTED(SOXT_LINUX_JOYSTICK_DEVICE,
    "$sim_ac_joystick_linux_device",
    [Define this to device handle to fallback on])
])
AM_CONDITIONAL(HAVE_JOYSTICK_LINUX, $sim_ac_joystick_linux_avail)

SIM_CHECK_MOTIF([
  SOGUI_EXTRA_CPPFLAGS="$SOGUI_EXTRA_CPPFLAGS $sim_ac_motif_cppflags"
  SOGUI_EXTRA_LDFLAGS="$SOGUI_EXTRA_LDFLAGS $sim_ac_motif_ldflags"
  SOGUI_EXTRA_LIBS="$sim_ac_motif_libs $SOGUI_EXTRA_LIBS"
], AC_MSG_ERROR([couldn't compile and link with Motif]))

AC_DEFINE_UNQUOTED([GUI_TOOLKIT_VERSION], ["$sim_ac_motif_version"],
                   [Version string for Motif])

# **************************************************************************

# --with-devcode is a silent option
sim_ac_want_devcode=false
if test x"${with_devcode+set}" = x"set"; then
  AC_MSG_CHECKING([for devcode directory])
  if test -f ${srcdir}/devcode/configure.in; then
    AC_MSG_RESULT([found])
    sim_ac_want_devcode=true
  else
    AC_MSG_RESULT([not found])
    AC_MSG_ERROR([can't use '--with-devcode' when devcode/ doesn't exist])
  fi
fi

SIM_AC_SOGUI_STATIC_DEFAULTS

if $sim_ac_static_defaults; then
  SIM_AC_CONFIGURATION_SETTING([Static Materials], [Yes])
else
  SIM_AC_CONFIGURATION_SETTING([Static Materials], [No])
fi

AM_CONDITIONAL(BUILD_DEVCODE, $sim_ac_want_devcode)

# **************************************************************************

COIN_COMPILER="$CXX"
if $BUILD_WITH_MSVC; then
  # don't use wrapmsvc from source hierarchy when later using soxt-config.
  COIN_COMPILER="wrapmsvc"
fi
AC_SUBST(COIN_COMPILER)

# **************************************************************************

else
  AM_CONDITIONAL([USE_LIBXPM], [false])
  AM_CONDITIONAL([HAVE_JOYSTICK_LINUX], [false])
  AM_CONDITIONAL([BUILD_DEVCODE], [false])
fi  # $sim_ac_build_library == true

# *******************************************************************
# * Provide this nasty hack to help building Coin on platforms
# * where one can bump into the "Arg list too long" problem
# * (IBM AIX, SGI IRIX 6.2 and IRIX 6.5 with standard settings are
# * known to fail).
# *******************************************************************

AC_ARG_ENABLE(compact,
  AC_HELP_STRING([--enable-compact], [enable hack for compact compilation [[default=no]]]),
    [case "${enableval}" in
     yes | true)  enable_compact=yes ;;
     no | false)  enable_compact=no ;;
     *) SIM_AC_ENABLE_ERROR([--enable-compact]) ;;
     esac],
    enable_compact=no)

AM_CONDITIONAL(HACKING_COMPACT_BUILD, test x"$enable_compact" = x"yes")

# **************************************************************************
# Variable substitutions for using libSoXt in the generic code.

GUI=XT
Gui=Xt
gui=xt

WIDGET="Widget"
EVENT="XAnyEvent *"
COMPONENTHEADER=""

AC_SUBST(GUI)
AC_SUBST(Gui)
AC_SUBST(gui)
AC_SUBST(WIDGET)
AC_SUBST(EVENT)
AC_SUBST(COMPONENTHEADER)

# **************************************************************************
# For the soxt.spec RPM specification file.

SIM_AC_ISO8601_DATE(soxt_configure_date)
AC_SUBST(soxt_configure_date)

# **************************************************************************
# Remove redundant options from certain option lists.

SIM_AC_UNIQIFY_LIST(SOGUI_EXTRA_CPPFLAGS, -I$includedir $SOGUI_EXTRA_CPPFLAGS)
SIM_AC_UNIQIFY_LIST(SOGUI_EXTRA_LDFLAGS, -L$libdir $SOGUI_EXTRA_LDFLAGS)
SIM_AC_UNIQIFY_LIST(SOGUI_EXTRA_LIBS, -lSoXt $SOGUI_EXTRA_LIBS)

SIM_AC_UNIQIFY_LIST(CPPFLAGS, $CPPFLAGS)
SIM_AC_UNIQIFY_LIST(LDFLAGS, $LDFLAGS)
SIM_AC_UNIQIFY_LIST(LIBS, $LIBS)

# **************************************************************************
# Configure the submodule with the data.

# Material editor(s) not part of the public API yet, so there's
# no point yet in installing the material data.
#if $sim_ac_build_library; then
#  AC_CONFIG_SUBDIRS([data])
#fi

# **************************************************************************
# Generate the makefiles and config files.
#
# Note that soxt-$CONFIG.cfg needs to be generated from the configure
# script -- not just from the Makefile -- as the data/configure script
# depends on it. (data/configure is temporarily disabled, BTW, but anyway..)

AC_CONFIG_FILES([
        soxt-$CONFIG.cfg:src/Inventor/Xt/common/sogui.cfg.in
	SoXt.pc
        Makefile
        src/Makefile
        src/Inventor/Makefile
        src/Inventor/Xt/Makefile
        src/Inventor/Xt/devices/Makefile
        src/Inventor/Xt/editors/Makefile
        src/Inventor/Xt/engines/Makefile
        src/Inventor/Xt/nodes/Makefile
        src/Inventor/Xt/viewers/Makefile
        src/Inventor/Xt/widgets/Makefile
        man/Makefile
        man/man1/Makefile
        man/man3/Makefile
        html/Makefile
        test-code/Makefile
        build/soxt.spec
])

# MKS will have "\c" as $ECHO_C, but that string in the config.status
# substitution command will cause config.status to hang because it is
# somehow translated to "&c".  We therefore blank it here, and restore
# it further below, after AC_OUTPUT.

sim_ac_ECHO_C=$ECHO_C
case $build in
*-mks ) ECHO_C="" ;;
esac

AC_OUTPUT

ECHO_C=$sim_ac_ECHO_C

SIM_AC_CONFIGURATION_SETTING([Installation Prefix], [$prefix])

# **************************************************************************

SIM_AC_CONFIGURATION_SUMMARY

echo ""
echo "Now, run 'make install' to build and install So$Gui."
echo ""

# **************************************************************************
