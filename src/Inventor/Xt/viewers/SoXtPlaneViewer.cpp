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

#include <Xm/PushB.h>

#include <Inventor/errors/SoDebugError.h>
#include <Inventor/nodes/SoPerspectiveCamera.h>
#include <Inventor/nodes/SoOrthographicCamera.h>

#include <Inventor/Xt/SoXtBasic.h>
#include <Inventor/Xt/SoXt.h>
#include <Inventor/Xt/viewers/SoAnyPlaneViewer.h>

#include <Inventor/Xt/common/pixmaps/ortho.xpm>
#include <Inventor/Xt/common/pixmaps/perspective.xpm>

#include <Inventor/Xt/viewers/SoXtPlaneViewer.h>

// *************************************************************************

/*!
  Constructor.
*/

SoXtPlaneViewer::SoXtPlaneViewer(
  Widget parent,
  const char * const name,
  SbBool inParent,
  SoXtFullViewer::BuildFlag flag,
  SoXtViewer::Type type )
: inherited( parent, name, inParent, flag, type, FALSE )
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
  SbBool inParent,
  SoXtFullViewer::BuildFlag flag,
  SoXtViewer::Type type,
  SbBool build )
: inherited( parent, name, inParent, flag, type, FALSE )
, common( new SoAnyPlaneViewer( this ) )
{
} // SoXtPlaneViewer()

/*!
  Common constructor code.
*/

void
SoXtPlaneViewer::constructor(
  SbBool build )
{
  this->prefshell = this->prefsheet = (Widget) NULL;
  this->prefparts = NULL;
  this->numprefparts = 0;

  this->setLeftWheelString( "Trans Y" );
  this->setBottomWheelString( "Trans X" );
  this->setRightWheelString( "Dolly" );
  this->setClassName( this->getDefaultWidgetName() );
  this->setTitle( "Plane Viewer" );

  if ( build ) {
    Widget viewer = this->buildWidget( this->getParentWidget() );
    this->setBaseWidget( viewer );
  }
} // constructor()

/*!
  Destructor.
*/

SoXtPlaneViewer::~SoXtPlaneViewer(
  void )
{
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
    this->setRightWheelString( "Dolly" );
  } else if ( camera->isOfType( SoOrthographicCamera::getClassTypeId() ) ) {
    pixmap = this->pixmaps.ortho;
    pixmap_ins = this->pixmaps.ortho_ins;
    this->setRightWheelString( "Zoom" );
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

  XmString labelstring;

  labelstring = SoXt::encodeString( "X" );
  this->buttons.x = XtVaCreateManagedWidget( "xbutton",
    xmPushButtonWidgetClass, parent,
    XmNlabelString, labelstring,
    XmNhighlightThickness, 0,
    XmNshadowThickness, 2,
    XmNwidth, 30,
    XmNheight, 30,
    NULL );
  XtFree( (char *) labelstring );
  buttonlist->append( this->buttons.x );

  XtAddCallback( this->buttons.x, XmNactivateCallback,
    SoXtPlaneViewer::xbuttonCB, (XtPointer) this );

  labelstring = SoXt::encodeString( "Y" );
  this->buttons.y = XtVaCreateManagedWidget( "ybutton",
    xmPushButtonWidgetClass, parent,
    XmNlabelString, labelstring,
    XmNhighlightThickness, 0,
    XmNshadowThickness, 2,
    XmNwidth, 30,
    XmNheight, 30,
    NULL );
  XtFree( (char *) labelstring );
  buttonlist->append( this->buttons.y );

  XtAddCallback( this->buttons.y, XmNactivateCallback,
    SoXtPlaneViewer::ybuttonCB, (XtPointer) this );

  labelstring = SoXt::encodeString( "Z" );
  this->buttons.z = XtVaCreateManagedWidget( "zbutton",
    xmPushButtonWidgetClass, parent,
    XmNlabelString, labelstring,
    XmNhighlightThickness, 0,
    XmNshadowThickness, 2,
    XmNwidth, 30,
    XmNheight, 30,
    NULL );
  XtFree( (char *) labelstring );
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
  inherited::processEvent( event );
} // processEvent()

/*!
*/

void
SoXtPlaneViewer::setSeekMode(
  SbBool enable )
{
  SOXT_STUB_ONCE();
} // setSeekMode()

/*!
*/

void
SoXtPlaneViewer::actualRedraw(
  void )
{
  inherited::actualRedraw();
  // draw the roll graphics from here
} // actualRedraw()

/*!
*/

void
SoXtPlaneViewer::openViewerHelpCard( // virtual
  void )
{
  this->openHelpCard( "SoXtPlaneViewer.help" );
} // openViewerHelpCard()

/*!
*/

void
SoXtPlaneViewer::bottomWheelMotion( // virtual
  float value )
{
  inherited::bottomWheelMotion(
    common->transXWheelMotion( value, this->getBottomWheelValue(),
      this->getCamera() ) );
} // bottomWheelMotion()

/*!
*/

void
SoXtPlaneViewer::leftWheelMotion( // virtual
  float value )
{
  inherited::leftWheelMotion(
    common->transYWheelMotion( value, this->getLeftWheelValue(),
      this->getCamera() ) );
} // leftWheelMotion()

/*!
*/

void
SoXtPlaneViewer::rightWheelMotion( // virtual
  float value )
{
  this->zoom( this->getRightWheelValue() - value );
  inherited::rightWheelMotion( value );
} // rightWheelMotion()

/*!
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

/*!
*/

void
SoXtPlaneViewer::createPrefSheet(
  void )
{
  if ( ! this->prefshell ) {
#if SOXT_DEBUG && 0
    SoDebugError::postInfo( "SoXtExaminerViewer::createPrefSheet",
      "creating preferences window" );
#endif // SOXT_DEBUG
    this->prefparts = new Widget [ 16 ];
    this->createPrefSheetShellAndForm( this->prefshell, this->prefsheet );
    this->createDefaultPrefSheetParts( this->prefparts, this->numprefparts,
      this->prefsheet );
#if SOXT_DEBUG && 0
    SoDebugError::postInfo( "SoXtExaminerViewer::createPrefSheet",
      "numparts = %d", this->numprefparts );
#endif // SOXT_DEBUG
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
  common->viewPlaneX( this->getCamera() );
} // xbutton()

/*!
*/

SOXT_WIDGET_CALLBACK_IMPLEMENTATION(
  SoXtPlaneViewer,
  ybutton )
{
  common->viewPlaneY( this->getCamera() );
} // ybutton()

/*!
*/

SOXT_WIDGET_CALLBACK_IMPLEMENTATION(
  SoXtPlaneViewer,
  zbutton )
{
  common->viewPlaneZ( this->getCamera() );
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
