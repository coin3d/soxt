/**************************************************************************
 *
 *  This file is part of the Coin SoXt GUI binding library.
 *  Copyright (C) 2000 by Systems in Motion.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License version
 *  2.1 as published by the Free Software Foundation.  See the file
 *  LICENSE.LGPL at the root directory of the distribution for all the
 *  details.
 *
 *  If you want to use Coin SoXt for applications not compatible with the
 *  LGPL, please contact SIM to acquire a Professional Edition License.
 *
 *  Systems in Motion, Prof Brochs gate 6, N-7030 Trondheim, NORWAY
 *  http://www.sim.no/ support@sim.no Voice: +47 22114160 Fax: +47 22207097
 *
 **************************************************************************/

#if HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H

#if HAVE_LIBXPM
#include <X11/xpm.h>
#endif // HAVE_LIBXPM

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

#include <assert.h>
#include <soxtdefs.h>
#include <SoXtInternal.h>
#include <Inventor/errors/SoDebugError.h>


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

  Display * dpy = XtDisplay(widget);
  Screen * screen = XtScreen(widget);

  XpmAttributes attrs;

  attrs.colormap = XDefaultColormapOfScreen(screen);
  assert(attrs.colormap != 0);
  attrs.visual = XDefaultVisualOfScreen(screen);
  assert(attrs.visual != (Visual *)NULL);
  attrs.depth = XDefaultDepthOfScreen(screen);

  attrs.valuemask = XpmVisual | XpmColormap | XpmDepth;

  Drawable draw = XRootWindowOfScreen(screen);
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
