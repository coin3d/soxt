#include <stdio.h>

#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <X11/Shell.h>
#include <X11/Xutil.h>

int
main(int argc, char ** argv)
{
  Display * display = XOpenDisplay(NULL);
  XtAppContext context;
  Widget toplevel = XtVaOpenApplication(&context, "SoXt", NULL, 0, &argc, argv,
                                        NULL, topLevelShellWidgetClass, NULL);
  XtVaSetValues(toplevel,
      XtNtitle, "TestWin",
      XtNwidth, 400,
      XtNheight, 300,
      NULL); 
  XSizeHints * hints = XAllocSizeHints();
  hints->width = 400;
  hints->height = 300;
  hints->flags = PSize;
  XSetWMNormalHints(display, XtWindow(toplevel), hints);
  XtRealizeWidget(toplevel);
  XtAppMainLoop(context);
}


