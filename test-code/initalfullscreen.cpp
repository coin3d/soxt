// This might have problems if the window manager is Motif based.
// Example provided by Fernando Vega.

#include <X11/IntrinsicP.h>


int
main(
  int argc,
  char ** argv )
{

  XtAppContext app_context;
  Widget TheWindow = XtVaAppInitialize( &app_context,
				        "Full Screen Test",
				        NULL, 0,
				        &argc, argv,
				        NULL,
				        NULL );

  if (TheWindow == NULL) exit(1);

  Display *display=XOpenDisplay(NULL);

  int screen = DefaultScreen(display);

  // Destroy the window asociated to the widget
  XtUnrealizeWidget( TheWindow );

  // Put some values in the size, otherwise the window creation will complain
  XtConfigureWidget( TheWindow, 0, 0, 10, 10, 0 );

  // Now set the attributes for the new window
  XSetWindowAttributes attr;

  // We'll handle it; make the Window manager leave us alone
  attr.override_redirect = true;

  Visual *visual =  DefaultVisual(display,screen);

  // Create a window with our attributes
  XtCreateWindow( TheWindow, InputOutput, visual, CWOverrideRedirect, &attr );

  SoXt::init( TheWindow );

  SoXtExaminerViewer *Viewer =
    new SoXtExaminerViewer(TheWindow, "My Examiner Viewer");

  Viewer->setDecoration(FALSE);
  Viewer->setBorder(FALSE);

  SoXt::show(TheWindow);

  // Set size and position; no border
  XtConfigureWidget( TheWindow, 0, 0,
		     DisplayWidth( display, DefaultScreen( display ) ),
		     DisplayHeight( display, DefaultScreen( display ) ), 0 );


  SoXt::mainLoop();

}

