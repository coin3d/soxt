#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#include <X11/xpm.h>
#include <X11/Xmu/StdCmap.h>
#include <X11/Xmu/Atoms.h>

#include <Xm/Xm.h>
#include <Xm/Form.h>
#include <Xm/PushB.h>

// define to 0 for default visual - a visualid to get specific visual
#define WANT_VISUALID 0

static const char * home_xpm[] = {
  "24 24 11 1",
  " 	c None",
  ".	c #000000",
  "+	c #808080",
  "@	c #898989",
  "#	c #C0C0C0",
  "$	c #F9F9F9",
  "%	c #424242",
  "&	c #A4A4A4",
  "*	c #5D5D5D",
  "=	c #C1C1C1",
  "-	c #DFDFDF",
  "                        ", "                        ", "                        ",
  "           ..           ", "           ..           ", "      .............     ",
  "     ..+............    ", "    ..+@+............   ", "   ..+###+............  ",
  "  ..+#$$$#+............ ", " ...#$$$$$#+.......%%   ", "   .$+&$$$+++....%**%   ",
  "   .$&#$$$+#$.*%**%+%   ", "   .$##$$$=#$$**++++%   ", "   .$$$$$$--$$++++%+%   ",
  "   .$$$$$$$$$$+%++*+%   ", "   .$$$@@#-$$$+*++++%   ", "   .$$$@##-$$$+%+++%%   ",
  "   .$$$###-$$$+++%%     ", "   .%+####-$$$+%%       ", "     %%%%%%%..%         ",
  "                        ", "                        ", "                        "
};


Pixmap
createPixmapFromXpm(Widget widget, const char ** xpm)
{
  Pixmap pixels = 0;
  Widget shell = widget;
  while (! XtIsShell(shell) && shell != (Widget) NULL) {
    shell = XtParent(shell);
  }
  assert(shell != (Widget) NULL);
  fprintf(stderr, "shell: %p\n", shell);
  Display * display = XtDisplay(shell);

  XpmAttributes attrs;
  attrs.visual = NULL;
  attrs.colormap = 0;
  attrs.depth = 0;
  attrs.valuemask = XpmVisual | XpmColormap | XpmDepth;

  XtVaGetValues(shell, XmNcolormap, &attrs.colormap, NULL);
  XtVaGetValues(shell, XmNdepth, &attrs.depth, NULL);
  XtVaGetValues(shell, XmNvisual, &attrs.visual, NULL);
  if ( attrs.visual == NULL ) {
    fprintf(stderr, "attrs.visual for shell was NULL - using default\n");
    attrs.visual = DefaultVisual(display, DefaultScreen(display));
    attrs.depth = DefaultDepth(display, DefaultScreen(display));
    attrs.colormap = DefaultColormap(display, DefaultScreen(display));
  }

  /* I'm questioning the use of the root Window here... */
  /* An alternative approach could maybe be to set it up once the window was
   * created (so we can use the actual window in question) through the use
   * of XtAddCallback()? */

  Drawable draw = RootWindow(display, DefaultScreen(display));
  Pixmap stencil = 0;

  // FIXME: that cast is pretty nasty -- get rid of it. 20020319 mortene.
  int error = XpmCreatePixmapFromData(display, draw, (char **)xpm, &pixels, &stencil, &attrs);

  if (error != XpmSuccess) {
    fprintf(stderr, "XpmCreatePixmapFromData() failed: %d", error);
    return (Pixmap) 0;
  }

  if (stencil) {
    Pixel bg;
    XtVaGetValues(widget, XmNbackground, &bg, NULL);

    XImage * pixmap = XGetImage(display, pixels, 0, 0, attrs.width, attrs.height,
                                0xffffffff, ZPixmap);
    XImage * mask = XGetImage(display, stencil, 0, 0, attrs.width, attrs.height,
                              0xffffffff, ZPixmap);
    assert(pixmap != NULL && mask != NULL);

    for (unsigned int x = 0; x < attrs.width; x++) {
      for (unsigned int y = 0; y < attrs.height; y++) {
        Pixel pixel = XGetPixel(mask, x, y);
        Bool usebg = (pixel == 0);
        if (usebg) { // background must be set in image
          XPutPixel(pixmap, x, y, bg);
        }
      }
    }

    GC temp = XCreateGC(display, pixels, 0, NULL);
    XPutImage(display, pixels, temp, pixmap,
              0, 0, 0, 0, attrs.width, attrs.height);
    XFreeGC(display, temp);

    XDestroyImage(pixmap);
    XDestroyImage(mask);
  }
  return pixels;
}

int
main(int argc, char ** argv)
{

  setbuf(stdout, NULL);
  setbuf(stderr, NULL);
  
  Display * display = XOpenDisplay(NULL);
  assert(display != NULL);
  XSynchronize(display, True);

  XtAppContext context = XtCreateApplicationContext();

  Widget appshell = XtVaAppInitialize(&context, "SoXtTest", NULL, 0, &argc, argv, NULL, NULL);
  fprintf(stderr, "appshell: %p\n", appshell);

#if WANT_VISUALID
  int i, numvisuals;
  unsigned int wanted = WANT_VISUALID;
  XVisualInfo templ;
  XVisualInfo * visuals = XGetVisualInfo(display, VisualNoMask, &templ, &numvisuals);
  for ( i = 0; i < numvisuals; i++ ) {
    if ( visuals[i].visualid == wanted ) goto selected;
  }
  assert(0 && "no visual selected");
selected:

  Visual * visual = visuals[i].visual;
  int visualid = visuals[i].visualid;
  int depth = visuals[i].depth;
  Colormap colormap = 0;

  fprintf(stderr, "visualid: %d, depth: %d, class: %s\n", visualid, depth, visuals[i].c_class == DirectColor ? "DirectColor" : "Other");

  int numcmaps;
  XStandardColormap * stdcolormaps = NULL;
  if ( XmuLookupStandardColormap(display, visuals[i].screen, visuals[i].visualid, visuals[i].depth,
                                 XA_RGB_DEFAULT_MAP, False, True) &&
       XGetRGBColormaps(display, RootWindow(display, visuals[i].screen),
                        &stdcolormaps, &numcmaps, XA_RGB_DEFAULT_MAP) ) {
    for ( int j = 0; j < numcmaps; j++ ) {
      if (stdcolormaps[j].visualid == visuals[i].visualid) {
        colormap = stdcolormaps[j].colormap;
        goto cont;
      }
    }
    colormap = XCreateColormap(display, RootWindow(display, visuals[i].screen), visuals[i].visual, AllocNone);
    fprintf(stderr, "standard RGB colormaps did not work with visual - created own (%ld)", colormap);
  } else {
    assert(0);
  }

cont:
  fprintf(stderr, "colormap: %ld\n", colormap);
#else
  Visual * visual = NULL;
  int depth = 0;
  Colormap colormap = 0;

  int snum = XDefaultScreen(display);
  visual = XDefaultVisual(display, snum);
  depth = XDefaultDepth(display, snum);
  colormap = XDefaultColormap(display, snum);

  fprintf(stderr, "visual: %p, depth: %d\n", visual, depth);
  fprintf(stderr, "colormap: %ld\n", colormap);
#endif

  XtVaSetValues(appshell,
    XmNwidth, 100,
    XmNheight, 100,
    XmNvisual, visual,
    XmNcolormap, colormap,
    XmNdepth, depth,
    NULL);

  Widget form = XtVaCreateManagedWidget(
    "form", xmFormWidgetClass,
    appshell,
    NULL);

  Widget button = XtVaCreateManagedWidget(
    "button", xmPushButtonWidgetClass,
    form,
    XmNtopAttachment, XmATTACH_FORM,
    XmNleftAttachment, XmATTACH_FORM,
    XmNbottomAttachment, XmATTACH_FORM,
    XmNrightAttachment, XmATTACH_FORM,
    NULL);

  Pixmap pixmap = createPixmapFromXpm(button, home_xpm);
  XtVaSetValues(button,
    XmNlabelType, XmPIXMAP,
    XmNlabelPixmap, pixmap,
    XmNlabelInsensitivePixmap, pixmap,
    XmNselectPixmap, pixmap,
    XmNselectInsensitivePixmap, pixmap,
    NULL);

  Widget list[] = { appshell, form, button, NULL };
  XtSetWMColormapWindows(appshell, list, 3);

  XtRealizeWidget(appshell);
  XtAppMainLoop(context);
}

