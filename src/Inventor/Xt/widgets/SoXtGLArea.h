/*
 * (c) Copyright 1993, Silicon Graphics, Inc.
 * ALL RIGHTS RESERVED 
 * Permission to use, copy, modify, and distribute this software for 
 * any purpose and without fee is hereby granted, provided that the above
 * copyright notice appear in all copies and that both the copyright notice
 * and this permission notice appear in supporting documentation, and that 
 * the name of Silicon Graphics, Inc. not be used in advertising
 * or publicity pertaining to distribution of the software without specific,
 * written prior permission. 
 *
 * THE MATERIAL EMBODIED ON THIS SOFTWARE IS PROVIDED TO YOU "AS-IS"
 * AND WITHOUT WARRANTY OF ANY KIND, EXPRESS, IMPLIED OR OTHERWISE,
 * INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF MERCHANTABILITY OR
 * FITNESS FOR A PARTICULAR PURPOSE.  IN NO EVENT SHALL SILICON
 * GRAPHICS, INC.  BE LIABLE TO YOU OR ANYONE ELSE FOR ANY DIRECT,
 * SPECIAL, INCIDENTAL, INDIRECT OR CONSEQUENTIAL DAMAGES OF ANY
 * KIND, OR ANY DAMAGES WHATSOEVER, INCLUDING WITHOUT LIMITATION,
 * LOSS OF PROFIT, LOSS OF USE, SAVINGS OR REVENUE, OR THE CLAIMS OF
 * THIRD PARTIES, WHETHER OR NOT SILICON GRAPHICS, INC.  HAS BEEN
 * ADVISED OF THE POSSIBILITY OF SUCH LOSS, HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, ARISING OUT OF OR IN CONNECTION WITH THE
 * POSSESSION, USE OR PERFORMANCE OF THIS SOFTWARE.
 * 
 * 
 * US Government Users Restricted Rights 
 * Use, duplication, or disclosure by the Government is subject to
 * restrictions set forth in FAR 52.227.19(c)(2) or subparagraph
 * (c)(1)(ii) of the Rights in Technical Data and Computer Software
 * clause at DFARS 252.227-7013 and/or in similar or successor
 * clauses in the FAR or the DOD or NASA FAR Supplement.
 * Unpublished-- rights reserved under the copyright laws of the
 * United States.  Contractor/manufacturer is Silicon Graphics,
 * Inc., 2011 N.  Shoreline Blvd., Mountain View, CA 94039-7311.
 *
 * OpenGL(TM) is a trademark of Silicon Graphics, Inc.
 */

/*
 * This file has been heavily modified from the original.  It has been
 * stripped down and moved into the SoXt namespace to avoid potential
 * name-collisions with externally installed GL widgets, and all pure
 * Xt vs. Xt/Motif ifdef wrappers have been removed (only the Motif
 * parts remain).  You are encouraged to rather go back to the source,
 * which you will find with Mesa, than to base your GL widget derivation
 * on this code.
 *
 *     Lars J. Aas <larsa@sim.no>,
 *     19th may 2000
 */

/* $Id$ */

#ifndef SOXT_GLAREA_H
#define SOXT_GLAREA_H

/* GL/glx.h includes X11/Xmd.h which contains typedefs for BOOL and
 * INT32 that conflict with the definitions in windef.h (which is
 * included from windows.h, which may be included from
 * Inventor/system/gl.h).  To avoid this conflict, we rename the
 * typedefs done in X11/Xmd.h to use other names (tempbool and
 * tempint32), and try to clean up the hack after the header has been
 * parsed.  2003-06-25 larsa */
#ifndef BOOL
#define BOOL tempbool
#define COIN_DEFINED_BOOL
#endif /* !BOOL */
#ifndef INT32
#define INT32 tempint32
#define COIN_DEFINED_INT32
#endif /* !INT32 */
#include <GL/glx.h>
/* This is the cleanup part of the X11/Xmd.h conflict fix hack set up
 * above.  2003-06-25 larsa */
#ifdef COIN_DEFINED_BOOL
#undef BOOL
#undef COIN_DEFINED_BOOL
#endif /* COIN_DEFINED_BOOL */
#ifdef COIN_DEFINED_INT32
#undef INT32
#undef COIN_DEFINED_INT32
#endif /* COIN_DEFINED_INT32 */

#include <GL/gl.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* ********************************************************************** */

/* Resources:

 Name		     Class		RepType		Default Value
 ----		     -----		-------		-------------
 attribList	     AttribList		int *		NULL
 visualInfo	     VisualInfo		VisualInfo	NULL
 installColormap     InstallColormap	Boolean		TRUE
 allocateBackground  AllocateColors	Boolean		FALSE
 allocateOtherColors AllocateColors	Boolean		FALSE
 installBackground   InstallBackground	Boolean		TRUE
 exposeCallback      Callback		Pointer		NULL
 ginitCallback       Callback		Pointer		NULL
 inputCallback       Callback		Pointer		NULL
 resizeCallback      Callback		Pointer		NULL

*** The following resources all correspond to the GLX configuration
*** attributes and are used to create the attribList if it is NULL
 bufferSize	     BufferSize		int		0
 level		     Level		int		0
 rgba		     Rgba		Boolean		FALSE
 doublebuffer	     Doublebuffer	Boolean		FALSE
 stereo		     Stereo		Boolean		FALSE
 auxBuffers	     AuxBuffers		int		0
 redSize	     ColorSize		int		1
 greenSize	     ColorSize		int		1
 blueSize	     ColorSize		int		1
 alphaSize	     AlphaSize		int		0
 depthSize	     DepthSize		int		0
 stencilSize	     StencilSize	int		0
 accumRedSize	     AccumColorSize	int		0
 accumGreenSize	     AccumColorSize	int		0
 accumBlueSize	     AccumColorSize	int		0
 accumAlphaSize	     AccumAlphaSize	int		0
*/

/* ********************************************************************** */

#define SoXtNattribList           "attribList"
#define SoXtCAttribList           "AttribList"
#define SoXtNvisualInfo           "visualInfo"
#define SoXtCVisualInfo           "VisualInfo"
#define SoXtRVisualInfo           "VisualInfo"

#define SoXtNinstallColormap      "installColormap"
#define SoXtCInstallColormap      "InstallColormap"
#define SoXtNallocateBackground   "allocateBackground"
#define SoXtNallocateOtherColors  "allocateOtherColors"
#define SoXtCAllocateColors       "AllocateColors"
#define SoXtNinstallBackground    "installBackground"
#define SoXtCInstallBackground    "InstallBackground"

#define SoXtCCallback             "Callback"
#define SoXtNexposeCallback       "exposeCallback"
#define SoXtNginitCallback        "ginitCallback"
#define SoXtNresizeCallback       "resizeCallback"
#define SoXtNinputCallback        "inputCallback"

#define SoXtNbufferSize           "bufferSize"
#define SoXtCBufferSize           "BufferSize"
#define SoXtNlevel                "level"
#define SoXtCLevel                "Level"
#define SoXtNrgba                 "rgba"
#define SoXtCRgba                 "Rgba"
#define SoXtNdoublebuffer         "doublebuffer"
#define SoXtCDoublebuffer         "Doublebuffer"
#define SoXtNstereo               "stereo"
#define SoXtCStereo               "Stereo"
#define SoXtNauxBuffers           "auxBuffers"
#define SoXtCAuxBuffers           "AuxBuffers"
#define SoXtNredSize              "redSize"
#define SoXtNgreenSize            "greenSize"
#define SoXtNblueSize             "blueSize"
#define SoXtCColorSize            "ColorSize"
#define SoXtNalphaSize            "alphaSize"
#define SoXtCAlphaSize            "AlphaSize"
#define SoXtNdepthSize            "depthSize"
#define SoXtCDepthSize            "DepthSize"
#define SoXtNstencilSize          "stencilSize"
#define SoXtCStencilSize          "StencilSize"
#define SoXtNaccumRedSize         "accumRedSize"
#define SoXtNaccumGreenSize       "accumGreenSize"
#define SoXtNaccumBlueSize        "accumBlueSize"
#define SoXtCAccumColorSize       "AccumColorSize"
#define SoXtNaccumAlphaSize       "accumAlphaSize"
#define SoXtCAccumAlphaSize       "AccumAlphaSize"

#ifndef SoXtNrefresh
#define SoXtNrefresh              "refresh"
#endif
#ifndef SoXtCRefresh
#define SoXtCRefresh              "Refresh"
#endif

typedef  struct _SoXtGLAreaClassRec *  SoXtGLAreaWidgetClass;
typedef  struct _SoXtGLAreaRec *       SoXtGLAreaWidget;

extern WidgetClass soxtGLAreaWidgetClass;

/* Callback reasons */
#define SoXtCR_EXPOSE    XmCR_EXPOSE
#define SoXtCR_RESIZE    XmCR_RESIZE
#define SoXtCR_INPUT     XmCR_INPUT

#define SoXtCR_GINIT     31975	/* Arbitrary number that should never clash */

typedef struct {
  int       reason;
  XEvent  * event;
  Dimension width;
  Dimension height;
} SoXtGLAreaCallbackStruct;

void SoXtGLAreaMakeCurrent(Widget w, GLXContext ctx);
void SoXtGLAreaSwapBuffers(Widget w);

#ifndef XtIsSoXtGLArea
#define XtIsSoXtGLArea(w) XtIsSubclass(w, soxtGLAreaWidgetClass)
#endif /* ! SoXtIsGLArea */

/* ********************************************************************** */

#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */

#endif /* ! SOXT_GLAREA_H */
