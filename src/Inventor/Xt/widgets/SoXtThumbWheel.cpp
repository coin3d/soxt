/**************************************************************************
 *
 *  Copyright (C) 2000 by Systems in Motion.  All rights reserved.
 *
 *  This file is part of the Coin library.
 *
 *  This file may be distributed under the terms of the Q Public License
 *  as defined by Troll Tech AS of Norway and appearing in the file
 *  LICENSE.QPL included in the packaging of this file.
 *
 *  If you want to use Coin in applications not covered by licenses
 *  compatible with the QPL, you can contact SIM to aquire a
 *  Professional Edition license for Coin.
 *
 *  Systems in Motion AS, Prof. Brochs gate 6, N-7030 Trondheim, NORWAY
 *  http://www.sim.no/ sales@sim.no Voice: +47 22114160 Fax: +47 67172912
 *
 **************************************************************************/

static const char rcsid[] =
  "$Id$";

#include <assert.h>

#include <Xm/XmP.h>

#include <Inventor/errors/SoDebugError.h>

#include <Inventor/Xt/SoXtBasic.h>
#include <Inventor/Xt/widgets/SoAnyThumbWheel.h>
#include <Inventor/Xt/widgets/SoXtThumbWheelP.h>

/*
  TODO:
    - interactivity and callbacks
    - implement use of a virtual Colormap instead of creating graphics
      the way it is now.
*/

// *************************************************************************
// RESOURCES

#define offset( field ) \
  XtOffsetOf( SoXtThumbWheelRec, thumbwheel.field )

static
XtResource
resources[] = {
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
  {
    XmNvalue, XmCValue, XmRFloat,
    sizeof(float), offset(value),
    XmRPointer, (XtPointer) NULL
  },
  // insensitiveForeground
  // shadowThickness
  // highlightThickness
  {
    XmNarmCallback, XmCCallback, XmRCallback,
    sizeof(XtCallbackList), offset(arm_callback),
    XmRPointer, (XtPointer) NULL
  },
  {
    XmNdisarmCallback, XmCCallback, XmRCallback,
    sizeof(XtCallbackList), offset(disarm_callback),
    XmRPointer, (XtPointer) NULL
  },
  {
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

static void Arm( Widget, XEvent *, String *, Cardinal * );
static void Disarm( Widget, XEvent *, String *, Cardinal * );
static void Roll( Widget, XEvent *, String *, Cardinal * );
static void WheelUp( Widget, XEvent *, String *, Cardinal * );
static void WheelDown( Widget, XEvent *, String *, Cardinal * );

// *************************************************************************
// ACTIONS TABLE

static XtActionsRec actions[] = {
  { "Arm", Arm },
  { "Disarm", Disarm },
  { "Roll", Roll },
  { "WheelUp", WheelUp },
  { "WheelDown", WheelDown }
}; // actions()

// *************************************************************************
// DEFAULT TRANSLATION TABLE

static char defaultTranslations[] = "\
   <Btn1Down>:    Arm()            \n\
   <Btn1Up>:      Disarm()         \n\
   <Btn1Motion>:  Roll()           \n\
   <Btn4Down>:    WheelUp()        \n\
   <Btn5Down>:    WheelDown()";

// *************************************************************************
// METHOD FUNCTION DECLARATIONS

static void initialize( Widget, Widget, ArgList, Cardinal * );
static void destroy( Widget );
static void resize( Widget );
static void expose( Widget, XExposeEvent *, Region );
static XtGeometryResult query_geometry(
                          Widget, XtWidgetGeometry *, XtWidgetGeometry * );
static Boolean set_values( Widget, Widget, Widget, ArgList, Cardinal * );

static void realize( Widget, XtValueMask *, XSetWindowAttributes * );

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
    actions,                               // actions
    XtNumber(actions),                     // num_actions
    resources,                             // resources
    XtNumber(resources),                   // num_resources
    NULLQUARK,                             // xrm_class
    True,                                  // compress_motion
    XtExposeCompressMultiple,              // compress_exposure
    True,                                  // compress_enterleave
    False,                                 // visible_interest
    (XtWidgetProc) destroy,                // destroy
    (XtWidgetProc) resize,                 // resize
    (XtExposeProc) expose,                 // expose
    (XtSetValuesFunc) set_values,          // set_values
    (XtArgsFunc) NULL,                     // set_values_hook
    XtInheritSetValuesAlmost,              // set_values_almost
    (XtArgsProc) NULL,                     // get_values_hook
    (XtAcceptFocusProc) NULL,              // accept_focus
    XtVersion,                             // version
    NULL,                                  // callback offsets
    defaultTranslations,                   // tm_table
    (XtGeometryHandler) query_geometry,    // query_geometry
    (XtStringProc) NULL,                   // display_accelerator
    (XtPointer) NULL,                      // extension
  },
  { // primitive part <Xm/PrimitiveP.h>
    (XtWidgetProc) NULL, // _XtInherit,             // border_highlight
    (XtWidgetProc) NULL, // _XtInherit,             // border_unhighlight
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

static
void
initialize(
  Widget treq,
  Widget tnew,
  ArgList args,
  Cardinal * num_args )
{
  SoXtThumbWheelWidget widget = (SoXtThumbWheelWidget) tnew;

  widget->thumbwheel.pixmaps = NULL;
  widget->thumbwheel.numpixmaps = 0;
  widget->thumbwheel.armed = False;
  widget->thumbwheel.arm_value = 0.0f;
  widget->thumbwheel.thumbwheel = NULL;

  XGCValues gc;
  gc.line_style = LineSolid;
  gc.line_width = 1;
  gc.foreground = widget->primitive.foreground;
  gc.background = widget->core.background_pixel;

  int mask = GCLineWidth | GCLineStyle | GCForeground | GCBackground;
  widget->thumbwheel.context = XtGetGC( tnew, mask, &gc );
} // initialize()

static
void
realize(
  Widget widget,
  XtValueMask * valueMask,
  XSetWindowAttributes * attributes )
{
  SOXT_STUB();
} // realize()

// these come on top of primitive.shadow_thickness
static const int WHEEL_DIAMETER_PADDING = 6;
static const int WHEEL_THICKNESS_PADDING = 3;

static
SoAnyThumbWheel *
create_thumbwheel(
  SoXtThumbWheelWidget widget )
{
  assert( widget != NULL );

  int diameter = widget->core.width
               - (2 * widget->primitive.shadow_thickness) - 2;
  int thickness = widget->core.height
                - (2 * widget->primitive.shadow_thickness) - 2;
  switch ( widget->thumbwheel.orientation ) {
  case XmHORIZONTAL:
    // assumed in initialization
    break;
  case XmVERTICAL:
    SoXtSwap( diameter, thickness );
    break;
  default:
    assert( 0 && "impossible orientation" );
    break;
  } // switch ( widget->thumbwheel.orientation )
  diameter -= 2 * WHEEL_DIAMETER_PADDING;
  thickness -= 2 * WHEEL_THICKNESS_PADDING;

  SoAnyThumbWheel * wheel = new SoAnyThumbWheel;
  wheel->SetWheelSize( diameter, thickness );
  wheel->SetGraphicsByteOrder( SoAnyThumbWheel::ABGR );
  wheel->SetWheelMotionMethod( SoAnyThumbWheel::UNIFORM );
  wheel->SetWheelRangeBoundaryHandling( SoAnyThumbWheel::ACCUMULATE );
  return wheel;
} // create_thumbwheel()

static
Boolean
dirty_pixmaps(
  SoXtThumbWheelWidget widget )
{
  assert( widget != NULL );
  assert( widget->thumbwheel.thumbwheel != NULL );

  int diameter = widget->core.width
               - (2 * widget->primitive.shadow_thickness) - 2;
  int thickness = widget->core.height
                - (2 * widget->primitive.shadow_thickness) - 2;

  switch ( widget->thumbwheel.orientation ) {
  case XmHORIZONTAL:
    // assumed in initialization
    break;
  case XmVERTICAL:
    SoXtSwap( diameter, thickness );
    break;
  default:
    assert( 0 && "impossible orientation" );
    break;
  } // switch ( widget->thumbwheel.orientation )
  diameter -= 2 * WHEEL_DIAMETER_PADDING;
  thickness -= 2 * WHEEL_THICKNESS_PADDING;

  int d = 0, t = 0;
  ((SoAnyThumbWheel *) widget->thumbwheel.thumbwheel)->GetWheelSize( d, t );

  if ( diameter != d || thickness != t ) {
#if SOXT_DEBUG
    SoDebugError::postInfo( "SoXtThumbWheel:dirty_pixmaps", "dirty pixmaps" );
#endif // SOXT_DEBUG
    ((SoAnyThumbWheel *) widget->thumbwheel.thumbwheel)->
      SetWheelSize( diameter, thickness );
    return True;
  }

  return False;
} // dirty_pixmaps()

/*!
  \internal
*/

static
inline
unsigned long
from32to16bit(
  unsigned long the32bitcolor )
{
  unsigned long the16bitcolor = 0;
  the16bitcolor |= (the32bitcolor >>  3) & 0x001f;
  the16bitcolor |= (the32bitcolor >>  5) & 0x07e0;
  the16bitcolor |= (the32bitcolor >>  8) & 0xf800;
  return the16bitcolor;
} // to16bit()

/*!
  \internal
*/

static
void
init_pixmaps(
  SoXtThumbWheelWidget widget )
{
  assert( widget != NULL );
  if ( widget->thumbwheel.pixmaps != NULL ) {
#if SOXT_DEBUG
    SoDebugError::postInfo( "SoXtThumbWheel:init_pixmaps",
      "pixmaps already initialized" );
#endif // SOXT_DEBUG
    return;
  }
  assert( widget->thumbwheel.thumbwheel != NULL );

  SoAnyThumbWheel * wheel = (SoAnyThumbWheel *) widget->thumbwheel.thumbwheel;

  widget->thumbwheel.numpixmaps = wheel->BitmapsRequired();
  widget->thumbwheel.pixmaps = new Pixmap [ widget->thumbwheel.numpixmaps ];

  const int width = widget->core.width;
  const int height = widget->core.height;
  const int pixels = width * height;
  int diameter = 0, thickness = 0;
  wheel->GetWheelSize( diameter, thickness );

  Pixel image[ pixels ];

  Pixel normal = widget->core.background_pixel;
  Pixel light = widget->primitive.top_shadow_color;
  Pixel shade = widget->primitive.bottom_shadow_color;
  const int t = widget->primitive.shadow_thickness;
  int x, y;
  for ( x = 0; x < pixels; x++ )
    image[x] = normal;

  int rect_top = 0, rect_left = 0, rect_bottom = 0, rect_right = 0;
  switch ( widget->thumbwheel.orientation ) {
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
  } // switch ( widget->thumbwheel.orientation )

  for ( y = 0; y < height; y++ ) {
    for ( x = 0; x < width; x++ ) {
      // top/left light
      if ( x < t || y < t )
        image[(y * width) + x] = light;
      // bottom shadow
      if ( y > (height - t - 1) && (x >= (height - y)) )
        image[(y * width) + x] = shade;
      // right shadow
      if ( y > (width - x - 1) && (x >= (width - t)) )
        image[(y * width) + x] = shade;

      // rectangle
      if ( (y == rect_top || y == rect_bottom) &&
           (x >= rect_left && x <= rect_right) )
        image[(y * width) + x] = 0x00000000;
      if ( (x == rect_left || x == rect_right) &&
           (y >= rect_top && y <= rect_bottom) )
        image[(y * width) + x] = 0x00000000;
    }
  }

  int j;
  for ( j = 0; j < widget->thumbwheel.numpixmaps; j++ ) {
    unsigned long bitmap[ diameter * thickness ];
    switch ( widget->thumbwheel.orientation ) {
    case XmHORIZONTAL:
      wheel->DrawBitmap( j, (void *) bitmap, SoAnyThumbWheel::HORIZONTAL );
      break;
    case XmVERTICAL:
      wheel->DrawBitmap( j, (void *) bitmap, SoAnyThumbWheel::VERTICAL );
      break;
    default:
      assert( 0 && "no orientation" );
      break;
    } // switch ( widget->thumbweel.orientation )

    Display * dpy = XtDisplay(widget);
    Screen * screen = XtScreen(widget);
    Visual * visual = DefaultVisual(dpy,DefaultScreen(dpy));
    int depth = DefaultDepthOfScreen(screen);

    int lpadding = widget->primitive.shadow_thickness + 1;
    int tpadding = widget->primitive.shadow_thickness + 1;
    int wheelwidth = 0;
    int wheelheight = 0;
    switch ( widget->thumbwheel.orientation ) {
    case XmHORIZONTAL:
      lpadding += WHEEL_DIAMETER_PADDING;
      tpadding += WHEEL_THICKNESS_PADDING;
      wheel->GetWheelSize( wheelwidth, wheelheight );
      break;
    case XmVERTICAL:
      lpadding += WHEEL_THICKNESS_PADDING;
      tpadding += WHEEL_DIAMETER_PADDING;
      wheel->GetWheelSize( wheelheight, wheelwidth );
      break;
    default:
      assert( 0 && "impossible" );
      break;
    } // switch ( widget->thumbweel.orientation )

    for ( y = 0; y < wheelheight; y++ ) {
      for ( x = 0; x < wheelwidth; x++ ) {
        switch ( depth ) {
        case 16:
          image[((y+tpadding)*width)+lpadding+x] =
            from32to16bit( bitmap[(y*wheelwidth)+x] );
          break;
        case 24:
        case 32:
          image[((y+tpadding)*width)+lpadding+x] =
            bitmap[(y*wheelwidth)+x];
          break;
        default:
#if SOXT_DEBUG
          SoDebugError::postWarning( "SoXtThumbWheel:init_pixmaps",
            "unsupported pixmap depth (%d)", depth );
#endif // SOXT_DEBUG
          break;
        }
      }
    }

    char * data = new char [width * height * (depth / 8) * 2];

    XImage * img = XCreateImage( dpy, visual, depth, ZPixmap, 0,
      data, width, height, 8, 0 );
    assert( img != NULL );

    // put pixels
    for ( y = 0; y < height; y++ ) {
      for ( x = 0; x < width; x++ ) {
        unsigned long pixel = image[(y*width)+x];
        XPutPixel( img, x, y, pixel );
      }
    }
    widget->thumbwheel.pixmaps[j] =
      XCreatePixmap( XtDisplay(widget), XtWindow(widget),
        width, height, img->depth );
    XPutImage( XtDisplay(widget), widget->thumbwheel.pixmaps[j],
      widget->thumbwheel.context, img, 0, 0, 0, 0, width, height );
    delete [] data;
    img->data = NULL;
    XDestroyImage( img );
  }
} // init_pixmaps()

/*!
  \internal
*/

static
void
clean_pixmaps(
  SoXtThumbWheelWidget widget )
{
  assert( widget != NULL );
  if ( widget->thumbwheel.pixmaps == NULL )
    return;
  int i;
  for ( i = 0; i < widget->thumbwheel.numpixmaps; i++ )
    XFreePixmap( XtDisplay(widget), widget->thumbwheel.pixmaps[i] );
  delete [] widget->thumbwheel.pixmaps;
  widget->thumbwheel.pixmaps = NULL;
  widget->thumbwheel.numpixmaps = 0;
} // clean_pixmaps()

/*!
*/

static
void
expose(
  Widget w,
  XExposeEvent * event,
  Region region )
{
  SoXtThumbWheelWidget widget = (SoXtThumbWheelWidget) w;
  if ( ! XtIsRealized( widget ) ) return;

  if ( ! widget->thumbwheel.thumbwheel ) {
    widget->thumbwheel.thumbwheel = (void *) create_thumbwheel( widget );
    init_pixmaps( widget );
  }

  if ( dirty_pixmaps( widget ) ) {
    clean_pixmaps( widget );
    init_pixmaps( widget );
  }
  assert( ! dirty_pixmaps( widget ) );

  if ( widget->thumbwheel.numpixmaps > 0 ) {
    int pixmap = 0;
    if ( widget->core.sensitive ) {
      pixmap = ((SoAnyThumbWheel *) widget->thumbwheel.thumbwheel)->
        GetBitmapForValue( widget->thumbwheel.value, SoAnyThumbWheel::ENABLED );
    } else {
      pixmap =
        ((SoAnyThumbWheel *) widget->thumbwheel.thumbwheel)->
          GetBitmapForValue( widget->thumbwheel.value,
                             SoAnyThumbWheel::DISABLED );
    }
    XCopyArea( XtDisplay(widget), widget->thumbwheel.pixmaps[pixmap],
      XtWindow(widget), widget->thumbwheel.context,
      0, 0, widget->core.width, widget->core.height, 0, 0 );
  } else {
#if SOXT_DEBUG
    SoDebugError::postInfo( "SoXtThumbWheel::expose",
      "expose, but no pixmaps" );
#endif // SOXT_DEBUG
  }
} // expose()

/*!
*/

static
Boolean
set_values(
  Widget current,
  Widget request,
  Widget new_widget,
  ArgList args,
  Cardinal * num_args )
{
  Boolean redisplay = False;
  SOXT_STUB();
  return redisplay;
} // set_values()

/*!
*/

static
void
resize(
  Widget w )
{
  SoXtThumbWheelWidget widget = (SoXtThumbWheelWidget) w;
  if ( ! widget->thumbwheel.thumbwheel ) return;
  // schedule edisplay
} // resize()

/*!
*/

static
XtGeometryResult
query_geometry(
  Widget,
  XtWidgetGeometry *,
  XtWidgetGeometry * )
{
  SOXT_STUB();
} // query_geometry()

/*!
*/

static
void
destroy(
  Widget w )
{
  assert( w != NULL );
  SoXtThumbWheelWidget widget = (SoXtThumbWheelWidget) w;
  clean_pixmaps( widget );
  SoAnyThumbWheel * wheel = (SoAnyThumbWheel *) widget->thumbwheel.thumbwheel;
  delete wheel;
} // destroy()

// *************************************************************************
// ACTION FUNCTION DEFINITIONS

/*!
*/

void
Arm(
  Widget w,
  XEvent * e,
  String *,
  Cardinal * )
{
  assert( e->type == ButtonPress );
  XButtonPressedEvent * event = (XButtonPressedEvent *) e;

  SoXtThumbWheelWidget widget = (SoXtThumbWheelWidget) w;
  SoAnyThumbWheel * wheel = (SoAnyThumbWheel *) widget->thumbwheel.thumbwheel;

  int width = 0, height = 0;
  int tpadding = 0, lpadding = 0;

  switch ( widget->thumbwheel.orientation ) {
  case XmHORIZONTAL:
    wheel->GetWheelSize( width, height );
    tpadding = widget->primitive.shadow_thickness + 1 + WHEEL_THICKNESS_PADDING;
    lpadding = widget->primitive.shadow_thickness + 1 + WHEEL_DIAMETER_PADDING;
    widget->thumbwheel.arm_position = event->x - lpadding;
    break;
  case XmVERTICAL:
    wheel->GetWheelSize( height, width );
    tpadding = widget->primitive.shadow_thickness + 1 + WHEEL_DIAMETER_PADDING;
    lpadding = widget->primitive.shadow_thickness + 1 + WHEEL_THICKNESS_PADDING;
    widget->thumbwheel.arm_position = event->y - tpadding;
    break;
  default:
    assert( 0 && "not possible" );
    break;
  } // switch ( widget->thumbwheel.orientation )

  if ( event->x < lpadding || event->x >= (widget->core.width - lpadding) ||
       event->y < tpadding || event->y >= (widget->core.height - tpadding) )
    return; // pointer missed wheel

  widget->thumbwheel.arm_value = widget->thumbwheel.value;
  widget->thumbwheel.armed = True;

  XtCallCallbackList( w, widget->thumbwheel.arm_callback, NULL );
} // Arm()

/*!
*/

void
Disarm(
  Widget w,
  XEvent *,
  String *,
  Cardinal * )
{
  SoXtThumbWheelWidget widget = (SoXtThumbWheelWidget) w;
  if ( ! widget->thumbwheel.armed ) return;
  widget->thumbwheel.armed = False;
  XtCallCallbackList( w, widget->thumbwheel.disarm_callback, NULL );
} // Disarm()

void
Roll(
  Widget w,
  XEvent * e,
  String *,
  Cardinal * )
{
  assert( e->type == MotionNotify );
  XMotionEvent * event = (XMotionEvent *) e;
  SoXtThumbWheelWidget widget = (SoXtThumbWheelWidget) w;
  if ( ! widget->thumbwheel.armed )
    return;
  int pos = 0;
  switch ( widget->thumbwheel.orientation ) {
  case XmHORIZONTAL:
    pos = event->x - widget->primitive.shadow_thickness - 1 -
          WHEEL_DIAMETER_PADDING;
    break;
  case XmVERTICAL:
    pos = event->y - widget->primitive.shadow_thickness - 1 -
          WHEEL_DIAMETER_PADDING;
    break;
  default:
    assert( 0 );
    break;
  } // switch ( widget->thumbwheel.orientation )

  widget->thumbwheel.value =
    ((SoAnyThumbWheel *) widget->thumbwheel.thumbwheel)->
      CalculateValue( widget->thumbwheel.arm_value,
                      widget->thumbwheel.arm_position,
                      (pos - widget->thumbwheel.arm_position) );

  XtCallCallbackList( w, widget->thumbwheel.valuechanged_callback,
                      &(widget->thumbwheel.value) );
} // Roll()

/*!
*/

void
WheelUp(
  Widget,
  XEvent *,
  String *,
  Cardinal * )
{
  SOXT_STUB();
} // WheelUp()

/*!
*/

void
WheelDown(
  Widget,
  XEvent *,
  String *,
  Cardinal * )
{
  SOXT_STUB();
} // WheelDown()

// *************************************************************************
