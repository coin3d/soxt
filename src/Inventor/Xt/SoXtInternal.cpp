/**************************************************************************\
 * Copyright (c) Kongsberg Oil & Gas Technologies AS
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 * 
 * Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 * 
 * Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 * 
 * Neither the name of the copyright holder nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
\**************************************************************************/

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H

#include <assert.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Intrinsic.h>

#ifdef HAVE_LIBXPM
#include <X11/xpm.h>
#endif // HAVE_LIBXPM

#ifdef HAVE_LIBXMU
#include <X11/Xmu/Xmu.h>
#include <X11/Xmu/StdCmap.h>
#endif // HAVE_LIBXMU

#include <Xm/Xm.h>
#include <Xm/Form.h>
#include <Xm/Frame.h>
#include <Xm/Label.h>
#include <Xm/LabelG.h>
#include <Xm/PushB.h>
#include <Xm/ToggleB.h>
#include <Xm/DialogS.h>
#include <Xm/RowColumn.h>
#include <Xm/Text.h>
#include <Xm/TextF.h>
#include <Xm/Scale.h>

#include <soxtdefs.h>
#include <Inventor/errors/SoDebugError.h>

#include <Inventor/Xt/SoXtInternal.h>
#include <Inventor/Xt/SoAny.h>

char * SoXtInternal::appname = NULL;
char * SoXtInternal::appclass = NULL;

// *************************************************************************

/*
  \internal
*/
const char *
SoXtInternal::xpmErrorString(int error)
{
#ifdef HAVE_LIBXPM
  switch (error) {
  case XpmSuccess:      return "success";
  case XpmColorError:   return "color error";
  case XpmOpenFailed:   return "open failed";
  case XpmFileInvalid:  return "file invalid";
  case XpmNoMemory:     return "no memory";
  case XpmColorFailed:  return "color failed";
  default:              return "<unknown>";
  }
#endif
  return "no libxpm";
}


/*
  \internal
  Does nothing if libXpm use hasn't been enabled.
*/
Pixmap
SoXtInternal::createPixmapFromXpm(Widget widget, const char ** xpm, SbBool ghost)
{
  Pixmap pixels = 0;
#if HAVE_LIBXPM
  Widget shell = widget;
  while (! XtIsShell(shell) && shell != (Widget) NULL) {
    shell = XtParent(shell);
  }
  assert(shell != (Widget) NULL);
  Display * dpy = XtDisplay(shell);

  XpmAttributes attrs;
  attrs.visual = NULL;
  attrs.colormap = 0;
  attrs.depth = 0;

  XtVaGetValues(shell,
                XmNcolormap, &attrs.colormap,
                XmNdepth,    &attrs.depth,
                XmNvisual,   &attrs.visual,
                NULL);

  // CAT_MOD
  if(!attrs.visual) {
    int snum = XDefaultScreen(dpy);
    attrs.visual = XDefaultVisual(dpy, snum);
  }

  attrs.valuemask = XpmVisual | XpmColormap | XpmDepth;

  Drawable draw = RootWindow(dpy, DefaultScreen(dpy));
  Pixmap stencil = 0;

  // FIXME: that cast is pretty nasty -- get rid of it. 20020319 mortene.
  int error = XpmCreatePixmapFromData(dpy, draw, (char **)xpm,
                                      &pixels, &stencil, &attrs);

  if (error != XpmSuccess) {
#if SOXT_DEBUG
    SoDebugError::postInfo("SoXtInternal::createPixmapFromXpm",
                           "XpmCreatePixmapFromData() failed: %s",
                           SoXtInternal::xpmErrorString(error));
#endif // SOXT_DEBUG
    return (Pixmap)0;
  }

  if (stencil) {
    Pixel bg;
    XtVaGetValues(widget, XmNbackground, &bg, NULL);

    XImage * pixmap = XGetImage(dpy, pixels, 0, 0, attrs.width, attrs.height,
                                0xffffffff, ZPixmap);
    XImage * mask = XGetImage(dpy, stencil, 0, 0, attrs.width, attrs.height,
                              0xffffffff, ZPixmap);
    assert(pixmap != NULL && mask != NULL);

    for (unsigned int x = 0; x < attrs.width; x++) {
      for (unsigned int y = 0; y < attrs.height; y++) {
        Pixel pixel = XGetPixel(mask, x, y);
        Bool usebg = (pixel == 0);
        if (ghost && !usebg) { usebg = ((x+y) % 2) == 1; }
        if (usebg) { // background must be set in image
          XPutPixel(pixmap, x, y, bg);
        }
      }
    }

    GC temp = XCreateGC(dpy, pixels, 0, NULL);
    XPutImage(dpy, pixels, temp, pixmap,
              0, 0, 0, 0, attrs.width, attrs.height);
    XFreeGC(dpy, temp);

    XDestroyImage(pixmap);
    XDestroyImage(mask);
  }

#endif // HAVE_LIBXPM
  return pixels;
}

void
SoXtInternal::setAppName(const char * appname)
{
  if ( SoXtInternal::appname != NULL ) {
    free(SoXtInternal::appname);
    SoXtInternal::appname = NULL;
  }
  if ( appname )
    SoXtInternal::appname = strcpy(new char [strlen(appname) + 1], appname);
}

void
SoXtInternal::setAppClass(const char * appclass)
{
  if ( SoXtInternal::appclass != NULL ) {
    free(SoXtInternal::appclass);
    SoXtInternal::appclass = NULL;
  }
  if ( appclass )
    SoXtInternal::appclass = strcpy(new char [strlen(appclass) + 1], appclass);
}

const char *
SoXtInternal::getAppName(void)
{
  return SoXtInternal::appname;
}

const char *
SoXtInternal::getAppClass(void)
{
  return SoXtInternal::appclass;
}

void
SoXtInternal::selectBestVisual(Display * dpy, Visual * & visual,
                       Colormap & colormap, int & depth)
{
  assert(dpy != NULL);
  const int screen = DefaultScreen(dpy);
  visual = DefaultVisual(dpy, screen);
  colormap = DefaultColormap(dpy, screen);
  depth = DefaultDepth(dpy, screen);
  return;
}

