cmake_minimum_required(VERSION 3.0)

set(gui xt)
set(Gui Xt)
set(GUI XT)
set(WIDGET Widget)
set(EVENT XAnyEvent*)

function(dump_variable)
  if (SO${GUI}_VERBOSE)
    foreach(f ${ARGN})
      if (DEFINED ${f})
        message("${f} = ${${f}}")
      else()
        message("${f} = ***UNDEF***")
      endif()
    endforeach()
  endif()
endfunction()

set(SO${GUI}_MAJOR_VERSION 1)
set(SO${GUI}_MINOR_VERSION 3)
set(SO${GUI}_MICRO_VERSION 2)
set(SO${GUI}_BETA_VERSION  a)
set(SO${GUI}_VERSION ${SO${GUI}_MAJOR_VERSION}.${SO${GUI}_MINOR_VERSION}.${SO${GUI}_MICRO_VERSION}${SO${GUI}_BETA_VERSION})

project(So${Gui} VERSION ${SO${GUI}_MAJOR_VERSION}.${SO${GUI}_MINOR_VERSION}.${SO${GUI}_MICRO_VERSION})
string(TOLOWER ${PROJECT_NAME} PROJECT_NAME_LOWER)

# ############################################################################
# these will be removed after upgrading CMake minimum version to 3.9.6
set(PROJECT_DESCRIPTION   "Xt/Motif Gui-toolkit binding for Coin")
# ############################################################################
 
string(TIMESTAMP SO${GUI}_BUILD_YEAR "%Y")
math(EXPR SO${GUI}_SO_VERSION ${PROJECT_VERSION_MAJOR}*20)
set(VERSION ${SO${GUI}_VERSION})

if(POLICY CMP0072)
  # get rid of OpenGL GLVND warning from CMake 3.11
  cmake_policy(SET CMP0072 NEW)
endif()

if(POLICY CMP0075)
  # get rid of CMAKE_REQUIRED_LIBRARIES warning from CMake 3.12
  cmake_policy(SET CMP0075 NEW)
endif()

# ############################################################################
# Prevent in-source builds, as they often cause severe build problems
# ############################################################################

if(CMAKE_SOURCE_DIR STREQUAL CMAKE_BINARY_DIR)
  message(FATAL_ERROR "${CMAKE_PROJECT_NAME} requires an out of source build. Please create a separate build directory and run 'cmake <path_to_${CMAKE_PROJECT_NAME}> [options]' from there.")
endif()

# ############################################################################
# Include necessary submodules
# ############################################################################

include(CheckCXXSourceCompiles)
include(CheckFunctionExists)
include(CheckIncludeFiles)
include(CheckLibraryExists)
include(CheckStructHasMember)
include(CheckSymbolExists)
include(CMakeDependentOption)
include(CMakePackageConfigHelpers)
include(GNUInstallDirs)

# ############################################################################
# Provide options to customise the build
# ############################################################################

option(SO${GUI}_VERBOSE "Verbose build " OFF)
option(COIN_IV_EXTENSIONS "Enable extra Open Inventor extensions" ON)
option(SO${GUI}_BUILD_SHARED_LIBS "Build shared libraries" ON)
option(WITH_STATIC_DEFAULTS "Enable statically linked in default materials" ON)
option(HAVE_SPACENAV_SUPPORT "Enable Space Navigator support" ON)
option(SO${GUI}_BUILD_DOCUMENTATION "Build and install API documentation (requires Doxygen)." ON)

cmake_dependent_option(SO${GUI}_BUILD_INTERNAL_DOCUMENTATION "Document internal code not part of the API." OFF "SO${GUI}_BUILD_DOCUMENTATION" OFF)
cmake_dependent_option(SO${GUI}_BUILD_DOC_MAN "Build So${Gui} man pages." OFF "SO${GUI}_BUILD_DOCUMENTATION" OFF)
cmake_dependent_option(SO${GUI}_BUILD_DOC_QTHELP "Build QtHelp documentation." OFF "SO${GUI}_BUILD_DOCUMENTATION" OFF)
cmake_dependent_option(SO${GUI}_BUILD_DOC_CHM "Build compressed HTML help manual (requires HTML help compiler)" OFF "SO${GUI}_BUILD_DOCUMENTATION" OFF)

cmake_dependent_option(SO${GUI}_BUILD_MAC_FRAMEWORK "Build framework instead of dylib on Mac OS X when ON. Only valid if SO${GUI}_BUILD_SHARED_LIBS is ON." ON "APPLE;NOT IOS;SO${GUI}_BUILD_SHARED_LIBS" OFF)

# ############################################################################
# Find all necessary and optional SoQt dependencies
# ############################################################################

# Fail early if one of the required packages cannot be found

find_package(OpenGL REQUIRED)
find_package(Motif REQUIRED)
find_package(X11 REQUIRED)
# On Mac OS X, GLX is provided as a separate OpenGL implementation, different
# from the standard OpenGL framework which provides support for GLUT and native
# Mac OS X applications.
if (X11_FOUND)
	if (APPLE)
		find_path(X11_GL_INCLUDE_PATH GL/glx.h ${X11_INC_SEARCH_PATH})
		if (NOT X11_GL_INCLUDE_PATH)
			message(FATAL_ERROR "Could not find GL/glx.h")
		endif()
		set(X11_INCLUDE_DIR ${X11_INCLUDE_DIR} ${X11_GL_INCLUDE_PATH})

		find_library(X11_GL_LIB GL ${X11_LIB_SEARCH_PATH})
		if (NOT X11_GL_LIB)
			message(FATAL_ERROR "Could not find libGL.dylib")
		endif()

		find_library(X11_GLU_LIB GLU ${X11_LIB_SEARCH_PATH})
		if (NOT X11_GLU_LIB)
			message(FATAL_ERROR "Could not find libGLU.dylib")
		endif()
	else ()
		set(X11_INCLUDE_DIR ${X11_INCLUDE_DIR} ${OPENGL_INCLUDE_DIR})
		set(X11_GL_LIB ${OPENGL_gl_LIBRARY})
		set(X11_GLU_LIB ${OPENGL_glu_LIBRARY})
	endif ()
endif()

# Coin is typically installed as Inventor.framework on APPLE
find_package(Coin NAMES Inventor Coin REQUIRED)

# ##########################################################################
# Setup build environment
# ##########################################################################

if (X11_Xmu_FOUND)
  set(HAVE_LIBXMU 1)
endif()

if (X11_Xpm_FOUND)
  set(HAVE_LIBXPM 1)
endif()

if (CMAKE_HOST_SYSTEM_NAME STREQUAL "Linux")
  set(HAVE_JOYSTICK_LINUX 1)
endif()

check_include_files(windows.h HAVE_WINDOWS_H)
check_include_files(sys/types.h HAVE_SYS_TYPES_H)

# gets the Motif toolkit version
file(STRINGS "${MOTIF_INCLUDE_DIR}/Xm/Xm.h"  motif REGEX "#define XmVERSION_STRING")
string(REGEX MATCH "Motif Version ([0-9]+)\\.([0-9]+)\\.([0-9]+)" dummy ${motif})
set(MOTIF_VERSION_MAJOR ${CMAKE_MATCH_1})
set(MOTIF_VERSION_MINOR ${CMAKE_MATCH_2})
set(MOTIF_VERSION_PATCH ${CMAKE_MATCH_3})
dump_variable(
MOTIF_VERSION_MAJOR
MOTIF_VERSION_MINOR
MOTIF_VERSION_PATCH
)
set(GUI_TOOLKIT_VERSION ${MOTIF_VERSION_MAJOR}${MOTIF_VERSION_MINOR}${MOTIF_VERSION_PATCH})

check_symbol_exists(__func__ "" FUNC)
check_symbol_exists(__PRETTY_FUNCTION__ "" PRETTY_FUNCTION)
check_symbol_exists(__FUNCTION__ "" FUNCTION)
if(FUNC)
  set(HAVE_C_COMPILER_FUNCTION_NAME_VAR __func__)
  set(HAVE_CPP_COMPILER_FUNCTION_NAME_VAR __func__)
elseif(PRETTY_FUNCTION)
  set(HAVE_C_COMPILER_FUNCTION_NAME_VAR __PRETTY_FUNCTION__)
  set(HAVE_CPP_COMPILER_FUNCTION_NAME_VAR __PRETTY_FUNCTION__)
elseif(FUNCTION)
  set(HAVE_C_COMPILER_FUNCTION_NAME_VAR __FUNCTION__)
  set(HAVE_CPP_COMPILER_FUNCTION_NAME_VAR __FUNCTION__)
endif()
check_include_files(dlfcn.h HAVE_DLFCN_H)

set(CMAKE_REQUIRED_INCLUDES ${X11_INCLUDE_DIR})
set(CMAKE_REQUIRED_LIBRARIES Coin::Coin ${X11_GL_LIB} ${X11_GLU_LIB})

check_cxx_source_compiles("
#include <GL/gl.h>
#include <GL/glx.h>
int main() { (void)glXChooseVisual(0L, 0, 0L); glEnd(); return 0; }
" HAVE_GLX)
#check_library_exists(GL glXChooseVisual "" HAVE_GLX)
if(HAVE_WINDOWS_H)
  check_include_files("windows.h;GL/gl.h" HAVE_GL_GL_H)
  check_include_files("windows.h;GL/glu.h" HAVE_GL_GLU_H)
else()
  check_include_files(GL/gl.h HAVE_GL_GL_H)
  check_include_files(GL/glu.h HAVE_GL_GLU_H)
endif()
check_include_files(OpenGL/gl.h HAVE_OPENGL_GL_H)
check_include_files(OpenGL/glu.h HAVE_OPENGL_GLU_H)

#unset(CMAKE_REQUIRED_INCLUDES)
#unset(CMAKE_REQUIRED_LIBRARIES)

check_include_files(inttypes.h HAVE_INTTYPES_H)
check_include_files(memory.h HAVE_MEMORY_H)
check_include_files(netinet/in.h HAVE_NETINET_IN_H)
if(HAVE_SYS_TYPES_H)
  check_cxx_source_compiles("
    #include <sys/types.h>
    #include <pthread.h>
    int main() { struct timespec timeout; timeout.tv_nsec = 0; return 0; }
  " HAVE_PTHREAD_TIMESPEC_NSEC)
else()
  check_cxx_source_compiles("
    #include <pthread.h>
    int main() { struct timespec timeout; timeout.tv_nsec = 0; return 0; }
  " HAVE_PTHREAD_TIMESPEC_NSEC)
endif()

#set(CMAKE_REQUIRED_LIBRARIES ${Coin_LIBRARY})

check_cxx_source_compiles("
  #include <Inventor/SbImage.h>
  int main() { SbImage::addReadImageCB(NULL, NULL); return 0; }
" HAVE_SBIMAGE_ADDREADIMAGECB)
check_cxx_source_compiles("
  #include <Inventor/nodes/SoPerspectiveCamera.h>
  int main() { SoPerspectiveCamera * c = new SoPerspectiveCamera; c->setStereoMode(SoCamera::MONOSCOPIC); return 0; }
" HAVE_SOCAMERA_SETSTEREOMODE)
check_cxx_source_compiles("
  #include <Inventor/misc/SoContextHandler.h>
  int main() { SoContextHandler::destructingContext(0); return 0; }
" HAVE_SOCONTEXTHANDLER)
check_cxx_source_compiles("
  #include <Inventor/events/SoKeyboardEvent.h>
  int main() { SoKeyboardEvent::Key key = SoKeyboardEvent::DELETE; return 0; }
" HAVE_SOKEYBOARDEVENT_DELETE)
check_cxx_source_compiles("
  #include <Inventor/events/SoMouseButtonEvent.h>
  int main() { SoMouseButtonEvent::Button button = SoMouseButtonEvent::BUTTON5; return 0; }
" HAVE_SOMOUSEBUTTONEVENT_BUTTON5)
check_cxx_source_compiles("
  #include <Inventor/nodes/SoPolygonOffset.h>
  int main() { SoPolygonOffset * p = new SoPolygonOffset; return 0; }
" HAVE_SOPOLYGONOFFSET)
check_cxx_source_compiles("
  #include <Inventor/actions/SoGLRenderAction.h>
  int main() { int num = (int) SoGLRenderAction::SORTED_LAYERS_BLEND; return 0; }
" HAVE_SORTED_LAYERS_BLEND)
check_cxx_source_compiles("
  #include <Inventor/VRMLnodes/SoVRMLBackground.h>
  int main() { SoVRMLBackground * p = new SoVRMLBackground; return 0; }
" HAVE_SOVRMLBACKGROUND)
check_cxx_source_compiles("
  #include <Inventor/VRMLnodes/SoVRMLFog.h>
  int main() { SoVRMLFog * p = new SoVRMLFog; return 0; }
" HAVE_SOVRMLFOG)
check_cxx_source_compiles("
  #include <Inventor/VRMLnodes/SoVRMLMaterial.h>
  int main() { SoVRMLMaterial * p = new SoVRMLMaterial; return 0; }
" HAVE_SOVRMLMATERIAL)
check_cxx_source_compiles("
  #include <Inventor/VRMLnodes/SoVRMLViewpoint.h>
  int main() { SoVRMLViewpoint * p = new SoVRMLViewpoint; return 0; }
" HAVE_SOVRMLVIEWPOINT)

unset(CMAKE_REQUIRED_LIBRARIES)

check_include_files(stdint.h HAVE_STDINT_H)
check_include_files(stdlib.h HAVE_STDLIB_H)
check_include_files(strings.h HAVE_STRINGS_H)
check_include_files(string.h HAVE_STRING_H)
check_include_files(sys/stat.h HAVE_SYS_STAT_H)
check_include_files(sys/time.h HAVE_SYS_TIME_H)
check_include_files(unistd.h HAVE_UNISTD_H)
check_include_files("assert.h;ctype.h;errno.h;float.h;limits.h;locale.h;math.h;setjmp.h;signal.h;stdarg.h;stddef.h;stdio.h;stdlib.h;string.h;time.h" STDC_HEADERS)

if(HAVE_WINDOWS_H)
  check_include_files("windows.h;tlhelp32.h" HAVE_TLHELP32_H)
  check_cxx_source_compiles("
    #include <windows.h>
    int main() {
      CreateDirectory(NULL, NULL);
      RemoveDirectory(NULL);
      SetLastError(0);
      GetLastError();
      LocalAlloc(0, 1);
      LocalFree(NULL);
      return 0;
    }
  " HAVE_WIN32_API)
  check_symbol_exists(LoadLibrary windows.h HAVE_WIN32_LOADLIBRARY)
  if(HAVE_WIN32_LOADLIBRARY)
    set(HAVE_DYNAMIC_LINKING 1)
  endif()
  check_symbol_exists(GetEnvironmentVariable windows.h HAVE_GETENVIRONMENTVARIABLE)
endif()
set(USE_EXCEPTIONS ON)
set(X_DISPLAY_MISSING 1)
check_include_files(X11/Xlib.h HAVE_X11_AVAILABLE)
if(HAVE_X11_AVAILABLE)
  set(X_DISPLAY_MISSING 0)
  check_include_files(X11/extensions/SGIMisc.h HAVE_X11_EXTENSIONS_SGIMISC_H)
  check_include_files(X11/extensions/XInput.h HAVE_X11_EXTENSIONS_XINPUT_H)
  check_include_files(X11/Xproto.h HAVE_X11_XPROTO_H)
endif()
unset(CMAKE_REQUIRED_INCLUDES)

set(PACKAGE ${PROJECT_NAME})
set(PACKAGE_DESCRIPTION "${PROJECT_DESCRIPTION}")
set(PACKAGE_BUGREPORT "coin-support@coin3d.org")
set(PACKAGE_NAME ${PROJECT_NAME})
set(PACKAGE_STRING "${PROJECT_NAME} ${PROJECT_VERSION}")
set(PACKAGE_TARNAME ${PROJECT_NAME_LOWER})
set(PACKAGE_URL "https://bitbucket.org/Coin3D/${PROJECT_NAME}")
set(PACKAGE_VERSION ${PROJECT_VERSION})
set(PACKAGE_HOST ${CMAKE_HOST_SYSTEM_PROCESSOR}-${CMAKE_HOST_SYSTEM_NAME})
set(PACKAGE_COMPILER ${CMAKE_CXX_COMPILER})
set(PACKAGE_REQUIREMENTS "Coin, ${PACKAGE_ADDITIONAL_REQUIREMENTS}")

# ############################################################################
# Setup targets in subdirectories
# ############################################################################

add_subdirectory(data)
add_subdirectory(src)
##### small test programs (to be run interactively)
add_subdirectory(test-code)

############################################################################
# New CPACK section, please see the README file inside cpack.d directory.
add_subdirectory(cpack.d)
