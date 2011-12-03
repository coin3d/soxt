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

#include <assert.h>
#include <stdio.h>

#include <Inventor/errors/SoDebugError.h>

#include <soxtdefs.h>
#include <Inventor/Xt/SoXtBasic.h>
#include <Inventor/Xt/widgets/SoAnyThumbWheel.h>
#include <Inventor/Xt/widgets/SoXtThumbWheelP.h>

/*
  FIXME :
  - use a virtual Colormap instead of creating graphics the way it is
  hardcoded now.
  - share a cache of pixmaps instead of having pixmaps generated for
  each thumbwheel
*/

// *************************************************************************
// RESOURCES

#define offset(field) \
  XtOffsetOf(SoXtThumbWheelRec, thumbwheel.field)

static float default_value = 0.0f;

static
XtResource
SoXtThumbWheel_resources[] = {
  {
    XmNorientation, XmCOrientation, XmROrientation,
    sizeof(int), offset(orientation),
    XmRImmediate, (XtPointer) XmNO_ORIENTATION
  },
  //  {
  //    XmNforeground, XmCForeground, XmRPixel,
  //    sizeof(Pixel), offset(foreground),
  //    XmRCallProc, (XtPointer) _XmSelectColorDefault
  //  },
  // insensitiveForeground
  // shadowThickness
  // highlightThickness
  {
    SoXtNrefresh, SoXtCRefresh, XmRBoolean,
    sizeof(Boolean), offset(refresh),
    XmRImmediate, (XtPointer) False
  }, {
    XmNarmCallback, XmCCallback, XmRCallback,
    sizeof(XtCallbackList), offset(arm_callback),
    XmRPointer, (XtPointer) NULL
  }, {
    XmNdisarmCallback, XmCCallback, XmRCallback,
    sizeof(XtCallbackList), offset(disarm_callback),
    XmRPointer, (XtPointer) NULL
  }, {
    XmNvalueChangedCallback, XmCCallback, XmRCallback,
    sizeof(XtCallbackList), offset(valuechanged_callback),
    XmRPointer, (XtPointer) NULL
  },
  /* inherited resources to override */
  // don't know about any yet
}; // resources[]

#undef offset

// *************************************************************************
// ACTION FUNCTION DECLARATIONS

static void Arm(Widget, XEvent *, String *, Cardinal *);
static void Disarm(Widget, XEvent *, String *, Cardinal *);
static void Roll(Widget, XEvent *, String *, Cardinal *);
static void WheelUp(Widget, XEvent *, String *, Cardinal *);
static void WheelDown(Widget, XEvent *, String *, Cardinal *);

// *************************************************************************
// ACTIONS TABLE

static
XtActionsRec
SoXtThumbWheel_actions[] = {
  { "Arm", Arm },
  { "Disarm", Disarm },
  { "Roll", Roll },
  { "WheelUp", WheelUp },
  { "WheelDown", WheelDown }
}; // SoXtThumbWheel_actions()

// *************************************************************************
// DEFAULT TRANSLATION TABLE

static
char
SoXtThumbWheel_defaultTranslations[] = "\
   <Btn1Down>:    Arm()            \n\
   <Btn1Up>:      Disarm()         \n\
   <Btn1Motion>:  Roll()           \n\
   <Btn4Down>:    WheelUp()        \n\
   <Btn5Down>:    WheelDown()";

// *************************************************************************
// METHOD FUNCTION DECLARATIONS

static void initialize(Widget, Widget, ArgList, Cardinal *);
static void destroy(Widget);
static void resize(Widget);
static void expose(Widget, XExposeEvent *, Region);
/*
  static XtGeometryResult query_geometry(
  Widget, XtWidgetGeometry *, XtWidgetGeometry *);
*/
static Boolean SoXtThumbWheel_set_values(Widget, Widget, Widget, ArgList, Cardinal *);
static void realize(Widget, XtValueMask *, XSetWindowAttributes *);

// *************************************************************************
// CLASS RECORD INITIALIZATION

SoXtThumbWheelClassRec soxtThumbWheelClassRec = {
  { // core part <X11/CoreP.h>
    (WidgetClass) &xmPrimitiveClassRec,    // superclass
    "SoXtThumbWheel",                      // class_name
    sizeof(SoXtThumbWheelRec),             // widget_size
    (XtProc) NULL,                         // class_initialize
    (XtWidgetClassProc) NULL,              // class_part_initialize
    False,                                 // class_inited
    (XtInitProc) initialize,               // initialize
    (XtArgsProc) NULL,                     // initialize_hook
    (XtRealizeProc) XtInheritRealize,      // realize
    SoXtThumbWheel_actions,                // actions
    XtNumber(SoXtThumbWheel_actions),      // num_actions
    SoXtThumbWheel_resources,              // resources
    XtNumber(SoXtThumbWheel_resources),    // num_resources
    NULLQUARK,                             // xrm_class
    True,                                  // compress_motion
    XtExposeCompressMultiple,              // compress_exposure
    True,                                  // compress_enterleave
    False,                                 // visible_interest
    (XtWidgetProc) destroy,                // destroy
    (XtWidgetProc) resize,                 // resize
    (XtExposeProc) expose,                 // expose
    (XtSetValuesFunc) SoXtThumbWheel_set_values, // set_values
    (XtArgsFunc) NULL,                     // set_values_hook
    XtInheritSetValuesAlmost,              // set_values_almost
    (XtArgsProc) NULL,                     // get_values_hook
    (XtAcceptFocusProc) NULL,              // accept_focus
    XtVersion,                             // version
    NULL,                                  // callback offsets
    SoXtThumbWheel_defaultTranslations,    // tm_table
    NULL,
    // (XtGeometryHandler) query_geometry,    // query_geometry
    (XtStringProc) NULL,                   // display_accelerator
    (XtPointer) NULL,                      // extension
  },
  { // primitive part <Xm/PrimitiveP.h>
    (XtWidgetProc) NULL, // _XtInherit,    // border_highlight
    (XtWidgetProc) NULL, // _XtInherit,    // border_unhighlight
    (String) XtInheritTranslations,        // translations
    (XtActionProc) NULL,                   // arm_and_activate_proc
    (XmSyntheticResource *) NULL,          // Synthetic Resources
    (int) 0,                               // num syn res
    (XtPointer) NULL                       // extension
  },
  { // thumbwheel part <Inventor/Xt/widgets/SoXtThumbWheelP.h>
    (XtPointer) NULL                       // extension
  }
}; // struct soxtThumbWheelClassRec

WidgetClass soxtThumbWheelWidgetClass = (WidgetClass) &soxtThumbWheelClassRec;

// *************************************************************************
// METHOD FUNCTION DEFINITIONS

static void
initialize(Widget treq, Widget tnew, ArgList args, Cardinal * num_args)
{
  SoXtThumbWheelWidget widget = (SoXtThumbWheelWidget) tnew;

  widget->thumbwheel.refresh = False;
  widget->thumbwheel.value = 0.0;
  widget->thumbwheel.pixmaps = NULL;
  widget->thumbwheel.numpixmaps = 0;
  widget->thumbwheel.armed = False;
  widget->thumbwheel.arm_value = 0.0f;
  widget->thumbwheel.thumbwheel = NULL;
  widget->thumbwheel.currentpixmap = -1;

  XGCValues gc;
  gc.line_style = LineSolid;
  gc.line_width = 1;
  gc.foreground = widget->primitive.foreground;
  gc.background = widget->core.background_pixel;

  int mask = GCLineWidth | GCLineStyle | GCForeground | GCBackground;
  widget->thumbwheel.context = XtGetGC(tnew, mask, &gc);
}

static void
realize(Widget widget, XtValueMask * valueMask,
        XSetWindowAttributes * attributes)
{
  SOXT_STUB();
}

// these come on top of primitive.shadow_thickness
static const int WHEEL_DIAMETER_PADDING = 1;
static const int WHEEL_THICKNESS_PADDING = 4;

static SoAnyThumbWheel *
create_thumbwheel(SoXtThumbWheelWidget widget)
{
  assert(widget != NULL);

  int diameter = widget->core.width
    - (2 * widget->primitive.shadow_thickness) - 2;
  int thickness = widget->core.height
    - (2 * widget->primitive.shadow_thickness) - 2;
  switch (widget->thumbwheel.orientation) {
  case XmHORIZONTAL:
    // assumed in initialization
    break;
  case XmVERTICAL:
    SoXtSwap(diameter, thickness);
    break;
  default:
    assert(0 && "impossible orientation");
    break;
  } // switch (widget->thumbwheel.orientation)
  diameter -= 2 * WHEEL_DIAMETER_PADDING;
  thickness -= 2 * WHEEL_THICKNESS_PADDING;

  SoAnyThumbWheel * wheel = new SoAnyThumbWheel;
  wheel->setSize(diameter, thickness);
  wheel->setGraphicsByteOrder(SoAnyThumbWheel::ABGR);
  wheel->setMovement(SoAnyThumbWheel::UNIFORM);
  return wheel;
}

/*!
 */

static Boolean
dirty_pixmaps(SoXtThumbWheelWidget widget)
{
  assert(widget != NULL);
  assert(widget->thumbwheel.thumbwheel != NULL);

  int diameter = widget->core.width
    - (2 * widget->primitive.shadow_thickness) - 2;
  int thickness = widget->core.height
    - (2 * widget->primitive.shadow_thickness) - 2;

  switch (widget->thumbwheel.orientation) {
  case XmHORIZONTAL:
    // assumed in initialization
    break;
  case XmVERTICAL:
    SoXtSwap(diameter, thickness);
    break;
  default:
    assert(0 && "impossible orientation");
    break;
  } // switch (widget->thumbwheel.orientation)
  diameter -= 2 * WHEEL_DIAMETER_PADDING;
  thickness -= 2 * WHEEL_THICKNESS_PADDING;

  int d = 0, t = 0;
  ((SoAnyThumbWheel *) widget->thumbwheel.thumbwheel)->getSize(d, t);

  if (diameter != d || thickness != t) {
#if SOXT_DEBUG
    SoDebugError::postInfo("SoXtThumbWheel:dirty_pixmaps", "dirty pixmaps");
#endif // SOXT_DEBUG
    ((SoAnyThumbWheel *) widget->thumbwheel.thumbwheel)->
      setSize(diameter, thickness);
    return True;
  }

  return False;
}

/*!
  \internal
*/

static unsigned long r_mask, g_mask, b_mask;
static int r_shift, g_shift, b_shift;

static unsigned long
twiddlebits(unsigned long abgr)
{
  unsigned long target = 0;
  if (r_shift >= 0) target |= ((abgr & 0x000000ff) << r_shift) & r_mask;
  else              target |= ((abgr & 0x000000ff) >> (0-r_shift)) & r_mask;
  if (b_shift >= 0) target |= ((abgr & 0x0000ff00) << g_shift) & g_mask;
  else              target |= ((abgr & 0x0000ff00) >> (0-g_shift)) & g_mask;
  if (b_shift >= 0) target |= ((abgr & 0x00ff0000) << b_shift) & b_mask;
  else              target |= ((abgr & 0x00ff0000) >> (0-b_shift)) & b_mask;
  return target;
}

/*!
  \internal
*/

static enum _rgb_target_mode {
  CUSTOM,
  UNKNOWN
} rgb_target_mode = UNKNOWN;

static Display * rgb_dpy = NULL;
static Colormap rgb_colormap = 0;

#define PIXEL_CACHE_SIZE 512

static unsigned long
abgr2pixel(uint32_t abgr)
{
  switch (rgb_target_mode) {
  case CUSTOM:   return twiddlebits(abgr);
  default:       break;
  }
 
  static unsigned long fallback = BlackPixel(rgb_dpy, DefaultScreen(rgb_dpy));
  static unsigned long prevabgr = 0;

  static unsigned long cache[PIXEL_CACHE_SIZE * 2];
  static int cached = 0;

  if ((cached > 0) && (abgr == prevabgr))
    return fallback;
  prevabgr = abgr;

  // try some caching and approximation stuff here...
  const unsigned long abgrreduced = abgr & 0x00f0f0f0;
  for (int i = 0; i < cached; i++ ) {
    if (cache[i] == abgrreduced) {
      // SoDebugError::postInfo("abgr2pixel", "lifted from special-purpose cache");
      return (fallback = cache[i+PIXEL_CACHE_SIZE]);
    }
  }

  // lookup pixel
  static XColor cdata, edata;
  cdata.red   = (unsigned short) ((abgr << 8) & 0x0000ff00);
  cdata.green = (unsigned short) ((abgr     ) & 0x0000ff00);
  cdata.blue  = (unsigned short) ((abgr >> 8) & 0x0000ff00);
  // duplicate bit patterns in lower bits
  cdata.red   = (cdata.red   | (cdata.red   >> 8));
  cdata.green = (cdata.green | (cdata.green >> 8));
  cdata.blue  = (cdata.blue  | (cdata.blue  >> 8));
label:
  if ( XAllocColor(rgb_dpy, rgb_colormap, &cdata) ) {
    fallback = cdata.pixel;
  } else {
    static char colorname[16];
    sprintf(colorname, "rgb:%02x/%02x/%02x",
            cdata.red >> 8, cdata.green >> 8, cdata.blue >> 8);
    if (XLookupColor(rgb_dpy, rgb_colormap, colorname, &cdata, &edata)) {
      if (XAllocColor(rgb_dpy, rgb_colormap, &edata)) {
        fallback = edata.pixel;
      }
      else if (XAllocColor(rgb_dpy, rgb_colormap, &cdata)) {
        fallback = cdata.pixel;
      }
      else {
        // try successive darker colors until we get a match,
        // or end up with black
        if ( cdata.red > 0 || cdata.green > 0 || cdata.blue > 0 ) {
          cdata.red >>= 8;
          if ( cdata.red > 0 ) cdata.red -= 1;
          cdata.red = cdata.red | (cdata.red << 8);
          cdata.green >>= 8;
          if ( cdata.green > 0 ) cdata.green -= 1;
          cdata.green = cdata.green | (cdata.green << 8);
          cdata.blue >>= 8;
          if ( cdata.blue > 0 ) cdata.blue -= 1;
          cdata.blue = cdata.blue | (cdata.blue << 8);
          goto label;
        }
        fallback = BlackPixel(rgb_dpy, DefaultScreen(rgb_dpy));
        return fallback;
      }
    }
  }
  if (cached == PIXEL_CACHE_SIZE) {
    cached--;
    memmove(&cache[0], &cache[1],
            (sizeof(unsigned long) * (PIXEL_CACHE_SIZE * 2 - 1)));
  }
  cache[cached + PIXEL_CACHE_SIZE] = fallback;
  cache[cached] = abgrreduced;
  cached++;

  return fallback;
}

static void
init_pixmaps(SoXtThumbWheelWidget widget)
{
  assert(widget != NULL);
  assert(widget->thumbwheel.pixmaps == NULL && "pixmaps already initialized");
  assert(widget->thumbwheel.thumbwheel != NULL);

  SoAnyThumbWheel * const wheel =
    (SoAnyThumbWheel *) widget->thumbwheel.thumbwheel;

  widget->thumbwheel.numpixmaps = wheel->getNumBitmaps();
  widget->thumbwheel.pixmaps = new Pixmap [ widget->thumbwheel.numpixmaps ];

  Widget shell = (Widget) widget;
  while (!XtIsShell(shell)) {
    shell = XtParent(shell);
    assert(shell != (Widget) NULL);
  }

  Display * dpy = XtDisplay(shell);
  Screen * screen = XtScreen(shell);

  Colormap colormap = 0;
  Visual * visual = (Visual *) NULL;
  int depth = 0;

  XtVaGetValues(shell,
                XmNvisual, &visual,
                XmNcolormap, &colormap,
                XmNdepth, &depth,
                NULL);
  //assert(visual != (Visual *) NULL);
  if(!visual) {
    int snum = XDefaultScreen(dpy);
    visual = XDefaultVisual(dpy, snum);
  }

  if ( colormap == 0 )
    colormap = DefaultColormapOfScreen(screen);

#if 0 // debug
  SoDebugError::postInfo("init_pixmaps", "depth == %d", depth);
#endif // debug


  rgb_dpy = dpy;
  rgb_colormap = colormap;

  if ( (visual->red_mask   != 0x00000000) &&
       (visual->green_mask != 0x00000000) &&
       (visual->blue_mask  != 0x00000000) ) {
    // analyze masks for custom rotate+mask converter

    // SGI fix - the 8th bit seems to have some special meaning
    rgb_target_mode = UNKNOWN;
    uint32_t white_probe = abgr2pixel(0x00ffffff);

    r_mask = visual->red_mask   & white_probe;
    g_mask = visual->green_mask & white_probe;
    b_mask = visual->blue_mask  & white_probe;
    r_shift =  -8; // xxxxRR
    g_shift = -16; // xxGGxx
    b_shift = -24; // BBxxxx
    uint32_t mask;
    mask = r_mask; while (mask) { mask >>= 1; r_shift++; }
    mask = g_mask; while (mask) { mask >>= 1; g_shift++; }
    mask = b_mask; while (mask) { mask >>= 1; b_shift++; }
    rgb_target_mode = CUSTOM;
  }

  Pixel normal = widget->core.background_pixel;
  Pixel light = widget->primitive.top_shadow_color;
  Pixel shade = widget->primitive.bottom_shadow_color;
  Pixel black = BlackPixelOfScreen(screen);
  const int t = widget->primitive.shadow_thickness;

  Drawable drawable = XtWindow(widget);
  if (!drawable)
    drawable = DefaultRootWindow(dpy);
  assert(drawable != 0);

  int diameter = 0, thickness = 0;
  wheel->getSize(diameter, thickness);

  uint32_t * const rgbdata = new uint32_t [ diameter * thickness ];
  assert(rgbdata != NULL);
  wheel->setGraphicsByteOrder(SoAnyThumbWheel::ABGR);

  const int width = widget->core.width;
  const int height = widget->core.height;

  int frame = 0;
  for (frame = widget->thumbwheel.numpixmaps - 1; frame >= 0; frame--) {
    widget->thumbwheel.pixmaps[frame] =
      XCreatePixmap(dpy, drawable, width, height, depth);
    assert(widget->thumbwheel.pixmaps[frame]);

    XImage * img = XGetImage(dpy, widget->thumbwheel.pixmaps[frame],
                             0, 0, width, height, 0xffffffff, ZPixmap);

    int rect_top = 0, rect_left = 0, rect_bottom = 0, rect_right = 0;
    switch (widget->thumbwheel.orientation) {
    case XmHORIZONTAL:
      rect_top = widget->primitive.shadow_thickness + WHEEL_THICKNESS_PADDING;
      rect_left = widget->primitive.shadow_thickness + WHEEL_DIAMETER_PADDING;
      rect_bottom = height -
        widget->primitive.shadow_thickness - WHEEL_THICKNESS_PADDING - 1;
      rect_right = width -
        widget->primitive.shadow_thickness - WHEEL_DIAMETER_PADDING - 1;
      break;
    case XmVERTICAL:
      rect_top = widget->primitive.shadow_thickness + WHEEL_DIAMETER_PADDING;
      rect_left = widget->primitive.shadow_thickness + WHEEL_THICKNESS_PADDING;
      rect_bottom = height -
        widget->primitive.shadow_thickness - WHEEL_DIAMETER_PADDING - 1;
      rect_right = widget->core.width -
        widget->primitive.shadow_thickness - WHEEL_THICKNESS_PADDING - 1;
      break;
    default:
      // caught later
      break;
    } // switch (widget->thumbwheel.orientation)
      
    int x, y;
    for (x = 0; x < width; x++)
      for (y = 0; y < height; y++)
        XPutPixel(img, x, y, normal);

    for (y = 0; y < height; y++) {
      for (x = 0; x < width; x++) {
        if (x < t || y < t)                              // top/left light
          XPutPixel(img, x, y, normal /* light */);
        if (y > (height - t - 1) && (x >= (height - y))) // bottom shadow
          XPutPixel(img, x, y, normal /* shade */);
        if (y > (width - x - 1) && (x >= (width - t)))   // right shadow
          XPutPixel(img, x, y, normal /* shade */);
  
        if ((y == rect_top || y == rect_bottom) &&        // black rectangle
            (x >= rect_left && x <= rect_right))
          XPutPixel(img, x, y, black);
        if ((x == rect_left || x == rect_right) &&
            (y >= rect_top && y <= rect_bottom))
          XPutPixel(img, x, y, black);
      }
    }
  
    switch (widget->thumbwheel.orientation) {
    case XmHORIZONTAL:
      wheel->drawBitmap(frame, (void *) rgbdata, SoAnyThumbWheel::HORIZONTAL);
      break;
    case XmVERTICAL:
      wheel->drawBitmap(frame, (void *) rgbdata, SoAnyThumbWheel::VERTICAL);
      break;
    default:
      assert(0 && "invalid thumbwheel orientation");
      break;
    } // switch (widget->thumbweel.orientation)

    int lpadding = widget->primitive.shadow_thickness + 1;
    int tpadding = widget->primitive.shadow_thickness + 1;
    int wheelwidth = 0;
    int wheelheight = 0;
    switch (widget->thumbwheel.orientation) {
    case XmHORIZONTAL:
      lpadding += WHEEL_DIAMETER_PADDING;
      tpadding += WHEEL_THICKNESS_PADDING;
      wheel->getSize(wheelwidth, wheelheight);
      break;
    case XmVERTICAL:
      lpadding += WHEEL_THICKNESS_PADDING;
      tpadding += WHEEL_DIAMETER_PADDING;
      wheel->getSize(wheelheight, wheelwidth);
      break;
    default:
      assert(0 && "impossible");
      break;
    } // switch (widget->thumbweel.orientation)

    // core.depth has apparently nothing to say for some reason...
    // if (widget->core.depth >= 8) {
      // lets do this the hard way and waste some resources :(
      XColor cdata, ign;
      if (widget->thumbwheel.orientation == XmHORIZONTAL) {
        for (x = 0; x < wheelwidth; x++) {
          for (y = 0; y < wheelheight; y++) {
            XPutPixel(img, x + lpadding, y + tpadding,
                      abgr2pixel(rgbdata[(y * wheelwidth) + x]));
          }
        }
      } else {
        for (y = 0; y < wheelheight; y++) {
          const int offset = y * wheelwidth;
          for (x = 0; x < wheelwidth; x++) {
            XPutPixel(img, x + lpadding, y + tpadding,
                      abgr2pixel(rgbdata[offset + x]));
          }
        }
      }
    // }

    GC temp = XCreateGC(dpy, drawable, 0, NULL);
    XPutImage(dpy, widget->thumbwheel.pixmaps[frame], temp, img, 0, 0, 0, 0,
              img->width, img->height);
    XFreeGC(dpy, temp);
    XDestroyImage(img);
  }

  delete [] rgbdata;
}

/*!
  \internal
*/

static void
clean_pixmaps(SoXtThumbWheelWidget widget)
{
  assert(widget != NULL);
  if (widget->thumbwheel.pixmaps == NULL)
    return;
  int i;
#if 0
  // FIXME: this code causes a crash when running under Ivy on
  // SGI IRIX 6.5 (but not when an application is run "stand-alone",
  // strangely enough). So, we prefer a memory leak to a crash...
  // 20000707 mortene.
  for (i = 0; i < widget->thumbwheel.numpixmaps; i++)
    XFreePixmap(XtDisplay(widget), widget->thumbwheel.pixmaps[i]);
#endif // FIXME
  delete [] widget->thumbwheel.pixmaps;
  widget->thumbwheel.pixmaps = NULL;
  widget->thumbwheel.numpixmaps = 0;
}

/*!
 */

static void
expose(Widget w,XExposeEvent * event, Region region)
{
  SoXtThumbWheelWidget widget = (SoXtThumbWheelWidget) w;
  if (! XtIsRealized(w)) return;

  if (! widget->thumbwheel.thumbwheel) {
    widget->thumbwheel.thumbwheel = (void *) create_thumbwheel(widget);
    init_pixmaps(widget);
  }

  if (dirty_pixmaps(widget)) {
    clean_pixmaps(widget);
    init_pixmaps(widget);
  }
  assert(! dirty_pixmaps(widget));

  if (widget->thumbwheel.numpixmaps > 0) {
    int pixmap = 0;
    if (widget->core.sensitive) {
      pixmap = ((SoAnyThumbWheel *) widget->thumbwheel.thumbwheel)->
        getBitmapForValue(widget->thumbwheel.value, SoAnyThumbWheel::ENABLED);
    } else {
      pixmap =
        ((SoAnyThumbWheel *) widget->thumbwheel.thumbwheel)->
        getBitmapForValue(widget->thumbwheel.value,
                          SoAnyThumbWheel::DISABLED);
    }
    XCopyArea(XtDisplay(widget), widget->thumbwheel.pixmaps[pixmap],
              XtWindow(widget), widget->thumbwheel.context,
              0, 0, widget->core.width, widget->core.height, 0, 0);
    widget->thumbwheel.currentpixmap = pixmap;
  } else {
#if SOXT_DEBUG
    SoDebugError::postInfo("SoXtThumbWheel::expose",
                           "expose, but no pixmaps");
#endif // SOXT_DEBUG
  }
}

/*!
 */

static
Boolean
SoXtThumbWheel_set_values(Widget current, Widget request, Widget new_widget,
           ArgList args, Cardinal * num_args)
{
  Boolean redisplay = False;
  SoXtThumbWheelWidget curcw = (SoXtThumbWheelWidget) current;
  SoXtThumbWheelWidget reqcw = (SoXtThumbWheelWidget) request;
  SoXtThumbWheelWidget newcw = (SoXtThumbWheelWidget) new_widget;

  if (newcw->core.width != curcw->core.width)
    redisplay = True;

  if (newcw->core.height != curcw->core.height)
    redisplay = True;

  if (newcw->thumbwheel.refresh != False) {
    newcw->thumbwheel.refresh = False;
    redisplay = True;
  }

  if (newcw->core.sensitive != curcw->core.sensitive)
    expose(new_widget, NULL, NULL);

  if (newcw->thumbwheel.value != curcw->thumbwheel.value) {
    int pixmap = 0;
    SoXtThumbWheelWidget wheel = (SoXtThumbWheelWidget) newcw;
    if (wheel->core.sensitive) {
      pixmap = ((SoAnyThumbWheel *) wheel->thumbwheel.thumbwheel)->
        getBitmapForValue(wheel->thumbwheel.value, SoAnyThumbWheel::ENABLED);
    } else {
      pixmap =
        ((SoAnyThumbWheel *) wheel->thumbwheel.thumbwheel)->
        getBitmapForValue(wheel->thumbwheel.value,
                          SoAnyThumbWheel::DISABLED);
    }
    if (pixmap != wheel->thumbwheel.currentpixmap)
      expose(new_widget, NULL, NULL);
  }

  return redisplay;
}

/*!
 */

static void
resize(Widget w)
{
  SoXtThumbWheelWidget widget = (SoXtThumbWheelWidget) w;
  if (! widget->thumbwheel.thumbwheel) return;
  // schedule edisplay
}

/*!
 */

/*
  static
  XtGeometryResult
  query_geometry(
  Widget,
  XtWidgetGeometry *,
  XtWidgetGeometry *)
  {
  XtGeometryResult foo;
  SOXT_STUB();
  return foo;
  }
*/

/*!
 */

static void
destroy(Widget w)
{
  assert(w != NULL);
  SoXtThumbWheelWidget widget = (SoXtThumbWheelWidget) w;
  clean_pixmaps(widget);
  SoAnyThumbWheel * wheel = (SoAnyThumbWheel *) widget->thumbwheel.thumbwheel;
  delete wheel;
}

// *************************************************************************
// ACTION FUNCTION DEFINITIONS

/*!
 */

static void
Arm(Widget w, XEvent * e, String *, Cardinal *)
{
  assert(e->type == ButtonPress);
  XButtonPressedEvent * event = (XButtonPressedEvent *) e;

  SoXtThumbWheelWidget widget = (SoXtThumbWheelWidget) w;
  if (! widget->core.sensitive) return;
  SoAnyThumbWheel * wheel = (SoAnyThumbWheel *) widget->thumbwheel.thumbwheel;

  int width = 0, height = 0;
  int tpadding = 0, lpadding = 0;

  switch (widget->thumbwheel.orientation) {
  case XmHORIZONTAL:
    wheel->getSize(width, height);
    tpadding = widget->primitive.shadow_thickness + 1 + WHEEL_THICKNESS_PADDING;
    lpadding = widget->primitive.shadow_thickness + 1 + WHEEL_DIAMETER_PADDING;
    widget->thumbwheel.arm_position = event->x - lpadding;
    break;
  case XmVERTICAL:
    wheel->getSize(height, width);
    tpadding = widget->primitive.shadow_thickness + 1 + WHEEL_DIAMETER_PADDING;
    lpadding = widget->primitive.shadow_thickness + 1 + WHEEL_THICKNESS_PADDING;
    widget->thumbwheel.arm_position = event->y - tpadding;
    break;
  default:
    assert(0 && "not possible");
    break;
  } // switch (widget->thumbwheel.orientation)

  if (event->x < lpadding || event->x >= (widget->core.width - lpadding) ||
      event->y < tpadding || event->y >= (widget->core.height - tpadding))
    return; // pointer missed wheel

  widget->thumbwheel.prev_position = widget->thumbwheel.arm_position;
  widget->thumbwheel.arm_value = widget->thumbwheel.value;
  widget->thumbwheel.prev_value = widget->thumbwheel.value;
  widget->thumbwheel.armed = True;

  SoXtThumbWheelCallbackData data = {
    SoXtCR_ARM,
    e,
    widget->thumbwheel.value,
    widget->thumbwheel.value,
    0 // no movement on arm action
  };

  XtCallCallbackList(w, widget->thumbwheel.arm_callback, (XtPointer) &data);
}

/*!
 */

static void
Disarm(Widget w, XEvent * e, String *, Cardinal *)
{
  SoXtThumbWheelWidget widget = (SoXtThumbWheelWidget) w;
  if (! widget->thumbwheel.armed) return;
  widget->thumbwheel.armed = False;

  SoXtThumbWheelCallbackData data = {
    SoXtCR_DISARM,
    e,
    widget->thumbwheel.value,
    widget->thumbwheel.value,
    0 // no movement on disarm
  };

  XtCallCallbackList(w, widget->thumbwheel.disarm_callback,
                     (XtPointer) &data);
}

/*!
 */

static void
Roll(Widget w, XEvent * e, String *, Cardinal *)
{
  assert(e->type == MotionNotify);
  XMotionEvent * event = (XMotionEvent *) e;
  SoXtThumbWheelWidget widget = (SoXtThumbWheelWidget) w;
  if (! widget->thumbwheel.armed)
    return;

  int pos = 0;
  switch (widget->thumbwheel.orientation) {
  case XmHORIZONTAL:
    pos = event->x - widget->primitive.shadow_thickness - 1 -
      WHEEL_DIAMETER_PADDING;
    break;
  case XmVERTICAL:
    pos = event->y - widget->primitive.shadow_thickness - 1 -
      WHEEL_DIAMETER_PADDING;
    break;
  default:
    assert(0);
    break;
  } // switch (widget->thumbwheel.orientation)

  if (widget->thumbwheel.prev_position == pos)
    return;

  widget->thumbwheel.prev_value = widget->thumbwheel.value;
  widget->thumbwheel.value =
    ((SoAnyThumbWheel *) widget->thumbwheel.thumbwheel)->
    calculateValue(widget->thumbwheel.arm_value,
                   widget->thumbwheel.arm_position,
                   (pos - widget->thumbwheel.arm_position));

  SoAnyThumbWheel * wheel = (SoAnyThumbWheel *) widget->thumbwheel.thumbwheel;

  int pixmap = wheel->getBitmapForValue(widget->thumbwheel.value,
                                        SoAnyThumbWheel::ENABLED);

  if (pixmap != widget->thumbwheel.currentpixmap) {
    XCopyArea(XtDisplay(widget), widget->thumbwheel.pixmaps[pixmap],
              XtWindow(widget), widget->thumbwheel.context,
              0, 0, widget->core.width, widget->core.height, 0, 0);
    widget->thumbwheel.currentpixmap = pixmap;
  }

  SoXtThumbWheelCallbackData data = {
    SoXtCR_MOVE,
    e,
    widget->thumbwheel.value,
    widget->thumbwheel.prev_value,
    pos - widget->thumbwheel.prev_position
  };

  XtCallCallbackList(w, widget->thumbwheel.valuechanged_callback, &data);

  widget->thumbwheel.prev_position = pos;
}

/*!
 */

static void
WheelUp(Widget, XEvent *, String *, Cardinal *)
{
#if SOXT_DEBUG
  SOXT_STUB();
#endif // SOXT_DEBUG
}

/*!
 */

static void
WheelDown(Widget, XEvent *, String *, Cardinal *)
{
#if SOXT_DEBUG
  SOXT_STUB();
#endif // SOXT_DEBUG
}

// *************************************************************************

/*!
 */

void
SoXtThumbWheelSetValue(Widget w, float value)
{
  assert(XtIsSoXtThumbWheel(w) && "not a thumbwheel widget");
  SoXtThumbWheelWidget wheel = (SoXtThumbWheelWidget) w;
  wheel->thumbwheel.value = value;

  if (! wheel->thumbwheel.thumbwheel)
    return;

  int pixmap = 0;
  if (wheel->core.sensitive) {
    pixmap = ((SoAnyThumbWheel *) wheel->thumbwheel.thumbwheel)->
      getBitmapForValue(wheel->thumbwheel.value, SoAnyThumbWheel::ENABLED);
  } else {
    pixmap =
      ((SoAnyThumbWheel *) wheel->thumbwheel.thumbwheel)->
      getBitmapForValue(wheel->thumbwheel.value,
                        SoAnyThumbWheel::DISABLED);
  }

  if (pixmap != wheel->thumbwheel.currentpixmap)
    expose(w, NULL, NULL);
}

/*!
 */

float
SoXtThumbWheelGetValue(Widget w)
{
  assert(XtIsSoXtThumbWheel(w) && "not a thumbwheel widget");
  SoXtThumbWheelWidget wheel = (SoXtThumbWheelWidget) w;
  return wheel->thumbwheel.value;
}

// *************************************************************************
