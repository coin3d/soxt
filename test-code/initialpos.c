/*
 * This minimal program is for trying to fix the problem of Motif
 * placing new windows at position 0,0.  Some WMs are aware of that
 * bug and ignores it, but some WMs aren't, so it is best to fix it
 * on the application side.
 *
 * gcc -I/usr/X11R6/include -L/usr/X11R6/lib -o initialpos initialpos.c -lXm -lXt -lX11
 */

#include <stdio.h>

#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <X11/Shell.h>
#include <X11/Xutil.h>

int
main(int argc, char ** argv)
{
  Display * display;
  XtAppContext context;
  XSizeHints * hints;
  long supplied;
  Widget toplevel, w;
  Window toplevelwin;
  
  setbuf(stdout, NULL);
  setbuf(stderr, NULL);

  display = XOpenDisplay(NULL);
  toplevel = XtVaOpenApplication(&context, "SoXt", NULL, 0, &argc, argv,
                                 NULL, topLevelShellWidgetClass, NULL);
  XtVaSetValues(toplevel, XtNtitle, "TestWin", XtNwidth, 400, XtNheight, 300, NULL); 
  XtRealizeWidget(toplevel);

  /*
   ** One of the hints in the WM_NORMAL_HINTS protocol, PPlacement, gets set by
   ** the X toolkit (probably part of the Core or Shell widget) when a shell
   ** widget is realized to the value stored in the XmNx and XmNy resources of the
   ** Core widget.  While callers can set these values, there is no "unset" value
   ** for these resources.  On systems which are more Motif aware, a PPosition
   ** hint of 0,0, which is the default for XmNx and XmNy, is interpreted as,
   ** "place this as if no hints were specified".  Unfortunately the fvwm family
   ** of window managers, which are now some of the most popular, interpret this
   ** as "place this window at (0,0)".  This routine intervenes between the
   ** realizing and the mapping of the window to remove the inappropriate
   ** PPlacement hint.
   */

  w = toplevel;
  // while ( !XtIsWMShell(w) ) w = XtParent(w);
  toplevelwin = XtWindow(w);
  fprintf(stderr, "toplevelwin: %ld\n", toplevelwin);

  hints = XAllocSizeHints();

  /* FIXME: why does the next line crash with BadWindow when linking with Motif? */
  if ( XGetWMNormalHints(display, toplevelwin, hints, &supplied) ) {
    fprintf(stderr, "removing position hint\n");
    /* Get rid of the incorrect WMNormal PPosition hint */
    hints->flags &= ~PPosition;
    XSetWMNormalHints(display, toplevelwin, hints);
  }
  XFree(hints);
  XtAppMainLoop(context);
}

