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

// *************************************************************************

/*!
  \class SoXtExaminerViewer Inventor/Xt/viewers/SoXtExaminerViewer.h
  \brief The SoXtExaminerViewer class is the most used viewer component.
  \ingroup components viewers
*/

// *************************************************************************

#if HAVE_CONFIG_H
#include "config.h"
#endif // HAVE_CONFIG_H

#include <assert.h>
#include <string.h>

#include <X11/cursorfont.h>
#include <X11/keysym.h>

#include <Xm/Form.h>
#include <Xm/Frame.h>
#include <Xm/Label.h>
#include <Xm/LabelG.h>
#include <Xm/Text.h>
#include <Xm/TextF.h>
#include <Xm/PushB.h>
#include <Xm/ToggleB.h>

#include <Inventor/errors/SoDebugError.h>
#include <Inventor/nodes/SoOrthographicCamera.h>
#include <Inventor/nodes/SoPerspectiveCamera.h>
#include <Inventor/projectors/SbSphereSheetProjector.h>
#include <Inventor/projectors/SbSpherePlaneProjector.h>
#include <Inventor/sensors/SoTimerSensor.h>

#include <soxtdefs.h>
#include <Inventor/Xt/SoXt.h>
#include <Inventor/Xt/SoXtBasic.h>
#include <Inventor/Xt/SoXtResource.h>
#include <Inventor/Xt/viewers/SoAnyExaminerViewer.h>
#include <Inventor/Xt/widgets/SoAnyPopupMenu.h>
#include <Inventor/Xt/widgets/SoXtThumbWheel.h>

#include <Inventor/Xt/viewers/SoXtExaminerViewer.h>

#if HAVE_LIBXPM
#include <Inventor/Xt/common/pixmaps/ortho.xpm>
#include <Inventor/Xt/common/pixmaps/perspective.xpm>
#endif // HAVE_LIBXPM

// *************************************************************************

/*!
  \var Widget SoXtExaminerViewer::spinanimtoggle
  Widget for toggling spin animation.
*/

/*!
  \var Widget SoXtExaminerViewer::rotpointaxestoggle
  Widget for toggling rotation point axes graphics.
*/

/*!
  \var Widget SoXtExaminerViewer::rotaxesoverlaytoggle
  Widget for toggling if rotation point axes should be drawn in overlay
  graphics or not.
*/

/*!
  \var Widget SoXtExaminerViewer::axessizewheel
  Widget for wheel controlling axes size.
*/

/*!
  \var Widget SoXtExaminerViewer::axessizefield
  Widget for field for setting axes size.
*/

// *************************************************************************

/*!
  Public constructor.
*/

SoXtExaminerViewer::SoXtExaminerViewer(
  Widget parent,
  const char * name,
  SbBool embed,
  SoXtFullViewer::BuildFlag flag,
  SoXtViewer::Type type )
: inherited( parent, name, embed, flag, type, FALSE )
, common( new SoAnyExaminerViewer( this ) )
{
  this->constructor( TRUE );
} // SoXtExaminerViewer()

/*!
  Protected constructor for derived viewers.
*/

SoXtExaminerViewer::SoXtExaminerViewer( // protected
  Widget parent,
  const char * name,
  SbBool embed,
  SoXtFullViewer::BuildFlag flag,
  SoXtViewer::Type type,
  SbBool build )
: inherited( parent, name, embed, flag, type, FALSE )
, common( new SoAnyExaminerViewer( this ) )
{
  this->constructor( build );
} // SoXtExaminerViewer()

/*!
  Common constructor code.
*/

void
SoXtExaminerViewer::constructor( // private
  const SbBool build )
{
//  this->prefshell = this->prefsheet = (Widget) NULL;
  this->prefparts = NULL;
  this->numprefparts = 0;

  this->mode = EXAMINE;

  this->setClassName( this->getWidgetName() );
  this->camerabutton = (Widget) NULL;

  if ( build ) {
    Widget viewer = this->buildWidget( this->getParentWidget() );
    this->setBaseWidget( viewer );
    this->fitSize( SbVec2s( 500, 300 ) );

    char * dollyString = NULL;
    SoXtResource rsc( this->getRightWheelLabelWidget() );
    if ( rsc.getResource( "dollyString", XmRString, dollyString ) &&
         dollyString != NULL )
      this->setRightWheelString( dollyString );
  }
  this->mapped = FALSE; // ?
} // constructor()

/*!
  The destructor.
*/

SoXtExaminerViewer::~SoXtExaminerViewer(
  void )
{
  delete this->common;
  delete [] this->prefparts;
} // ~SoXtExaminerViewer()

// *************************************************************************

/*!
  Translating events into SoEvent events and working with them makes the
  code more portable, so that is the purpose of this method.

  This method is not part of the original InventorXt API.
*/

SbBool
SoXtExaminerViewer::processSoEvent( // virtual
  const SoEvent * const event )
{
  if ( common->processSoEvent(event) ) return TRUE;
  if ( inherited::processSoEvent(event) ) return TRUE;
  return FALSE;
} // processSoEvent()

/*!
  This method handles X events in the way they should be handled especially
  for examiner viewer components.
*/

void
SoXtExaminerViewer::processEvent(
  XAnyEvent * event )
{
// if ( SoXtViewer::processCommonEvents( event ) )
//   return; // handled in SoXtViewer

  inherited::processEvent( event );
  return;

/*
  SbVec2s canvassize = this->getGLSize();
  mousepos[1] = canvassize[1] - mousepos[1];

  switch ( event->type ) {

  case ButtonPress:
    common->lastmouseposition = norm_mousepos;
    common->lastspinposition = norm_mousepos;

    this->interactiveCountInc();

    if ( this->mode == WAITING_FOR_SEEK ) {
      this->seekToPoint( mousepos );
    }

    break;

  case ButtonRelease:
    if ( ((XButtonEvent *)event)->button != Button1 &&

  case MotionNotify:
    // XPointerMovedEvent
    break;

  case KeyPress:
    {
      unsigned int estate = ((XKeyEvent *)event)->state;
      KeySym keysym = 0;
      char keybuf[8];
      int keybuflen =
        XLookupString( (XKeyEvent *) event, keybuf, 8, &keysym, NULL );

      switch ( keysym ) {
      case XK_Control_L:
      case XK_Control_R:
        estate |= ControlMask;
        break;
      default:
        break;
      } // switch ( keysym )
      this->setModeFromState( estate );
    }
    break;

  case KeyRelease:
    {
      unsigned int estate = ((XKeyEvent *)event)->state;
      KeySym keysym = 0;
      char keybuf[8];
      int keybuflen =
        XLookupString( (XKeyEvent *) event, keybuf, 8, &keysym, NULL );
      switch ( keysym ) {
      case XK_Control_L:
      case XK_Control_R:
        estate &= ~ControlMask;
        break;
      default:
        break;
      } // switch ( keysym )
      this->setModeFromState( estate );
    }
    break;

  default:
    break;

  } // switch ( event )

  common->lastmouseposition = norm_mousepos;
*/
} // processEvent()

// *************************************************************************

/*!
  Stops the model from spin-animating.
*/

void
SoXtExaminerViewer::leftWheelStart( // virtual, protected
  void )
{
  if ( common->isAnimating() )
    common->stopAnimating();
  inherited::leftWheelStart();
} // leftWheelStart()

/*!
  FIXME: write doc
*/

void
SoXtExaminerViewer::leftWheelMotion( // virtual, protected
  float value )
{
  inherited::leftWheelMotion(
    common->rotXWheelMotion( value, this->getLeftWheelValue() ) );
} // leftWheelMotion()

/*!
  Stops the model from spin-animating.
*/

void
SoXtExaminerViewer::bottomWheelStart( // virtual, protected
  void )
{
  if ( common->isAnimating() )
    common->stopAnimating();
  inherited::bottomWheelStart();
} // bottomWheelStart()

/*!
  FIXME: write doc
*/

void
SoXtExaminerViewer::bottomWheelMotion( // virtual, protected
  float value )
{
  inherited::bottomWheelMotion(
    common->rotYWheelMotion( value, this->getBottomWheelValue() ) );
} // bottomWheelMotion()

/*!
  FIXME: write doc
*/

void
SoXtExaminerViewer::rightWheelMotion( // virtual, protected
  float value )
{
  common->zoom( this->getRightWheelValue() - value );
  inherited::rightWheelMotion( value );
} // rightWheelMotion()

// *************************************************************************

/*!
  FIXME: write doc
*/

const char *
SoXtExaminerViewer::getDefaultWidgetName(
  void ) const
{
  static const char defaultWidgetName[] = "SoXtExaminerViewer";
  return defaultWidgetName;
} // getDefaultWidgetName()

/*!
  FIXME: write doc
*/

const char *
SoXtExaminerViewer::getDefaultTitle(
  void ) const
{
  static const char defaultTitle[] = "Examiner Viewer";
  return defaultTitle;
} // getDefaultTitle()

/*!
  FIXME: write doc
*/

const char *
SoXtExaminerViewer::getDefaultIconTitle(
  void ) const
{
  static const char defaultIconTitle[] = "Examiner Viewer";
  return defaultIconTitle;
} // getDefaultIconTitle()

// *************************************************************************

/*!
  FIXME: write doc
*/

void
SoXtExaminerViewer::openViewerHelpCard(
  void )
{
  this->openHelpCard( "SoXtExaminerViewer.help" );
} // openViewerHelpCard()

// *************************************************************************

/*!
  This method overloaded from parent class to make sure the mouse
  pointer cursor is updated.
*/

void
SoXtExaminerViewer::setViewing( // virtual
  SbBool enable )
{
  this->setMode( enable ? EXAMINE : INTERACT );
  inherited::setViewing( enable );
} // setViewing()

// *************************************************************************

/*!
  \internal

  Set cursor graphics according to mode.
*/

void
SoXtExaminerViewer::setCursorRepresentation(const ViewerMode mode)
{
  // FIXME: the cursor handling is just a hack at the moment, but the
  // design layout matches that of SoQtExaminerViewer and is looking
  // ok. What needs to be done is: use pixmaps for the cursor graphics
  // (same gfx as for the other GUI glue libraries), and remember to
  // set up / change the cursor gfx whenever needed (we need to
  // overload setCursorEnabled(), for instance). 20000426 mortene.


  // Don't try to actually set cursor before window has been mapped.
  if ( ! this->mapped ) return;

  if ( ! this->isCursorEnabled() ) {
    // FIXME: set blank bitmap for cursor (or disable it through X11
    // call?). 20000426 mortene.
    return;
  }

  Display * display = this->getDisplay();

  switch ( mode ) {
  case EXAMINE:
  case DRAGGING:
    this->cursor = XCreateFontCursor( display, XC_hand2 );
    XDefineCursor( display, XtWindow( this->glxWidget ), this->cursor );
    break;

  case INTERACT:
  case ZOOMING:
  case WAITING_FOR_SEEK:
  case WAITING_FOR_PAN:
  case PANNING:
    XUndefineCursor( display, XtWindow( this->glxWidget ) );
    break;

  default: SOXT_STUB(); break;
  }

#if 0
  Colormap cmap( DefaultColormap( display, DefaultScreen( display ) ) );
  XColor redcol, whitecol, unused;
  XAllocNamedColor( display, cmap, "red", &redcol, &unused );
  XAllocNamedColor( display, cmap, "white",  &whitecol, &unused );
  XRecolorCursor( display, this->cursor, &redcol, &whitecol );
#endif
} // setCursorRepresentation()

// *************************************************************************

/*!
  \internal
*/

void
SoXtExaminerViewer::setModeFromState(
  const unsigned int state )
{
#if SOXT_DEBUG && 0
  SoDebugError::postInfo( "SoXtExaminerViewer::setModeFromState",
    "state = %08x", state );
#endif // SOXT_DEBUG

  ViewerMode mode = EXAMINE;

  const unsigned int maskedstate =
    state & (Button1Mask | Button2Mask | ControlMask);

  switch ( maskedstate ) {
  case 0:
    mode = EXAMINE;
    break;

  case Button1Mask:
    mode = DRAGGING;
    break;

  case Button2Mask:
  case (Button1Mask | ControlMask):
    mode = PANNING;
    break;

  case ControlMask:
    mode = WAITING_FOR_PAN;
    break;

  case (Button2Mask | ControlMask):
  case (Button1Mask | Button2Mask):
  case (Button1Mask | Button2Mask | ControlMask):
    mode = ZOOMING;
    break;

  default:
    assert(0);
    break;
  }

  this->setMode( mode );
} // setModeFromState()

/*!
  \internal
*/

void
SoXtExaminerViewer::setMode(
  const ViewerMode mode )
{
  switch ( mode ) {

  case INTERACT:
    if ( this->isAnimating() )
      this->stopAnimating();
    while ( this->getInteractiveCount() )
      this->interactiveCountDec();
    break;

  case DRAGGING:
    common->spinprojector->project( common->lastmouseposition );
    break;

  case PANNING:
    do {
      SoCamera * camera = this->getCamera();
      SbViewVolume volume = camera->getViewVolume( this->getGLAspectRatio() );
      common->panningplane = volume.getPlane( camera->focalDistance.getValue() );
    } while ( FALSE );
    break;

  default:
    break;

  } // switch ( mode )

  this->mode = mode;
  this->setCursorRepresentation( mode );
} // setMode()

// *************************************************************************

/*!
  FIXME: write doc
*/

void
SoXtExaminerViewer::createViewerButtons( // virtual
  Widget parent,
  SbPList * buttonlist )
{
  assert( this->camerabutton == (Widget) NULL );

  inherited::createViewerButtons( parent, buttonlist );

  this->camerabutton = XtVaCreateManagedWidget( "C",
    xmPushButtonWidgetClass, parent,
    NULL );

  XtAddCallback( this->camerabutton,
    XmNdisarmCallback, SoXtExaminerViewer::camerabuttonCB, this );

  buttonlist->append( this->camerabutton );

#if HAVE_LIBXPM
  this->camerapixmaps.ortho =
    createPixmapFromXpmData( this->camerabutton, ortho_xpm );
  this->camerapixmaps.ortho_ins =
    createInsensitivePixmapFromXpmData( this->camerabutton, ortho_xpm );
  this->camerapixmaps.perspective =
    createPixmapFromXpmData( this->camerabutton, perspective_xpm );
  this->camerapixmaps.perspective_ins =
    createInsensitivePixmapFromXpmData( this->camerabutton, perspective_xpm );
#endif // HAVE_LIBXPM

} // createViewerButtons()

// *************************************************************************

/*!
  FIXME: write doc
*/

void
SoXtExaminerViewer::camerabuttonClicked(
  void )
{
  this->toggleCameraType();
} // camerabuttonClicked()

// *************************************************************************

/*!
  FIXME: write doc
*/

void
SoXtExaminerViewer::camerabuttonCB( // static
  Widget w,
  XtPointer client_data,
  XtPointer call_data )
{
  SoXtExaminerViewer * viewer = (SoXtExaminerViewer *) client_data;
  viewer->camerabuttonClicked();
} // camerabuttonCB()

// *************************************************************************

/*!
  FIXME: write doc
*/

void
SoXtExaminerViewer::setCamera( // virtual
  SoCamera * camera )
{
#if SOXT_DEBUG && 0
  SoDebugError::postInfo( "SoXtExaminerViewer::setCamera", "[enter]" );
#endif // SOXT_DEBUG
  Pixmap pixmap, pixmap_ins;
  if ( camera == NULL ) {
    // find better pixmaps for this...
    pixmap = this->camerapixmaps.ortho;
    pixmap_ins = this->camerapixmaps.ortho_ins;
  } else if ( camera->isOfType( SoPerspectiveCamera::getClassTypeId() ) ) {
    pixmap = this->camerapixmaps.perspective;
    pixmap_ins = this->camerapixmaps.perspective_ins;
    SoXtResource rsc( this->getRightWheelLabelWidget() );
    char * dollyString = NULL;
    if ( rsc.getResource( "dollyString", XmRString, dollyString ) &&
         dollyString != NULL )
      this->setRightWheelString( dollyString );
  } else if ( camera->isOfType( SoOrthographicCamera::getClassTypeId() ) ) {
    pixmap = this->camerapixmaps.ortho;
    pixmap_ins = this->camerapixmaps.ortho_ins;
    SoXtResource rsc( this->getRightWheelLabelWidget() );
    char * zoomString = NULL;
    if ( rsc.getResource( "zoomString", XmRString, zoomString ) &&
         zoomString != NULL )
      this->setRightWheelString( zoomString );
  } else {
    SoDebugError::postWarning( "SoXtExaminerViewer::setCamera",
      "unknown camera type - got no pixmap" );
    // find better pixmaps for this...
    pixmap = this->camerapixmaps.ortho;
    pixmap_ins = this->camerapixmaps.ortho_ins;
  }

#if HAVE_LIBXPM
  SbBool extra = FALSE;
  if ( XtIsRealized( this->camerabutton ) ) {
    extra = TRUE;
  }
  extra = FALSE;

  if ( extra ) {
    XtUnmapWidget( this->camerabutton );
    XtUnrealizeWidget( this->camerabutton );
  }
  if ( pixmap ) {
    XtVaSetValues( this->camerabutton,
      XmNlabelType, XmPIXMAP,
      XmNlabelPixmap, pixmap,
      XmNselectPixmap, pixmap,
      XmNlabelInsensitivePixmap, pixmap_ins,
      XmNselectInsensitivePixmap, pixmap_ins,
      NULL );
    XtVaSetValues( this->camerabutton,
      XmNwidth, 30,
      XmNheight, 30,
      NULL );
  }
  if ( extra ) {
    XtRealizeWidget( this->camerabutton );
    XtMapWidget( this->camerabutton );
  }
#endif // HAVE_LIBXPM

  inherited::setCamera( camera );
#if SOXT_DEBUG && 0
  SoDebugError::postInfo( "SoXtExaminerViewer::setCamera", "[exit]" );
#endif // SOXT_DEBUG
} // setCamera()

// *************************************************************************

/*!
  FIXME: write doc
*/

void
SoXtExaminerViewer::setSeekMode(
  SbBool enable )
{
#if SOXT_DEBUG
  if ( enable == this->isSeekMode() ) {
    SoDebugError::postWarning( "SoXtExaminerViewer::setSeekMode",
      "seek mode already %sset", enable ? "" : "un" );
    return;
  }
#endif // SOXT_DEBUG

  if ( common->isAnimating() )
    common->stopAnimating();
  inherited::setSeekMode( enable );
  this->setMode( enable ? WAITING_FOR_SEEK : EXAMINE );
} // setSeekMode()

// *************************************************************************

/*!
  This method is overloaded to draw the point-of-rotation axis cross on each
  redraw, if the user has configured it that way.
*/

void
SoXtExaminerViewer::actualRedraw( // virtual
  void )
{
#if SOXT_DEBUG && 0
  SoDebugError::postInfo( "SoXtDebugError::actualRedraw()", "[invoked]" );
#endif // SOXT_DEBUG

  common->actualRedraw();               // spinanimation preparation
  inherited::actualRedraw();            // actual scene rendering
  if ( common->isFeedbackVisible() )    // extra dingbats
    common->drawAxisCross();
  if ( common->isAnimating() )          // animation
    this->scheduleRedraw();
} // actualRedraw()

// *************************************************************************

/*!
  FIXME: write doc
*/

void
SoXtExaminerViewer::setAnimationEnabled(
  const SbBool enable )
{
  common->setAnimationEnabled( enable );
  if ( this->spinanimtoggle ) {
    Boolean enabled = False;
    XtVaGetValues( this->spinanimtoggle,
      XmNset, &enabled,
      NULL );
    if ( enable != enabled )
      XtVaSetValues( this->spinanimtoggle,
        XmNset, enable ? True : False,
        NULL );
  }
} // setAnimationEnabled()

/*!
  FIXME: write doc
*/

SbBool
SoXtExaminerViewer::isAnimationEnabled(
  void ) const
{
  return common->isAnimationEnabled();
} // isAnimationEnabled()

/*!
  FIXME: write doc
*/

void
SoXtExaminerViewer::stopAnimating(
  void )
{
  common->stopAnimating();
} // stopAnimating()

/*!
  FIXME: write doc
*/

SbBool
SoXtExaminerViewer::isAnimating(
  void ) const
{
  return common->isAnimating();
} // isAnimating()

/*!
  FIXME: write doc
*/

void
SoXtExaminerViewer::setFeedbackVisibility(
  const SbBool enable )
{
  common->setFeedbackVisibility( enable );
} // setFeedbackVisibility()

/*!
  FIXME: write doc
*/

SbBool
SoXtExaminerViewer::isFeedbackVisible(
  void ) const
{
  return common->isFeedbackVisible();
} // isFeedbackVisible()

/*!
  FIXME: write doc
*/

void
SoXtExaminerViewer::setFeedbackSize(
  const int size )
{
  if ( this->axessizefield ) {
    char buf[8];
    sprintf( buf, "%d", size );
    XmTextSetString( this->axessizefield, buf );
    XmTextSetCursorPosition( this->axessizefield, (long) strlen(buf) );
  }
  common->setFeedbackSize( size );
} // setFeedbackSize()

/*!
  FIXME: write doc
*/

int
SoXtExaminerViewer::getFeedbackSize(
  void ) const
{
  return common->getFeedbackSize();
} // getFeedbackSize()

// *************************************************************************

/*!
  FIXME: write doc
*/

void
SoXtExaminerViewer::createPrefSheet( // protected, virtual
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
    this->prefparts[this->numprefparts] =
      this->createFramedSpinAnimPrefSheetGuts( this->prefsheet );
    if ( this->prefparts[this->numprefparts] != NULL ) this->numprefparts++;

    this->prefparts[this->numprefparts] =
      this->createFramedRotAxisPrefSheetGuts( this->prefsheet );
    if ( this->prefparts[this->numprefparts] != NULL ) this->numprefparts++;
  }
  this->layoutPartsAndMapPrefSheet( this->prefparts, this->numprefparts,
    this->prefsheet, this->prefshell );
} // createPrefSheet()

// *************************************************************************

/*!
  FIXME: write doc
*/

Widget
SoXtExaminerViewer::createFramedSpinAnimPrefSheetGuts(
  Widget parent )
{
  Widget frame = XtVaCreateManagedWidget( "spinanimframe",
    xmFrameWidgetClass, parent, NULL );

  Widget label = XtVaCreateManagedWidget( "spinanimlabel",
    xmLabelGadgetClass, frame,
    XmNchildType, XmFRAME_TITLE_CHILD,
    XmNchildVerticalAlignment, XmALIGNMENT_CENTER,
    XtVaTypedArg,
      XmNlabelString, XmRString,
      "Spin Animation Settings", strlen( "Spin Animation Settings" ) + 1,
    NULL );

  this->createSpinAnimPrefSheetGuts( frame );

  return frame;
} // createFramedSpinAnimPrefSheetGuts()

/*!
  FIXME: write doc
*/

Widget
SoXtExaminerViewer::createSpinAnimPrefSheetGuts(
  Widget parent )
{
  //  [] enable spin animation
  Widget form = XtVaCreateManagedWidget( "spinanimguts",
    xmFormWidgetClass, parent, NULL );

  this->spinanimtoggle = XtVaCreateManagedWidget( "spinanimtoggle",
    xmToggleButtonWidgetClass, form,
    XmNtopAttachment, XmATTACH_FORM,
    XmNleftAttachment, XmATTACH_FORM,
    XmNbottomAttachment, XmATTACH_FORM,
    XtVaTypedArg,
      XmNlabelString, XmRString,
      "enable spin animation", strlen( "enable spin animation" ) + 1,
    XmNset, common->isAnimationEnabled(),
    NULL );

  XtAddCallback( this->spinanimtoggle, XmNvalueChangedCallback,
    SoXtExaminerViewer::spinanimtoggledCB, (XtPointer) this );

  return form;
} // createSpinAnimPrefSheetGuts()

/*!
  FIXME: write doc
*/

void
SoXtExaminerViewer::spinanimtoggled(
  void )
{
  Boolean enable = False;
  XtVaGetValues( this->spinanimtoggle, XmNset, &enable, NULL );
  this->setAnimationEnabled( enable ? TRUE : FALSE );
  if ( ! enable && this->isAnimating() )
    this->stopAnimating();
} // spinanimtoggled()

/*!
  static callback
*/

void
SoXtExaminerViewer::spinanimtoggledCB( // static
  Widget,
  XtPointer closure,
  XtPointer )
{
  SoXtExaminerViewer * viewer = (SoXtExaminerViewer *) closure;
  viewer->spinanimtoggled();
} // spinanimtoggledCB()

// *************************************************************************

/*!
  This method is a wrapper for createRotAxisPrefSheetGuts() to include a
  titled frame around the widgets.
*/

Widget
SoXtExaminerViewer::createFramedRotAxisPrefSheetGuts(
  Widget parent )
{
  Widget frame = XtVaCreateManagedWidget( "rotaxisframe",
    xmFrameWidgetClass, parent, NULL );

  Widget label = XtVaCreateManagedWidget( "rotaxislabel",
    xmLabelGadgetClass, frame,
    XmNchildType, XmFRAME_TITLE_CHILD,
    XmNchildVerticalAlignment, XmALIGNMENT_CENTER,
    XtVaTypedArg,
      XmNlabelString, XmRString,
      "Rotation Point Axes Settings", strlen( "Rotation Point Axes Settings" ) + 1,
    NULL );

  this->createRotAxisPrefSheetGuts( frame );

  return frame;
} // createFramedRotAxisPrefSheetGuts()

/*!
  FIXME: write doc
*/

Widget
SoXtExaminerViewer::createRotAxisPrefSheetGuts(
  Widget parent )
{
  //  [] display point of rotation
  //  axes size   |||||||||||||||||||||||||  |NN| pixels

  Widget form = XtVaCreateManagedWidget( "rotaxisguts",
    xmFormWidgetClass, parent, NULL );

#if 0

  this->rotpointaxestoggle = XtVaCreateManagedWidget( "rotpointaxestoggle",
    xmToggleButtonWidgetClass, form,
    XmNtopAttachment, XmATTACH_FORM,
    XmNleftAttachment, XmATTACH_FORM,
    XtVaTypedArg,
      XmNlabelString, XmRString,
      "display rotation point", strlen( "display rotation point" ) + 1,
    XmNset, this->isFeedbackVisible(),
    NULL );

  XtAddCallback( this->rotpointaxestoggle, XmNvalueChangedCallback,
    SoXtExaminerViewer::rotpointtoggledCB, (XtPointer) this );

  this->rotaxesoverlaytoggle = XtVaCreateManagedWidget( "rotaxesoverlaytoggle",
    xmToggleButtonWidgetClass, form,
    XmNtopAttachment, XmATTACH_FORM,
    XmNrightAttachment, XmATTACH_FORM,
    XtVaTypedArg,
      XmNlabelString, XmRString,
      "overlay graphics", strlen( "overlay graphics" ) + 1,
    XmNset, True,
    NULL );

  XtAddCallback( this->rotaxesoverlaytoggle, XmNvalueChangedCallback,
    SoXtExaminerViewer::rotaxesoverlaytoggledCB, (XtPointer) this );

  Widget pixelslabel = XtVaCreateWidget( "pixelslabel",
    xmLabelWidgetClass, form,
    XtVaTypedArg,
      XmNlabelString, XmRString,
      "pixels", strlen( "pixels" ) + 1,
    NULL );

  Dimension width;
  XtVaGetValues( pixelslabel, XmNwidth, &width, NULL );
  this->axessizefield = XtVaCreateManagedWidget( "axessizefield",
    xmTextFieldWidgetClass, form,
    XmNtopAttachment, XmATTACH_WIDGET,
    XmNtopWidget, this->rotpointaxestoggle,
    XmNrightAttachment, XmATTACH_FORM,
    XmNrightOffset, width + 5,
    XmNwidth, 40,
    XmNsensitive, this->isFeedbackVisible() ? True : False,
    XmNeditable, this->isFeedbackVisible() ? True : False,
    XmNcursorPositionVisible, this->isFeedbackVisible() ? True : False,
    NULL );

  XmTextSetMaxLength( this->axessizefield, 3 );
  char buffer[16];
  sprintf( buffer, "%d", this->getFeedbackSize() );
  XmTextSetString( this->axessizefield, buffer );
  XmTextSetCursorPosition( this->axessizefield, (long) strlen(buffer) );

  XtAddCallback( this->axessizefield, XmNactivateCallback,
    SoXtExaminerViewer::axesfieldchangedCB, (XtPointer) this );
  XtAddCallback( this->axessizefield, XmNlosingFocusCallback,
    SoXtExaminerViewer::axesfieldchangedCB, (XtPointer) this );

  XtVaSetValues( pixelslabel,
    XmNrightAttachment, XmATTACH_FORM,
    XmNrightOffset, 2,
    XmNtopAttachment, XmATTACH_OPPOSITE_WIDGET,
    XmNtopWidget, this->axessizefield,
    XmNbottomAttachment, XmATTACH_OPPOSITE_WIDGET,
    XmNbottomWidget, this->axessizefield,
    NULL );
  XtManageChild( pixelslabel );

  this->axessizewheel = XtVaCreateManagedWidget( "axeswheel",
    soxtThumbWheelWidgetClass, form,
    XmNtopAttachment, XmATTACH_OPPOSITE_WIDGET,
    XmNtopWidget, this->axessizefield,
    XmNtopOffset, 2,
    XmNrightAttachment, XmATTACH_WIDGET,
    XmNrightWidget, this->axessizefield,
    XmNrightOffset, 2,
    XmNheight, 26,
    XmNwidth, 90,
    XmNorientation, XmHORIZONTAL,
    XmNsensitive, this->isFeedbackVisible() ? True : False,
    NULL );
  XtAddCallback( this->axessizewheel, XmNvalueChangedCallback,
    SoXtExaminerViewer::axeswheelmovedCB, (XtPointer) this );

  Widget label = XtVaCreateManagedWidget( "axeslabel",
    xmLabelWidgetClass, form,
    XmNtopAttachment, XmATTACH_OPPOSITE_WIDGET,
    XmNtopWidget, this->axessizewheel,
    XmNrightAttachment, XmATTACH_WIDGET,
    XmNrightWidget, this->axessizewheel,
    XmNrightOffset, 2,
    XmNbottomAttachment, XmATTACH_OPPOSITE_WIDGET,
    XmNbottomWidget, this->axessizewheel,
    XtVaTypedArg,
      XmNlabelString, XmRString,
      "axes size", strlen( "axes size" ) + 1,
    NULL );

#endif // 0

  return form;
} // createRotAxisPrefSheetGuts()

/*!
  FIXME: write doc
*/

void
SoXtExaminerViewer::rotpointtoggled(
  void )
{
  Boolean enable = False;
  XtVaGetValues( this->rotpointaxestoggle, XmNset, &enable, NULL );

  XtVaSetValues( this->axessizewheel,
    XmNsensitive, enable,
    NULL );
  XtVaSetValues( this->axessizefield,
    XmNsensitive, enable,
    XmNeditable, enable,
    XmNcursorPositionVisible, enable,
    NULL );

  this->setFeedbackVisibility( enable ? TRUE : FALSE );
} // rotpointtoggled()

/*!
  static callback
*/

void
SoXtExaminerViewer::rotpointtoggledCB(
  Widget,
  XtPointer closure,
  XtPointer )
{
  SoXtExaminerViewer * viewer = (SoXtExaminerViewer *) closure;
  viewer->rotpointtoggled();
} // rotpointtoggledCB()

/*!
  FIXME: write doc
*/

void
SoXtExaminerViewer::axeswheelmoved(
  int ticks )
{
  int size = this->getFeedbackSize() + ticks;
  if ( size < 3 )
    size = 3;
  else if ( size > 200 )
    size = 200;
  this->setFeedbackSize( size );
} // axeswheelmoved()

/*!
  static callback
*/

void
SoXtExaminerViewer::axeswheelmovedCB(
  Widget,
  XtPointer closure,
  XtPointer call_data )
{
  SoXtExaminerViewer * viewer = (SoXtExaminerViewer *) closure;
  SoXtThumbWheelCallbackData * data = (SoXtThumbWheelCallbackData *) call_data;
  viewer->axeswheelmoved( data->ticks );
} // axeswheelmovedCB()

/*!
  FIXME: write doc
*/

void
SoXtExaminerViewer::axesfieldchanged(
  void )
{
  int size = atoi( XmTextGetString( this->axessizefield ) );
  if ( size < 3 )
    size = 3;
  else if ( size > 200 )
    size = 200;
  this->setFeedbackSize( size );
} // axesfieldchanged()

/*!
  static callback
*/

void
SoXtExaminerViewer::axesfieldchangedCB(
  Widget,
  XtPointer closure,
  XtPointer )
{
  SoXtExaminerViewer * viewer = (SoXtExaminerViewer *) closure;
  viewer->axesfieldchanged();
} // axesfieldchangedCB()

/*!
  This callback should toggle wether the point of rotation should be embedded
  in the model Z-buffer wise, or be printed as overlay data.
*/

void
SoXtExaminerViewer::rotaxesoverlaytoggled(
  void )
{
#if SOXT_DEBUG
  SOXT_STUB_ONCE();
#endif // SOXT_DEBUG
  XtVaSetValues( this->rotaxesoverlaytoggle, XmNset, True, NULL );
} // rotpointoverlaytoggled()

/*!
  static callback
*/

void
SoXtExaminerViewer::rotaxesoverlaytoggledCB( // static
  Widget,
  XtPointer closure,
  XtPointer )
{
  SoXtExaminerViewer * viewer = (SoXtExaminerViewer *) closure;
  viewer->rotaxesoverlaytoggled();
} // rotaxesoverlaytoggledCB()

/*!
  Invoked when first mapped.
  Just makes sure the cursor gets set for the GL area.
*/

void
SoXtExaminerViewer::afterRealizeHook( // virtual, protected
  void )
{
#if SOXT_DEBUG && 0
  SoDebugError::postInfo( "SoXtExaminerViewer::afterRealizeHook",
    "[invoked]" );
#endif // SOXT_DEBUG
  inherited::afterRealizeHook();
  this->mapped = TRUE;
//  this->setCursorRepresentation( this->mode );
} // afterRealizeHook()

// *************************************************************************

#if SOXT_DEBUG
static const char * getSoXtExaminerViewerRCSId(void) { return rcsid; }
#endif // SOXT_DEBUG

