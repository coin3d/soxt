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

#if SOXT_DEBUG
static const char rcsid[] =
  "$Id$";
#endif // SOXT_DEBUG

#include <assert.h>

#include <X11/keysym.h>

#include <Xm/PushB.h>

#include <Inventor/errors/SoDebugError.h>
#include <Inventor/nodes/SoPerspectiveCamera.h>
#include <Inventor/nodes/SoOrthographicCamera.h>

#include <Inventor/Xt/SoXtBasic.h>
#include <Inventor/Xt/SoXt.h>
#include <Inventor/Xt/SoXtResource.h>
#include <Inventor/Xt/viewers/SoAnyPlaneViewer.h>
#include <Inventor/Xt/widgets/SoXtPopupMenu.h>

#include <Inventor/Xt/viewers/SoXtPlaneViewer.h>

#include <Inventor/Xt/common/pixmaps/ortho.xpm>
#include <Inventor/Xt/common/pixmaps/perspective.xpm>

// *************************************************************************

/*!
  Constructor.
*/

SoXtPlaneViewer::SoXtPlaneViewer(
  Widget parent,
  const char * const name,
  SbBool embed,
  SoXtFullViewer::BuildFlag flag,
  SoXtViewer::Type type )
: inherited( parent, name ? name : getDefaultWidgetName(), embed, flag, type, FALSE )
, common( new SoAnyPlaneViewer( this ) )
{
  this->constructor( TRUE );
} // SoXtPlaneViewer()

/*!
  Protected constructor for deriving classes.
*/

SoXtPlaneViewer::SoXtPlaneViewer( // protected
  Widget parent,
  const char * const name,
  SbBool embed,
  SoXtFullViewer::BuildFlag flag,
  SoXtViewer::Type type,
  SbBool build )
: inherited( parent, name ? name : getDefaultWidgetName(), embed, flag, type, FALSE )
, common( new SoAnyPlaneViewer( this ) )
{
  this->constructor( build );
} // SoXtPlaneViewer()

/*!
  Common constructor code.
*/

void
SoXtPlaneViewer::constructor(
  SbBool build )
{
  this->mode = IDLE_MODE;

  this->prefshell = this->prefsheet = (Widget) NULL;
  this->prefparts = NULL;
  this->numprefparts = 0;

  this->setClassName( this->getDefaultWidgetName() );

  if ( build ) {
    Widget viewer = this->buildWidget( this->getParentWidget() );
    this->setBaseWidget( viewer );
    XtVaSetValues( viewer,
      XmNleftAttachment, XmATTACH_FORM,
      XmNtopAttachment, XmATTACH_FORM,
      XmNrightAttachment, XmATTACH_FORM,
      XmNbottomAttachment, XmATTACH_FORM,
      NULL );

    SoXtResource rsc( this->getRightWheelLabelWidget() );
    char * dollyString = NULL;
    rsc.getResource( "dollyString", XmRString, dollyString );
    if ( dollyString != NULL )
      this->setRightWheelString( dollyString );
  }
} // constructor()

/*!
  Destructor.
*/

SoXtPlaneViewer::~SoXtPlaneViewer(
  void )
{
  delete this->common;
} // ~SoXtPlaneViewer()

// *************************************************************************

/*!
*/

void
SoXtPlaneViewer::setViewing( // virtual
  SbBool enable )
{
  inherited::setViewing( enable );
} // setViewing()

/*!
*/

void
SoXtPlaneViewer::setCamera( // virtual
  SoCamera * camera )
{
  Pixmap pixmap, pixmap_ins;
  if ( camera == NULL ) {
    // find better pixmaps for this...
    pixmap = this->pixmaps.ortho;
    pixmap_ins = this->pixmaps.ortho_ins;
  } else if ( camera->isOfType( SoPerspectiveCamera::getClassTypeId() ) ) {
    pixmap = this->pixmaps.perspective;
    pixmap_ins = this->pixmaps.perspective_ins;
    SoXtResource rsc( this->getRightWheelLabelWidget() );
    char * dollyString = NULL;
    rsc.getResource( "dollyString", XmRString, dollyString );
    if ( dollyString != NULL )
      this->setRightWheelString( dollyString );
  } else if ( camera->isOfType( SoOrthographicCamera::getClassTypeId() ) ) {
    pixmap = this->pixmaps.ortho;
    pixmap_ins = this->pixmaps.ortho_ins;
    SoXtResource rsc( this->getRightWheelLabelWidget() );
    char * zoomString = NULL;
    rsc.getResource( "zoomString", XmRString, zoomString );
    if ( zoomString != NULL )
      this->setRightWheelString( zoomString );
  } else {
    SoDebugError::postWarning( "SoXtExaminerViewer::setCamera",
      "unknown camera type - got no pixmap" );
    // find better pixmaps for this...
    pixmap = this->pixmaps.ortho;
    pixmap_ins = this->pixmaps.ortho_ins;
  }

  XtUnmanageChild( this->buttons.camera );
#if HAVE_LIBXPM
  XtVaSetValues( this->buttons.camera,
    XmNlabelType, XmPIXMAP,
    XmNlabelPixmap, pixmap,
    XmNselectPixmap, pixmap,
    XmNlabelInsensitivePixmap, pixmap_ins,
    XmNselectInsensitivePixmap, pixmap_ins,
    NULL );
#endif // HAVE_LIBXPM
  XtVaSetValues( this->buttons.camera,
    XmNwidth, 30, XmNheight, 30, NULL );
  XtManageChild( this->buttons.camera );

  inherited::setCamera( camera );
} // setCamera()

/*!
*/

void
SoXtPlaneViewer::setCursorEnabled( // virtual
  SbBool enable )
{
  inherited::setCursorEnabled( enable );
} // setCursorEnabled()

// *************************************************************************

/*!
  Overloads to add the x, y, z, and camera-toggle buttons to the list of
  viewer buttons.
*/

void
SoXtPlaneViewer::createViewerButtons( // virtual, protected
  Widget parent,
  SbPList * buttonlist )
{
  inherited::createViewerButtons( parent, buttonlist );

  this->buttons.x = XtVaCreateManagedWidget( "xbutton",
    xmPushButtonWidgetClass, parent,
    XmNshadowType, XmSHADOW_OUT,
    XmNhighlightThickness, 0,
    XmNshadowThickness, 2,
    XmNtraversalOn, False,
    XmNwidth, 30,
    XmNheight, 30,
    XtVaTypedArg,
      XmNlabelString, XmRString,
      "X", strlen( "X" ) + 1,
    NULL );

  buttonlist->append( this->buttons.x );

  XtAddCallback( this->buttons.x, XmNactivateCallback,
    SoXtPlaneViewer::xbuttonCB, (XtPointer) this );

  this->buttons.y = XtVaCreateManagedWidget( "ybutton",
    xmPushButtonWidgetClass, parent,
    XmNshadowType, XmSHADOW_OUT,
    XmNhighlightThickness, 0,
    XmNshadowThickness, 2,
    XmNtraversalOn, False,
    XmNwidth, 30,
    XmNheight, 30,
    XtVaTypedArg,
      XmNlabelString, XmRString,
      "Y", strlen( "Y" ) + 1,
    NULL );

  buttonlist->append( this->buttons.y );

  XtAddCallback( this->buttons.y, XmNactivateCallback,
    SoXtPlaneViewer::ybuttonCB, (XtPointer) this );

  this->buttons.z = XtVaCreateManagedWidget( "zbutton",
    xmPushButtonWidgetClass, parent,
    XmNshadowType, XmSHADOW_OUT,
    XmNhighlightThickness, 0,
    XmNshadowThickness, 2,
    XmNtraversalOn, False,
    XmNwidth, 30,
    XmNheight, 30,
    XtVaTypedArg,
      XmNlabelString, XmRString,
      "Z", strlen( "Z" ) + 1,
    NULL );

  buttonlist->append( this->buttons.z );

  XtAddCallback( this->buttons.z, XmNactivateCallback,
    SoXtPlaneViewer::zbuttonCB, (XtPointer) this );

  this->buttons.camera = XtVaCreateManagedWidget( "C",
    xmPushButtonWidgetClass, parent,
    XmNshadowType, XmSHADOW_OUT,
    XmNhighlightThickness, 0,
    XmNshadowThickness, 2,
    XmNtraversalOn, False,
    XmNwidth, 30,
    XmNheight, 30,
    XtVaTypedArg,
      XmNlabelString, XmRString,
      "C", strlen( "C" ) + 1,
    NULL );

  XtAddCallback( this->buttons.camera, XmNactivateCallback,
    SoXtPlaneViewer::camerabuttonCB, this );

  buttonlist->append( this->buttons.camera );

#if HAVE_LIBXPM
  this->pixmaps.ortho =
    createPixmapFromXpmData( this->buttons.camera, ortho_xpm );
  this->pixmaps.ortho_ins =
    createInsensitivePixmapFromXpmData( this->buttons.camera, ortho_xpm );
  this->pixmaps.perspective =
    createPixmapFromXpmData( this->buttons.camera, perspective_xpm );
  this->pixmaps.perspective_ins =
    createInsensitivePixmapFromXpmData( this->buttons.camera, perspective_xpm );
#endif // HAVE_LIBXPM

} // createViewerButtons()

/*!
*/

const char *
SoXtPlaneViewer::getDefaultWidgetName(
  void ) const
{
  static const char defaultWidgetName[] = "SoXtPlaneViewer";
  return defaultWidgetName;
} // getDefaultWidgetName()

/*!
*/

const char *
SoXtPlaneViewer::getDefaultTitle(
  void) const
{
  static const char defaultTitle[] = "Plane Viewer";
  return defaultTitle;
} // getDefaultTitle()

/*!
*/

const char *
SoXtPlaneViewer::getDefaultIconTitle(
  void ) const
{
  static const char defaultIconTitle[] = "Plane Viewer";
  return defaultIconTitle;
} // getDefaultIconTitle()

// *************************************************************************

/*!
*/

void
SoXtPlaneViewer::processEvent(
  XAnyEvent * event )
{
  if ( SoXtViewer::processCommonEvents( event ) )
    return;

/*
  if ( ! this->mapped ) {
    this->mapped = TRUE; // Must be set before setCursorRepresentation() call.
    this->setCursorRepresentation( this->mode );
  }
*/

  SbVec2s canvassize( this->getGLSize() );
  SbVec2s mousepos( -1, -1 );

  switch ( event->type ) {
  case ButtonPress:
  case ButtonRelease:
    mousepos[0] = ((XButtonEvent *)event)->x;
    mousepos[1] = canvassize[1] - ((XButtonEvent *)event)->y;
    break;
  case MotionNotify:
    mousepos[0] = ((XPointerMovedEvent *)event)->x;
    mousepos[1] = canvassize[1] - ((XPointerMovedEvent *)event)->y;
    break;
  default:
    break;
  }

  SbVec2f mousepos_normalized;
  mousepos_normalized[0] = (float) mousepos[0] / (float) canvassize[0];
  mousepos_normalized[1] = (float) mousepos[1] / (float) canvassize[1];

  switch ( event->type ) {
  case ButtonPress:
    common->setPointerLocation( mousepos );
    common->setPointerLocation( mousepos );
    this->prevMousePosition = mousepos_normalized;
    switch ( ((XButtonEvent *) event)->button ) {
    case 1:
      if ( this->mode == IDLE_MODE ) {
        this->interactiveCountInc();
        this->mode = DOLLY_MODE;
      } else if ( this->mode == SEEK_WAIT_MODE ) {
        this->interactiveCountInc();
        this->seekToPoint( mousepos );
      }
      break;

    case 2:
      if ( this->mode == IDLE_MODE ) {
        this->interactiveCountInc();
        this->mode = TRANSLATE_MODE;
      } else if ( this->mode == ROTZ_WAIT_MODE ) {
        this->mode = ROTZ_MODE;
      }
      break;

    case 3:
      if ( this->isPopupMenuEnabled() ) {
        int x = ((XButtonEvent *) event)->x_root;
        int y = ((XButtonEvent *) event)->y_root;
        if ( ! this->prefmenu )
          this->buildPopupMenu();
        this->prefmenu->PopUp( this->getGLWidget(), x, y );
        return;
      }
      break;

    case 4:
      this->zoom( 0.1f );
      return;

    case 5:
      this->zoom( -0.1f );
      return;

    default:
      break;
    } // switch ( button )

    break;

  case ButtonRelease:
    switch ( ((XButtonEvent *) event)->button ) {
    case 1:
    case 2:
      if ( this->mode != IDLE_MODE && this->mode != ROTZ_MODE ) {
        this->interactiveCountDec();
        this->mode = IDLE_MODE;
      }
      break;

    default:
      break;
    } // switch ( ((XButtonEvent *) event)->button )
    break;

  case MotionNotify:
    switch ( this->mode ) {
    case DOLLY_MODE:
      if ( mousepos_normalized[1] != this->prevMousePosition[1] ) {
        float value = this->getRightWheelValue() +
          (this->prevMousePosition[1] - mousepos_normalized[1]) * 10.0f;
        this->rightWheelMotion( value );
        this->setRightWheelValue( value );
      }
      break;

    case TRANSLATE_MODE:
      if ( (mousepos_normalized[0] != this->prevMousePosition[0]) ||
           (mousepos_normalized[1] != this->prevMousePosition[1]) ) {
        float dx = (mousepos_normalized[0] - this->prevMousePosition[0]) * 2.5f;
        float dy = (mousepos_normalized[1] - this->prevMousePosition[1]) * 2.5f;
        float yvalue = this->getLeftWheelValue() - dy;
        float xvalue = this->getBottomWheelValue() + dx;
        this->leftWheelMotion( yvalue );
        this->setLeftWheelValue( yvalue );
        this->bottomWheelMotion( xvalue );
        this->setBottomWheelValue( xvalue );
      }
      break;

    case ROTZ_MODE:
    {
      common->setPointerLocation( mousepos );
      common->rotateZ( common->getPointerOrigoMotionAngle() );
      break;
    }

    default:
      break;
    } // switch ( this->mode )
    break;

  case KeyPress:
  case KeyRelease:
  {
    char keybuf[8];
    KeySym keysym = 0;

//    int keybuflen =
      XLookupString( (XKeyEvent *) event, keybuf, 8, &keysym, NULL );

    switch ( keysym ) {
    case XK_Control_L:
    case XK_Control_R:
      if ( event->type == KeyPress ) {
        this->mode = ROTZ_MODE;
        common->setCanvasSize( canvassize );
        common->setPointerLocation( mousepos );
        common->setPointerLocation( mousepos );
	this->scheduleRedraw();
      } else {
        this->mode = IDLE_MODE;
	this->scheduleRedraw();
      }
      break;
    default:
      break;
    }

    break;
  }

  default:
    break;

  } // switch ( event->type )

  this->prevMousePosition = mousepos_normalized;

  inherited::processEvent( event );
} // processEvent()

// *************************************************************************

/*!
*/

void
SoXtPlaneViewer::setSeekMode(
  SbBool enable )
{
  SOXT_STUB_ONCE();
  inherited::setSeekMode( enable );
} // setSeekMode()

/*!
*/

void
SoXtPlaneViewer::actualRedraw(
  void )
{
  inherited::actualRedraw();
  if ( this->mode == ROTZ_MODE )
    common->drawRotateGraphics();
} // actualRedraw()

/*!
*/
void
SoXtPlaneViewer::openViewerHelpCard( // virtual
  void )
{
  this->openHelpCard( "SoXtPlaneViewer.help" );
} // openViewerHelpCard()

// *************************************************************************

/*!
  This method set up the bottom wheel to control camera translation in the
  horizontal direction.
*/

void
SoXtPlaneViewer::bottomWheelMotion( // virtual
  float value )
{
  common->translateX( value - this->getBottomWheelValue() );
  inherited::bottomWheelMotion( value );
} // bottomWheelMotion()

/*!
  This method set up the left wheel to control camera translation in the
  vertical direction.
*/

void
SoXtPlaneViewer::leftWheelMotion( // virtual
  float value )
{
  common->translateY( value - this->getLeftWheelValue() );
  inherited::leftWheelMotion( value );
} // leftWheelMotion()

/*!
  This method set up the right wheel to control camera movement in the
  inwards direction.
*/

void
SoXtPlaneViewer::rightWheelMotion( // virtual
  float value )
{
  this->zoom( this->getRightWheelValue() - value );
  inherited::rightWheelMotion( value );
} // rightWheelMotion()

/*!
  This method moves the camera inwards and outwards.  It will be moved to
  common code soon.
*/

void
SoXtPlaneViewer::zoom(
  const float difference )
{
  SoCamera * camera = this->getCamera();
  assert( camera != NULL );

  SoType type = camera->getTypeId();
  float multiplicator = exp( difference ); // in the range of <0, ->>

  if ( type.isDerivedFrom( SoOrthographicCamera::getClassTypeId() ) ) {
    SoOrthographicCamera * orthocam = (SoOrthographicCamera *) camera;
    orthocam->height = orthocam->height.getValue() * multiplicator;
  } else if ( type.isDerivedFrom( SoPerspectiveCamera::getClassTypeId() ) ) {
    float oldfocaldist = camera->focalDistance.getValue();
    camera->focalDistance = oldfocaldist * multiplicator;
    SbVec3f direction;
    camera->orientation.getValue().multVec( SbVec3f( 0, 0, -1 ), direction );
    camera->position = camera->position.getValue() +
      (camera->focalDistance.getValue() - oldfocaldist) * -direction;
  } else {
    assert( 0 && "impossible" );
  }
} // zoom()

// *************************************************************************

/*!
  This method creates the preferences sheet widgets for the Plane Viewer
  prefereences window.
*/

void
SoXtPlaneViewer::createPrefSheet(
  void )
{
  if ( ! this->prefshell ) {
    this->prefparts = new Widget [ 16 ];
    this->createPrefSheetShellAndForm( this->prefshell, this->prefsheet );
    this->createDefaultPrefSheetParts( this->prefparts, this->numprefparts,
      this->prefsheet );
    // add parts specific for derived viewer
  }
  this->layoutPartsAndMapPrefSheet( this->prefparts, this->numprefparts,
    this->prefsheet, this->prefshell );
} // createPrefSheet()

// *************************************************************************

/*!
*/

SOXT_WIDGET_CALLBACK_IMPLEMENTATION(
  SoXtPlaneViewer,
  xbutton )
{
  common->viewPlaneX();
} // xbutton()

/*!
*/

SOXT_WIDGET_CALLBACK_IMPLEMENTATION(
  SoXtPlaneViewer,
  ybutton )
{
  common->viewPlaneY();
} // ybutton()

/*!
*/

SOXT_WIDGET_CALLBACK_IMPLEMENTATION(
  SoXtPlaneViewer,
  zbutton )
{
  common->viewPlaneZ();
} // zbutton()

/*!
*/

SOXT_WIDGET_CALLBACK_IMPLEMENTATION(
  SoXtPlaneViewer,
  camerabutton )
{
  this->toggleCameraType();
} // camerabutton()

// *************************************************************************

#if SOXT_DEBUG
static const char * getSoXtPlaneViewerRCSId(void) { return rcsid; }
#endif // SOXT_DEBUG

