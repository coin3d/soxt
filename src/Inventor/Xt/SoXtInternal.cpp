/**************************************************************************\
 *
 *  This file is part of the Coin family of 3D visualization libraries.
 *  Copyright (C) 1998-2003 by Systems in Motion.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and / or
 *  modify it under the terms of the GNU General Public License
 *  version 2 as published by the Free Software Foundation.  See the
 *  file LICENSE.GPL at the root directory of this source distribution
 *  for more details.
 *
 *  If you desire to use this library in software that is incompatible
 *  with the GNU GPL, and / or you would like to take advantage of the
 *  additional benefits with regard to our support services, please
 *  contact Systems in Motion about acquiring a Coin Professional
 *  Edition License.  See <URL:http://www.coin3d.org> for more
 *  information.
 *
 *  Systems in Motion, Abels gate 5, Teknobyen, 7030 Trondheim, NORWAY
 *  <URL:http://www.sim.no>, <mailto:support@sim.no>
 *
\**************************************************************************/

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H

#include <assert.h>

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

// *************************************************************************

static const char *
debug_visualclassname(const int vclass)
{
  switch (vclass) {
  case StaticGray:   return "StaticGray";
  case GrayScale:    return "GrayScale";
  case StaticColor:  return "StaticColor";
  case PseudoColor:  return "PseudoColor";
  case TrueColor:    return "TrueColor";
  case DirectColor:  return "DirectColor";
  default:           return "<unknown>";
  }
}

static void
debug_dumpvisualinfo(const XVisualInfo * v)
{
  SoDebugError::postInfo("debug_dumpvisualinfo",
                         "Visual==%p, VisualID==%d, screen==%d, "
                         "depth==%d, class==%s, "
                         "rgb masks==[0x%04x, 0x%04x, 0x%04x], "
                         "colormap_size==%d, bits_per_rgb==%d",
                         v->visual,
                         v->visualid,
                         v->screen,
                         v->depth,
                         debug_visualclassname(v->c_class),
                         v->red_mask, v->green_mask, v->blue_mask,
                         v->colormap_size,
                         v->bits_per_rgb);
}

static void
debug_dumpvisualinfo(Display * d, Visual * v)
{
  XVisualInfo xvitemplate;
  xvitemplate.visualid = XVisualIDFromVisual(v);
  int nrret;
  XVisualInfo * xvi = XGetVisualInfo(d, VisualIDMask, &xvitemplate, &nrret);
  assert(nrret == 1);
  debug_dumpvisualinfo(xvi);
}

static void
debug_dumpallvisualsinfo(Display * d)
{
  int i, num;
  XVisualInfo templ;
  XVisualInfo * all = XGetVisualInfo(d, VisualNoMask, &templ, &num);
  SoDebugError::postInfo("debug_dumpallvisualinfo",
                         "%d available visual%s, dumping:",
                         num, num == 1 ? "" : "s");
  for ( i = 0; i < num; i++ ) { debug_dumpvisualinfo(&all[i]); }
  XFree((char *) all);
}

/*
  This function tries to find the best visual type, depth, and
  colormap combination for the display.  The display argument may be
  given, or set to \c NULL - then the default display will be used and
  returned.

  When SoXt doesn't get it's graphics displayed correctly, this
  routine may be the root of the problem.  Let's hope it solves more
  problems than it introduces...

  Misc Links:

  "Beyond the Default Visual", by John Cwikla
    http://www.motifzone.com/tmd/articles/DefaultVisual/DefaultVisual.html

  This function is not part of the original SGI InventorXt API.
*/


#if 1
// This strategy can hopefully be obsoleted for the alternative approach
// implemented below.  2003-04-09 larsa
void
SoXtInternal::selectBestVisual(Display * dpy, Visual * & visual,
                       Colormap & colormap, int & depth)
{
  assert(dpy != NULL);

#ifdef USE_DEFAULT_VISUAL
  const int screen = DefaultScreen(dpy);
  visual = DefaultVisual(dpy, screen);
  colormap = DefaultColormap(dpy, screen);
  depth = DefaultDepth(dpy, screen);
  return;
#endif

  int DEBUG_VISUAL = 0;
  const char * env = SoAny::getenv("SOXT_DEBUG_VISUAL");
  if ( env ) DEBUG_VISUAL = atoi(env);

  unsigned int wantedid = 0;
  env = SoAny::getenv("SOXT_SELECT_VISUAL");
  if ( env ) {
    wantedid = atoi(env);
    if ( DEBUG_VISUAL ) {
      SoDebugError::postInfo("SoXt::selectBestVisual", "will try to get visual with id %d", wantedid);
    }
  }

  // Dump all visuals available on the X server.
  if ( wantedid == 0 && DEBUG_VISUAL ) {
    static SbBool first = TRUE;
    if ( first ) {
      first = FALSE;
      debug_dumpallvisualsinfo(dpy);
    }
  }
  if ( wantedid != 0 ) { // we have a specific requests
    int num;
    XVisualInfo templ;
    templ.c_class = TrueColor;
    XVisualInfo * all = XGetVisualInfo(dpy, VisualNoMask, &templ, &num);
    for (int i=0; i < num; i++) {
      if ( all[i].visualid == wantedid ) {
        visual = all[i].visual;
        depth = all[i].depth;
        int numcmaps;
        XStandardColormap * stdcolormaps = NULL;

        if (XmuLookupStandardColormap(dpy, all[i].screen, all[i].visualid,
                                      all[i].depth, XA_RGB_DEFAULT_MAP,
                                      False, True)
            && XGetRGBColormaps(dpy, RootWindow(dpy, all[i].screen),
                                &stdcolormaps, &numcmaps, XA_RGB_DEFAULT_MAP)) {
          SbBool found = FALSE;
          for (int j = 0; j < numcmaps && ! found; j++) {
            if (stdcolormaps[j].visualid == all[i].visualid) {
              if ( DEBUG_VISUAL )
                SoDebugError::postInfo("SoXtInternal::selectBestVisual[1]", "found standard colormap");
              colormap = stdcolormaps[j].colormap;
              found = TRUE;
            }
          }
          if (! found) {
            colormap = XCreateColormap(dpy, RootWindow(dpy, all[i].screen), all[i].visual, AllocNone);
            if ( DEBUG_VISUAL )
              SoDebugError::postInfo("SoXtInternal::selectBestVisual[1]",
                                     "standard RGB colormaps did not work with visual - created own colormap %d", colormap);
          }
        }
        else {
          colormap = XCreateColormap(dpy, RootWindow(dpy, all[i].screen), all[i].visual, AllocNone);
          if ( DEBUG_VISUAL )
            SoDebugError::postInfo("SoXtInternal::selectBestVisual[1]", "no standard RGB colormaps - created own colormap %d", colormap);
        }
        XtFree((char *) stdcolormaps);
        return;
      }
    }
    SoDebugError::postInfo("SoXtInternal::selectBestVisual", "could not find requested visual with id %d", wantedid);
  }

  int snum = XDefaultScreen(dpy);
  if ( XDefaultDepth(dpy, snum) >= 16 ) { // me like...
    visual = XDefaultVisual(dpy, snum);
    if ( DEBUG_VISUAL ) {
      SoDebugError::postInfo("SoXtInternal::selectBestVisual", "using default visual:");
      debug_dumpvisualinfo(dpy, visual);
    }
    depth = XDefaultDepth(dpy, snum);
    colormap = XDefaultColormap(dpy, snum);
    return;
  }

  static struct {
    int depth;
    int vclass;
  } pri[] = { // how things are prioritized
    { 24, TrueColor },
    { 24, DirectColor },
    { 24, PseudoColor },
    { 16, TrueColor },
    { 16, DirectColor },
    { 16, PseudoColor },
    { 15, TrueColor },
    { 15, DirectColor },
    { 15, PseudoColor },
    { 12, TrueColor },
    { 12, DirectColor },
    { 12, PseudoColor },
    { 8, PseudoColor },
    { 0, 0 }
  };

  XVisualInfo vinfo;
  for (int i = 0; pri[i].depth != 0; i++) {
    if (XMatchVisualInfo(dpy, snum, pri[i].depth, pri[i].vclass, &vinfo)) {
      visual = vinfo.visual;
      if ( DEBUG_VISUAL ) {
        SoDebugError::postInfo("SoXtInternal::selectBestVisual", "found visual to use:");
        debug_dumpvisualinfo(&vinfo);
      }
      depth = vinfo.depth;

      int numcmaps;
      XStandardColormap * stdcolormaps = NULL;

#ifdef HAVE_LIBXMU
      if (XmuLookupStandardColormap(dpy, vinfo.screen, vinfo.visualid,
                                    vinfo.depth, XA_RGB_DEFAULT_MAP,
                                    False, True)
          && XGetRGBColormaps(dpy, RootWindow(dpy, vinfo.screen),
                              &stdcolormaps, &numcmaps, XA_RGB_DEFAULT_MAP)) {
        SbBool found = FALSE;
        for (i = 0; i < numcmaps && ! found; i++) {
          if (stdcolormaps[i].visualid == vinfo.visualid) {
            colormap = stdcolormaps[i].colormap;
            found = TRUE;
          }
        }
        if (! found) {
          SoDebugError::postInfo("SoXtInternal::selectBestVisual",
                                 "standard RGB colormaps did not work with visual - creating own colormap");
          colormap = XCreateColormap(dpy, RootWindow(dpy, vinfo.screen),
                                     vinfo.visual, AllocNone);
        }
      }
      else {
        SoDebugError::postInfo("SoXtInternal::selectBestVisual",
                               "no standard RGB colormaps - creating own colormap");
        colormap = XCreateColormap(dpy, RootWindow(dpy, vinfo.screen),
                                   vinfo.visual, AllocNone);
      }
      XtFree((char *) stdcolormaps);
#else
      SoDebugError::postInfo("SoXtInternal::selectBestVisual",
                             "SoXt does not support detecting best visual/colormap without the Xmu library (yet)");
      // FIXME: couldn't we do better than this? Exiting here doesn't
      // seem particularly robust.. 20020117 mortene.
      exit(1);
#endif /* ! HAVE_LIBXMU */
      return;
    }
  }
  // FIXME: didn't find visual - what should we do?
  SoDebugError::postInfo("SoXtInternal::selectBestVisual", "no visual found");
  exit(1);
}
#else
/*
  The DirectColor class of X Visuals are not handled correctly yet.
  TrueColor does not seem to be a problem.  The default Visual may
  be of class DirectColor, so we don't default to using the default
  any more, but try to obtain a TrueColor visual first.
*/
void
SoXtInternal::selectBestVisual(Display * dpy, Visual * & visual, Colormap & colormap, int & depth)
{
  assert(dpy != NULL);

  int DEBUG_VISUAL = 0;
  const char * env = SoAny::getenv("SOXT_DEBUG_VISUAL");
  if ( env ) DEBUG_VISUAL = atoi(env);

  unsigned int wanted = 0;
  env = SoAny::getenv("SOXT_SELECT_VISUAL");
  if ( env ) {
    wanted = atoi(env);
    if ( DEBUG_VISUAL ) {
      SoDebugError::postInfo("SoXtInternal::selectBestVisual", "will try to get visual with id %d", wanted);
    }
  }

  // Dump all visuals available on the X server.
  // (or run "glxinfo -t" to get a list of visuals and their ids...)
  if ( wanted == 0 && DEBUG_VISUAL ) {
    static SbBool first = TRUE;
    if ( first ) {
      first = FALSE;
      debug_dumpallvisualsinfo(dpy);
    }
  }

  int i, numvisuals, idx;
  depth = 0;
  idx = -1;
  XVisualInfo templ;
  XVisualInfo * visuals = XGetVisualInfo(dpy, VisualNoMask, &templ, &numvisuals);
  for ( i = 0; i < numvisuals; i++ ) {
    if ( visuals[i].visualid == wanted ) {
      idx = i;
      goto selected;
    }
    if ( visuals[i].c_class != TrueColor ) continue;
    if ( visuals[i].depth <= depth ) continue;
    idx = i;
    depth = visuals[i].depth;
  }
  if ( depth < 24 ) {
    for ( i = 0; i < numvisuals; i++ ) {
      if ( visuals[i].c_class != DirectColor ) continue;
      if ( visuals[i].depth <= depth ) continue;
      idx = i;
      depth = visuals[i].depth;
    }
    for ( i = 0; i < numvisuals; i++ ) {
      if ( visuals[i].c_class != PseudoColor ) continue;
      if ( visuals[i].depth <= depth ) continue;
      idx = i;
      depth = visuals[i].depth;
    }
  }

selected:
  if ( idx == -1 ) {
    SoDebugError::postInfo("SoXtInternal::selectBestVisual", "did not find a satisfactory visual");
    exit(1);
  }

  visual = visuals[idx].visual;
  depth = visuals[idx].depth;
  int numcmaps;
  XStandardColormap * stdcolormaps = NULL;
  if ( DEBUG_VISUAL ) {
    SoDebugError::postInfo("SoXtInternal::selectBestVisual", "found visual to use:");
    debug_dumpvisualinfo(&visuals[idx]);
  }

#ifdef HAVE_LIBXMU
  if ( XmuLookupStandardColormap(dpy, visuals[idx].screen, visuals[idx].visualid, visuals[idx].depth,
                                 XA_RGB_DEFAULT_MAP, False, True) &&
       XGetRGBColormaps(dpy, RootWindow(dpy, visuals[idx].screen),
                        &stdcolormaps, &numcmaps, XA_RGB_DEFAULT_MAP) ) {
    for (i = 0; i < numcmaps; i++) {
      if (stdcolormaps[i].visualid == visuals[idx].visualid) {
        colormap = stdcolormaps[i].colormap;
        goto cleanup;
      }
    }
    colormap = XCreateColormap(dpy, RootWindow(dpy, visuals[idx].screen), visuals[idx].visual, AllocNone);
    if ( DEBUG_VISUAL )
      SoDebugError::postInfo("SoXtInternal::selectBestVisual", "standard RGB colormaps did not work with visual - created own (%d)", colormap);
  } else {
    colormap = XCreateColormap(dpy, RootWindow(dpy, visuals[idx].screen), visuals[idx].visual, AllocNone);
    if ( DEBUG_VISUAL )
      SoDebugError::postInfo("SoXtInternal::selectBestVisual", "no standard RGB colormaps - created own (%d)", colormap);
  }
#else
  SoDebugError::postInfo("SoXtInternal::selectBestVisual",
                         "SoXt does not support detecting best visual/colormap without the Xmu library (yet)");
  // FIXME: couldn't we do better than this? Exiting here doesn't
  // seem particularly robust.. 20020117 mortene.
  exit(1);
#endif /* ! HAVE_LIBXMU */

cleanup:
  XFree((char *) visuals);
  if ( stdcolormaps ) XtFree((char *) stdcolormaps);
}
#endif

// *************************************************************************
