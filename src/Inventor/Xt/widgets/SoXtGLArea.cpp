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
 * This file has been slightly modified from the original for use with Mesa.
 *
 *     Jeroen van der Zijp <jvz@cyberia.cfdrc.com>
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

#include <assert.h>
#include <stdio.h>

#include <X11/IntrinsicP.h>
#include <X11/StringDefs.h>
#include <GL/gl.h>
#include <Xm/PrimitiveP.h>

#include <Inventor/errors/SoDebugError.h>

#include <soxtdefs.h>
#include <Inventor/Xt/widgets/SoXtGLAreaP.h>

#define ATTRIBLIST_SIZE 30

// *************************************************************************
// forward declarations:

static void createColormap(SoXtGLAreaWidget, int, XrmValue *);
static void Initialize(SoXtGLAreaWidget, SoXtGLAreaWidget,
                        ArgList, Cardinal *);
static void Realize(Widget, Mask *, XSetWindowAttributes *);
static void Redraw(SoXtGLAreaWidget, XEvent *, Region);
static void Resize(SoXtGLAreaWidget);
static void Destroy(SoXtGLAreaWidget);
static void glwInput(SoXtGLAreaWidget, XEvent *, String *, Cardinal *);
static Boolean SoXtGLArea_set_values(Widget, Widget, Widget, ArgList, Cardinal *);

// *************************************************************************

static
char
SoXtGLArea_defaultTranslations[] =
    "<Key>osfHelp:PrimitiveHelp() \n\
     <KeyDown>:   glwInput() \n\
     <KeyUp>:     glwInput() \n\
     <BtnDown>:   glwInput() \n\
     <BtnUp>:     glwInput() \n\
     <BtnMotion>: glwInput() ";

static
XtActionsRec
SoXtGLArea_actions[] = {
  { "glwInput", (XtActionProc) glwInput }, // key or mouse input
}; // actions

// There is a bit of unusual handling of the resources here.
// Because Xt insists on allocating the colormap resource when it is
// processing the core resources (even if we redeclare the colormap
// resource here, we need to do a little trick.  When Xt first allocates
// the colormap, we allow it to allocate the default one, since we have
// not yet determined the appropriate visual (which is determined from
// resources parsed after the colormap).  We also let it allocate colors
// in that default colormap.
//
// In the initialize proc we calculate the actual visual.  Then, we
// reobtain the colormap resource using XtGetApplicationResources in
// the initialize proc.  If requested, we also reallocate colors in
// that colormap using the same method.

#define coffset(field) XtOffset(SoXtGLAreaWidget, core.field)
#define poffset(field) XtOffset(SoXtGLAreaWidget, primitive.field)
#define offset(field)  XtOffset(SoXtGLAreaWidget, soxtGLArea.field)

static
XtResource
SoXtGLArea_resources[] =
{
  // The GLX attributes.  Add any new attributes here
  {
    SoXtNbufferSize, SoXtCBufferSize, XtRInt,
    sizeof(int), offset(bufferSize),
    XtRImmediate, (XtPointer) 0
  }, {
    SoXtNlevel, SoXtCLevel, XtRInt,
    sizeof(int), offset(level),
    XtRImmediate, (XtPointer) 0
  }, {
    SoXtNrgba, SoXtCRgba, XtRBoolean,
    sizeof(Boolean), offset(rgba),
    XtRImmediate, (XtPointer) FALSE
  }, {
    SoXtNdoublebuffer, SoXtCDoublebuffer, XtRBoolean,
    sizeof(Boolean), offset(doublebuffer),
    XtRImmediate, (XtPointer) FALSE
  }, {
    SoXtNstereo, SoXtCStereo, XtRBoolean,
    sizeof(Boolean), offset(stereo),
    XtRImmediate, (XtPointer) FALSE
  }, {
    SoXtNauxBuffers, SoXtCAuxBuffers, XtRInt,
    sizeof(int), offset(auxBuffers),
    XtRImmediate, (XtPointer) 0
  }, {
    SoXtNredSize, SoXtCColorSize, XtRInt,
    sizeof(int), offset(redSize),
    XtRImmediate, (XtPointer) 1
  }, {
    SoXtNgreenSize, SoXtCColorSize, XtRInt,
    sizeof(int), offset(greenSize),
    XtRImmediate, (XtPointer) 1
  }, {
    SoXtNblueSize, SoXtCColorSize, XtRInt,
    sizeof(int), offset(blueSize),
    XtRImmediate, (XtPointer) 1
  }, {
    SoXtNalphaSize, SoXtCAlphaSize, XtRInt,
    sizeof(int), offset(alphaSize),
    XtRImmediate, (XtPointer) 0
  }, {
    SoXtNdepthSize, SoXtCDepthSize, XtRInt,
    sizeof(int), offset(depthSize),
    XtRImmediate, (XtPointer) 0
  }, {
    SoXtNstencilSize, SoXtCStencilSize, XtRInt,
    sizeof(int), offset(stencilSize),
    XtRImmediate, (XtPointer) 0
  }, {
    SoXtNaccumRedSize, SoXtCAccumColorSize, XtRInt,
    sizeof(int), offset(accumRedSize),
    XtRImmediate, (XtPointer) 0
  }, {
    SoXtNaccumGreenSize, SoXtCAccumColorSize, XtRInt,
    sizeof(int), offset(accumGreenSize),
    XtRImmediate, (XtPointer) 0
  }, {
    SoXtNaccumBlueSize, SoXtCAccumColorSize, XtRInt,
    sizeof(int), offset(accumBlueSize),
    XtRImmediate, (XtPointer) 0
  }, {
    SoXtNaccumAlphaSize, SoXtCAccumAlphaSize, XtRInt,
    sizeof(int), offset(accumAlphaSize),
    XtRImmediate, (XtPointer) 0
  }, { // the attribute list
    SoXtNattribList, SoXtCAttribList, XtRPointer,
    sizeof(int *), offset(attribList),
    XtRImmediate, (XtPointer) NULL
  }, { // the visual info
    SoXtNvisualInfo, SoXtCVisualInfo, SoXtRVisualInfo,
    sizeof(XVisualInfo *), offset(visualInfo),
    XtRImmediate, (XtPointer) NULL
  },
  // miscellaneous resources
  {
    SoXtNinstallColormap, SoXtCInstallColormap, XtRBoolean,
    sizeof(Boolean), offset(installColormap),
    XtRImmediate, (XtPointer) TRUE
  }, {
    SoXtNallocateBackground, SoXtCAllocateColors, XtRBoolean,
    sizeof(Boolean), offset(allocateBackground),
    XtRImmediate, (XtPointer) FALSE
  }, {
    SoXtNallocateOtherColors, SoXtCAllocateColors, XtRBoolean,
    sizeof(Boolean), offset(allocateOtherColors),
    XtRImmediate, (XtPointer) FALSE
  }, {
    SoXtNinstallBackground, SoXtCInstallBackground, XtRBoolean,
    sizeof(Boolean), offset(installBackground),
    XtRImmediate, (XtPointer) TRUE
  }, {
    SoXtNginitCallback, SoXtCCallback, XtRCallback,
    sizeof(XtCallbackList), offset(ginitCallback),
    XtRImmediate, (XtPointer) NULL
  }, {
    SoXtNinputCallback, SoXtCCallback, XtRCallback,
    sizeof(XtCallbackList), offset(inputCallback),
    XtRImmediate, (XtPointer) NULL
  }, {
    SoXtNresizeCallback, SoXtCCallback, XtRCallback,
    sizeof(XtCallbackList), offset(resizeCallback),
    XtRImmediate, (XtPointer) NULL
  }, {
    SoXtNexposeCallback, SoXtCCallback, XtRCallback,
    sizeof(XtCallbackList), offset(exposeCallback),
    XtRImmediate, (XtPointer) NULL
  }, {
    SoXtNrefresh, SoXtCRefresh, XtRBoolean,
    sizeof(Boolean), offset(refresh),
    XtRImmediate, (XtPointer) NULL
  },

  // Changes to Motif primitive resources
  {
    XmNtraversalOn, XmCTraversalOn, XmRBoolean,
    sizeof(Boolean), poffset(traversal_on),
    XmRImmediate, (XtPointer) FALSE
  },
  // highlighting is normally disabled, as when Motif tries to disable
  // highlighting, it tries to reset the color back to the parent's
  // background (usually Motif blue).  Unfortunately, that is in a
  // different colormap, and doesn't work too well.
  {
    XmNhighlightOnEnter, XmCHighlightOnEnter, XmRBoolean,
    sizeof(Boolean), poffset(highlight_on_enter),
    XmRImmediate, (XtPointer) FALSE
  }, {
    XmNhighlightThickness, XmCHighlightThickness, XmRHorizontalDimension,
    sizeof(Dimension), poffset(highlight_thickness),
    XmRImmediate, (XtPointer) 0
  },
}; // resources

/*
** The following resources are reobtained using XtGetApplicationResources
** in the initialize proc.
*/

// The colormap
static XtResource initializeResources[] =
{
  // reobtain the colormap with the new visual
  {
    XtNcolormap, XtCColormap, XtRColormap,
    sizeof(Colormap), coffset(colormap),
    XtRCallProc,(XtPointer) createColormap
  },
}; // initializeResources

// reallocate any colors we need in the new colormap
// The background is obtained only if the allocateBackground resource is TRUE

static XtResource backgroundResources[] =
{
  {
    XmNbackground, XmCBackground, XmRPixel, 
    sizeof(Pixel), coffset(background_pixel),
    XmRString, (XtPointer) "lightgrey"
  }, {
    XmNbackgroundPixmap, XmCPixmap, XmRXmBackgroundPixmap, 
    sizeof(Pixmap), coffset(background_pixmap),
    XmRImmediate, (XtPointer) XmUNSPECIFIED_PIXMAP
  },
}; // backgroundResources
 
// The other colors such as the foreground are allocated only if
// allocateOtherColors are set.  These resources only exist in Motif.

static XtResource otherColorResources[] =
{
  {
    XmNforeground, XmCForeground, XmRPixel, 
    sizeof(Pixel), poffset(foreground),
    XmRString, (XtPointer) "lighgrey"
  }, {
    XmNhighlightColor, XmCHighlightColor, XmRPixel,
    sizeof(Pixel), poffset(highlight_color),
    XmRString, (XtPointer) "lightgrey"
  }, {
    XmNhighlightPixmap, XmCHighlightPixmap, XmRPrimHighlightPixmap,
    sizeof(Pixmap), poffset(highlight_pixmap),
    XmRImmediate, (XtPointer) XmUNSPECIFIED_PIXMAP
  },
}; // otherColorResources

#undef coffset
#undef poffset
#undef offset

SoXtGLAreaClassRec soxtGLAreaClassRec = {
  { // core fields
    (WidgetClass) &xmPrimitiveClassRec,    // superclass
    "SoXtGLArea",                          // class_name
    sizeof(SoXtGLAreaRec),                 // widget_size
    NULL,                                  // class_initialize
    NULL,                                  // class_part_initialize
    FALSE,                                 // class_inited
    (XtInitProc) Initialize,               // initialize
    NULL,                                  // initialize_hook
    Realize,                               // realize
    SoXtGLArea_actions,                    // actions
    XtNumber(SoXtGLArea_actions),          // num_actions
    SoXtGLArea_resources,                  // resources
    XtNumber(SoXtGLArea_resources),        // num_resources
    NULLQUARK,                             // xrm_class
    TRUE,                                  // compress_motion
    TRUE,                                  // compress_exposure
    TRUE,                                  // compress_enterleave
    TRUE,                                  // visible_interest
    (XtWidgetProc) Destroy,                // destroy
    (XtWidgetProc) Resize,                 // resize
    (XtExposeProc) Redraw,                 // expose
    SoXtGLArea_set_values,                 // set_values
    NULL,                                  // set_values_hook
    XtInheritSetValuesAlmost,              // set_values_almost
    NULL,                                  // get_values_hook
    NULL,                                  // accept_focus
    XtVersion,                             // version
    NULL,                                  // callback_private
    SoXtGLArea_defaultTranslations,        // tm_table
    XtInheritQueryGeometry,                // query_geometry
    XtInheritDisplayAccelerator,           // display_accelerator
    NULL                                   // extension
  }, {
    XmInheritBorderHighlight,              // border_highlight
    XmInheritBorderUnhighlight,            // border_unhighlight
    XtInheritTranslations,                 // translations
    NULL,                                  // arm_and_activate
    NULL,                                  // get_resources
    0,                                     // num get_resources
    NULL                                   // extension
  }, {
    (XtPointer) NULL                       // extension
  }
}; // soxtGLAreaClassRec

WidgetClass soxtGLAreaWidgetClass = (WidgetClass) &soxtGLAreaClassRec;

static void
error(
  Widget w,
  char * string)
{
  char buf[100];
  sprintf(buf, "SoXtGLArea: %s\n", string);
  XtAppError(XtWidgetToApplicationContext(w), buf);
} // error()

static void
warning(
  Widget w,
  char * string)
{
  char buf[100];
  sprintf(buf, "SoXtGLArea: %s\n", string);
  XtAppWarning(XtWidgetToApplicationContext(w), buf);
} // warning()

// *************************************************************************

// Initialize the attribList based on the attributes

static void
createAttribList(
  SoXtGLAreaWidget widget)
{
  int * ptr;
  widget->soxtGLArea.attribList =
    (int *) XtMalloc(ATTRIBLIST_SIZE * sizeof(int));
  if (! widget->soxtGLArea.attribList)
    error((Widget) widget, "Unable to allocate attribute list");
  ptr = widget->soxtGLArea.attribList;
  *ptr++ = GLX_BUFFER_SIZE;
  *ptr++ = widget->soxtGLArea.bufferSize;
  *ptr++ = GLX_LEVEL;
  *ptr++ = widget->soxtGLArea.level;
  if (widget->soxtGLArea.rgba) *ptr++ = GLX_RGBA;
  if (widget->soxtGLArea.doublebuffer) *ptr++ = GLX_DOUBLEBUFFER;
  if (widget->soxtGLArea.stereo) *ptr++ = GLX_STEREO;
  *ptr++ = GLX_AUX_BUFFERS;
  *ptr++ = widget->soxtGLArea.auxBuffers;
  *ptr++ = GLX_RED_SIZE;
  *ptr++ = widget->soxtGLArea.redSize;
  *ptr++ = GLX_GREEN_SIZE;
  *ptr++ = widget->soxtGLArea.greenSize;
  *ptr++ = GLX_BLUE_SIZE;
  *ptr++ = widget->soxtGLArea.blueSize;
  *ptr++ = GLX_ALPHA_SIZE;
  *ptr++ = widget->soxtGLArea.alphaSize;
  *ptr++ = GLX_DEPTH_SIZE;
  *ptr++ = widget->soxtGLArea.depthSize;
  *ptr++ = GLX_STENCIL_SIZE;
  *ptr++ = widget->soxtGLArea.stencilSize;
  *ptr++ = GLX_ACCUM_RED_SIZE;
  *ptr++ = widget->soxtGLArea.accumRedSize;
  *ptr++ = GLX_ACCUM_GREEN_SIZE;
  *ptr++ = widget->soxtGLArea.accumGreenSize;
  *ptr++ = GLX_ACCUM_BLUE_SIZE;
  *ptr++ = widget->soxtGLArea.accumBlueSize;
  *ptr++ = GLX_ACCUM_ALPHA_SIZE;
  *ptr++ = widget->soxtGLArea.accumAlphaSize;
  *ptr++ = None;
  assert((ptr - widget->soxtGLArea.attribList) < ATTRIBLIST_SIZE);
} // createAttribList()

// *************************************************************************
// Initialize the visualInfo based on the attribute list

static void
createVisualInfo(
  SoXtGLAreaWidget widget)
{
  static XVisualInfo * visualInfo;
  assert(widget->soxtGLArea.attribList);

  if (widget->soxtGLArea.visualInfo == NULL)
    widget->soxtGLArea.visualInfo = 
      glXChooseVisual(XtDisplay(widget),
                       XScreenNumberOfScreen(XtScreen(widget)),
                       widget->soxtGLArea.attribList);
  if (! widget->soxtGLArea.visualInfo)
    error((Widget) widget, "requested visual not supported");
} // createVisualkInfo()

// *************************************************************************

/* Initialize the colormap based on the visual info.
 * This routine maintains a cache of visual-infos to colormaps.  If two
 * widgets share the same visual info, they share the same colormap.
 * This function is called by the callProc of the colormap resource entry.
 */

static void
createColormap(
  SoXtGLAreaWidget widget,
  int offset,
  XrmValue * value)
{
  static struct cmapCache {
    Visual * visual;
    Colormap cmap;
  } * cmapCache;
  static int cacheEntries = 0;
  static int cacheMalloced = 0;
  register int i;
    
  assert(widget->soxtGLArea.visualInfo);

  // see if we can find it in the cache
  for (i = 0; i < cacheEntries; i++) {
    if (cmapCache[i].visual == widget->soxtGLArea.visualInfo->visual) {
      value->addr = (char *) &cmapCache[i].cmap;
      return;
    }
  }

  // not in the cache, create a new entry
  if (cacheEntries >= cacheMalloced) {
    // need to malloc a new one.  Since we are likely to have only a
    // few colormaps, we allocate one the first time, and double
    // each subsequent time.
    if (cacheMalloced == 0) {
      cacheMalloced = 1;
      cmapCache = (struct cmapCache *) XtMalloc(sizeof(struct cmapCache));
    } else {
      cacheMalloced <<= 1;
      cmapCache = (struct cmapCache *) XtRealloc((char *) cmapCache,
                     sizeof(struct cmapCache) * cacheMalloced);
    }
  }
       
  cmapCache[cacheEntries].cmap =
    XCreateColormap(XtDisplay(widget),
                     RootWindow(XtDisplay(widget),
                                widget->soxtGLArea.visualInfo->screen),
                     widget->soxtGLArea.visualInfo->visual,
                     AllocNone);
  cmapCache[cacheEntries].visual = widget->soxtGLArea.visualInfo->visual;
  value->addr = (char *) &cmapCache[cacheEntries++].cmap;
} // createColormap()

// *************************************************************************

static void
Initialize(
  SoXtGLAreaWidget req,
  SoXtGLAreaWidget neww,
  ArgList args,
  Cardinal * num_args)
{
  // fix size
  if (req->core.width == 0) neww->core.width = 100;
  if (req->core.height == 0) neww->core.width = 100;

  // create the attribute list if needed
  neww->soxtGLArea.myList = FALSE;
  if (neww->soxtGLArea.attribList == NULL) {
    neww->soxtGLArea.myList = TRUE;
    createAttribList(neww);
  }

  // Gotta have it
  assert(neww->soxtGLArea.attribList);

  // determine the visual info if needed
  neww->soxtGLArea.myVisual = FALSE;
  if (neww->soxtGLArea.visualInfo == NULL) {
    neww->soxtGLArea.myVisual = TRUE;
    createVisualInfo(neww);
  }

  // Gotta have that too
  assert(neww->soxtGLArea.visualInfo);

  neww->core.depth = neww->soxtGLArea.visualInfo->depth;

  // Reobtain the colormap and colors in it using XtGetApplicationResources
  XtGetApplicationResources((Widget) neww, neww, initializeResources,
    XtNumber(initializeResources), args, *num_args);

  // obtain the color resources if appropriate
  if (req->soxtGLArea.allocateBackground) {
    XtGetApplicationResources((Widget) neww, neww, backgroundResources,
      XtNumber(backgroundResources), args, *num_args);
  }

  if (req->soxtGLArea.allocateOtherColors) {
    XtGetApplicationResources((Widget) neww, neww, otherColorResources,
      XtNumber(otherColorResources), args, *num_args);
  }
} // Initialize()

// *************************************************************************

static void
Realize(
  Widget widget,
  Mask * valueMask,
  XSetWindowAttributes * attributes)
{
  register SoXtGLAreaWidget glw = (SoXtGLAreaWidget) widget;
  SoXtGLAreaCallbackStruct cb;
  Widget parentShell;
  Status status;
  Window windows[2], * windowsReturn, * windowList;
  int countReturn, i;
   
  // if we haven't requested that the background be both installed and
  // allocated, don't install it.

  if (! (glw->soxtGLArea.installBackground &&
          glw->soxtGLArea.allocateBackground))
    *valueMask &= ~CWBackPixel;
 
  XtCreateWindow(widget, (unsigned int) InputOutput,
    glw->soxtGLArea.visualInfo->visual, *valueMask, attributes);

  // if appropriate, call XSetWMColormapWindows to install the colormap
  if (glw->soxtGLArea.installColormap) {

    // get parent shell
    for (parentShell = XtParent(widget);
          parentShell && ! XtIsShell(parentShell);
          parentShell = XtParent(parentShell)) { }

    if (parentShell && XtWindow(parentShell)) {

      // check to see if there is already a property
      status = XGetWMColormapWindows(XtDisplay(parentShell),
                                      XtWindow(parentShell),
                                      &windowsReturn, &countReturn);
            
      // if no property, just create one
      if (! status) {
        windows[0] = XtWindow(widget);
        windows[1] = XtWindow(parentShell);
        XSetWMColormapWindows(XtDisplay(parentShell), XtWindow(parentShell),
                               windows, 2);
      } else {
        // there was a property, add myself to the beginning
        windowList = (Window *) XtMalloc(sizeof(Window) * (countReturn+1));
        windowList[0] = XtWindow(widget);
        for (i = 0; i < countReturn; i++)
          windowList[i+1] = windowsReturn[i];
        XSetWMColormapWindows(XtDisplay(parentShell), XtWindow(parentShell),
                               windowList, countReturn + 1);
        XtFree((char *) windowList);
        XtFree((char *) windowsReturn);
      }
    } else {
      warning(widget, "Could not set colormap property on parent shell");
    }
  }
  // Invoke init callbacks:
  cb.reason = SoXtCR_GINIT;
  cb.event = NULL;
  cb.width = glw->core.width;
  cb.height = glw->core.height;
  XtCallCallbackList((Widget) glw, glw->soxtGLArea.ginitCallback, &cb);
} // Realize()

// *************************************************************************

static void
Redraw(
  SoXtGLAreaWidget widget,
  XEvent * event,
  Region region)
{
  SoXtGLAreaCallbackStruct cb;
  XtCallbackList cblist;
  if (! XtIsRealized((Widget) widget))
    return;
  cb.reason = SoXtCR_EXPOSE;
  cb.event = event;
  cb.width = widget->core.width;
  cb.height = widget->core.height;
  XtCallCallbackList((Widget) widget, widget->soxtGLArea.exposeCallback, &cb);
} // Redraw()

// *************************************************************************

static void
Resize(
  SoXtGLAreaWidget glw)
{
  SoXtGLAreaCallbackStruct cb;
  if (! XtIsRealized((Widget) glw))
    return;
  cb.reason = SoXtCR_RESIZE;
  cb.event = NULL;
  cb.width = glw->core.width;
  cb.height = glw->core.height;
  XtCallCallbackList((Widget) glw, glw->soxtGLArea.resizeCallback, &cb);
} // Resize()

// *************************************************************************

static void
Destroy(
  SoXtGLAreaWidget glw)
{
  Window * windowsReturn;
  Widget parentShell;
  Status status;
  int countReturn;
  register int i;

  if (glw->soxtGLArea.myList && glw->soxtGLArea.attribList)
    XtFree((char *) glw->soxtGLArea.attribList);

  if (glw->soxtGLArea.myVisual && glw->soxtGLArea.visualInfo)
    XtFree((char *) glw->soxtGLArea.visualInfo);

  // if my colormap was installed, remove it
  if (glw->soxtGLArea.installColormap) {
    // Get parent shell
    for (parentShell = XtParent(glw);
          parentShell && ! XtIsShell(parentShell);
          parentShell = XtParent(parentShell)) { }

    if (parentShell && XtWindow(parentShell)) {
      // make sure there is a property
      status = XGetWMColormapWindows(XtDisplay(parentShell),
                                      XtWindow(parentShell),
                                      &windowsReturn, &countReturn);
            
      // if no property, just return.  If there was a property, continue
      if (status) {
        // search for a match
        for (i = 0; i < countReturn; i++) {
          if (windowsReturn[i] == XtWindow(glw)) {
            // we found a match, now copy the rest down
            for (i++; i < countReturn; i++)
              windowsReturn[i-1] = windowsReturn[i];

            XSetWMColormapWindows(XtDisplay(parentShell),
              XtWindow(parentShell), windowsReturn, countReturn-1);
            break; 
          }
        }
        XtFree((char *) windowsReturn);
      }
    }
  }
} // Destroy()

// *************************************************************************

// Action routine for keyboard and mouse events

static void
glwInput(
  SoXtGLAreaWidget glw,
  XEvent * event,
  String * params,
  Cardinal * numParams)
{
  SoXtGLAreaCallbackStruct cb;
  cb.reason = SoXtCR_INPUT;
  cb.event = event;
  cb.width = glw->core.width;
  cb.height = glw->core.height;
  XtCallCallbackList((Widget) glw, glw->soxtGLArea.inputCallback, &cb);
} // glwInput()

// *************************************************************************

void
SoXtGLAreaMakeCurrent(
  Widget w,
  GLXContext ctx)
{
  glXMakeCurrent(XtDisplay(w), XtWindow(w), ctx);
} // SoXtGLAreaMakeCurrent()

// *************************************************************************

void
SoXtGLAreaSwapBuffers(
  Widget w)
{
  glXSwapBuffers(XtDisplay(w), XtWindow(w));
} // SoXtGLAreaSwapBuffers()

// *************************************************************************

Boolean
SoXtGLArea_set_values(
  Widget current,
  Widget request,
  Widget new_widget,
  ArgList args,
  Cardinal * num_args)
{
  Boolean redisplay = False;
  SoXtGLAreaWidget curcw = (SoXtGLAreaWidget) current;
  SoXtGLAreaWidget reqcw = (SoXtGLAreaWidget) request;
  SoXtGLAreaWidget newcw = (SoXtGLAreaWidget) new_widget;

  if (newcw->soxtGLArea.refresh != curcw->soxtGLArea.refresh) {
    newcw->soxtGLArea.refresh = False;
    redisplay = True;
  }

  return redisplay;
} // SoXtGLArea_set_values()

// *************************************************************************

