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

#if HAVE_CONFIG_H
#include "config.h"
#endif // HAVE_CONFIG_H

#include <assert.h>
#include <string.h>

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
#include <Inventor/sensors/SoTimerSensor.h>

#include <Inventor/Xt/SoXt.h>
#include <Inventor/Xt/SoXtBasic.h>

#include <Inventor/Xt/viewers/SoAnyExaminerViewer.h>
#include <Inventor/Xt/viewers/SoXtExaminerViewer.h>

#include <Inventor/Xt/widgets/SoAnyPopupMenu.h>
#include <Inventor/Xt/widgets/SoXtThumbWheel.h>

#if HAVE_LIBXPM
#include <Inventor/Xt/common/pixmaps/ortho.xpm>
#include <Inventor/Xt/common/pixmaps/perspective.xpm>
#endif // HAVE_LIBXPM

#include <X11/cursorfont.h>
#include <X11/keysym.h>

// *************************************************************************

/*!
  Public constructor.
*/

SoXtExaminerViewer::SoXtExaminerViewer(
  Widget parent,
  const char * name,
  SbBool inParent,
  SoXtFullViewer::BuildFlag flag,
  SoXtViewer::Type type )
: inherited( parent, name, inParent, flag, type, FALSE )
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
  SbBool inParent,
  SoXtFullViewer::BuildFlag flag,
  SoXtViewer::Type type,
  SbBool build )
: inherited( parent, name, inParent, flag, type, FALSE )
, common( new SoAnyExaminerViewer( this ) )
{
  this->constructor( build );
} // SoXtExaminerViewer()

/*!
  Common constructor code.
*/

void
SoXtExaminerViewer::constructor( // private
  SbBool build )
{
  this->prefshell = this->prefsheet = (Widget) NULL;
  this->prefparts = NULL;
  this->numprefparts = 0;

// don't uncomment this until constructor sends FALSE to FullViewer class
  this->mode = EXAMINE;

  this->spindetecttimerId = 0;
  this->spindetecttimerActive = FALSE;

  this->setClassName( "SoXtExaminerViewer" );
  this->setTitle( "Examiner Viewer" );
  this->camerabutton = (Widget) NULL;
  this->setLeftWheelString( "Rot Y" );
  this->setBottomWheelString( "Rot X" );
  this->setRightWheelString( "Dolly" );

  if ( build ) {
    Widget viewer = this->buildWidget( this->getParentWidget() );
    this->setBaseWidget( viewer );
  }

  this->mapped = FALSE;
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
*/

void
SoXtExaminerViewer::processEvent(
  XAnyEvent * event )
{
  if ( SoXtViewer::processCommonEvents( event ) )
    return; // handled in SoXtViewer

  if ( ! this->mapped ) {
    this->mapped = TRUE; // Must be set before setCursorRepresentation() call.
    this->setCursorRepresentation( this->mode );
  }

  SbVec2s canvassize = this->getGLSize();
  SbVec2s mousepos( 0, 0 );

  switch ( event->type ) {
  case ButtonPress:
  case ButtonRelease:
    mousepos[0] = ((XButtonEvent *)event)->x;
    mousepos[1] = ((XButtonEvent *)event)->y;
    break;
    
  case MotionNotify:
    mousepos[0] = ((XPointerMovedEvent *)event)->x;
    mousepos[1] = ((XPointerMovedEvent *)event)->y;
    break;

  default:
    break;
  }

  mousepos[1] = canvassize[1] - mousepos[1];

  SbVec2f norm_mousepos( (float)mousepos[0]/(float)canvassize[0],
                         (float)mousepos[1]/(float)canvassize[1] );

  switch ( event->type ) {

  case ButtonPress:
    common->lastmouseposition = norm_mousepos;
    common->lastspinposition = norm_mousepos;

    if ( ((XButtonEvent *) event)->button == 3 && this->isPopupMenuEnabled() ) {
      int x = ((XButtonEvent *) event)->x_root;
      int y = ((XButtonEvent *) event)->y_root;
      if ( ! this->prefmenu )
        this->buildPopupMenu();
      this->prefmenu->PopUp( this->getGLWidget(), x, y );
      break;
    }

    if ( ((XButtonEvent *) event)->button == Button4 ) {
      common->zoom( 0.1f );
      break;
    }
      
    if ( ((XButtonEvent *) event)->button == Button5 ) {
      common->zoom( -0.1f );
      break;
    }

    this->interactiveCountInc();

    if ( this->mode == WAITING_FOR_SEEK ) {
      this->seekToPoint( mousepos );
    } else {
      do {
        unsigned int state = ((XButtonEvent *)event)->state;
        switch ( ((XButtonEvent *)event)->button ) {
        case Button1: state |= Button1Mask; break;
        case Button2: state |= Button2Mask; break;
        case Button3: state |= Button3Mask; break;
        case Button4: state |= Button4Mask; break;
        case Button5: state |= Button5Mask; break;
        default: break;
        }
        this->setModeFromState( state );
        if ( this->isAnimating() )
          this->stopAnimating();
      } while ( FALSE );
    }

    break;

  case ButtonRelease:
    if ( ((XButtonEvent *)event)->button != Button1 &&
         ((XButtonEvent *)event)->button != Button2 )
      break;

    if ( this->mode == DRAGGING &&
         this->isAnimationEnabled() &&
         this->spindetecttimerActive ) {
      XtRemoveTimeOut( this->spindetecttimerId );
      this->spindetecttimerId = 0;
      this->spindetecttimerActive = FALSE;
      common->spinanimating = TRUE;
      common->spintimertrigger->schedule();
      this->interactiveCountInc();
    }

    this->interactiveCountDec();
    do {
      unsigned int state = ((XButtonEvent *)event)->state;
      switch ( ((XButtonEvent *)event)->button ) {
      case Button1: state &= ~Button1Mask; break;
      case Button2: state &= ~Button2Mask; break;
      case Button3: state &= ~Button3Mask; break;
      case Button4: state &= ~Button4Mask; break;
      case Button5: state &= ~Button5Mask; break;
      default: break;
      }
      this->setModeFromState( state );
    } while ( FALSE );
    break;

  case MotionNotify:
    // XPointerMovedEvent
    switch ( this->mode ) {
    case DRAGGING:
      if ( this->spindetecttimerId )
        XtRemoveTimeOut( this->spindetecttimerId );
      this->spindetecttimerId =
        XtAppAddTimeOut( SoXt::getAppContext(), 10,
          SoXtExaminerViewer::spindetecttimerCB, this );
        this->spindetecttimerActive = TRUE;
        common->spin( norm_mousepos );
      break;

    case PANNING:
      common->pan( norm_mousepos );
      break;

    case ZOOMING:
      common->zoomByCursor( norm_mousepos );
      break;

    default:
      break;
    }
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
*/

void
SoXtExaminerViewer::bottomWheelMotion( // virtual, protected
  float value )
{
  inherited::bottomWheelMotion(
    common->rotYWheelMotion( value, this->getBottomWheelValue() ) );
} // bottomWheelMotion()

/*!
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
*/

const char *
SoXtExaminerViewer::getDefaultWidgetName(
  void ) const
{
  static const char defaultWidgetName[] = "SoXtExaminerViewer";
  return defaultWidgetName;
} // getDefaultWidgetName()

/*!
*/

const char *
SoXtExaminerViewer::getDefaultTitle(
  void ) const
{
  static const char defaultTitle[] = "Examiner Viewer";
  return defaultTitle;
} // getDefaultTitle()

/*!
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
  \internal
*/

void
SoXtExaminerViewer::spindetecttimerCB( // static
  XtPointer user,
  XtIntervalId * id )
{
  SoXtExaminerViewer * that = (SoXtExaminerViewer *) user;
  that->spindetecttimerId = 0;
  that->spindetecttimerActive = FALSE;
#if SOXT_DEBUG && 0
  SoDebugError::postInfo( "SoXtExaminerViewer::spindetecttimerCB",
    "called - but doin' squat" );
#endif // SOXT_DEBUG
} // spindetecttimerCB()

// *************************************************************************

/*!
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
    XmNshadowType, XmSHADOW_OUT,
    XmNhighlightThickness, 0,
    XmNshadowThickness, 2,
    XmNtraversalOn, False,
    XmNwidth, 30,
    XmNheight, 30,
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
*/

void
SoXtExaminerViewer::camerabuttonClicked(
  void )
{
  this->toggleCameraType();

/*
  SoCamera * const camera = this->getCamera();
  if ( ! camera ) {
#if SOXT_DEBUG
    SoDebugError::postWarning( "SoXtExaminerViewer::camerabuttonClicked",
      "no camera" );
#endif // SOXT_DEBUG
    return;
  }

  if ( camera->isOfType( SoPerspectiveCamera::getClassTypeId() ) ) {
    this->setCamera( new SoOrthographicCamera );
  } else if ( camera->isOfType( SoPerspectiveCamera::getClassTypeId() ) ) {
    this->setCamera( new SoPerspectiveCamera );
  } else {
#if SOXT_DEBUG
    SoDebugError::postWarning( "SoXtExaminerViewer::camerabuttonClicked",
      "unknown camera - got no pixmap" );
#endif // SOXT_DEBUG
  }

*/

} // camerabuttonClicked()

// *************************************************************************

/*!
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
*/

void
SoXtExaminerViewer::setCamera( // virtual
  SoCamera * camera )
{
  Pixmap pixmap, pixmap_ins;
  if ( camera == NULL ) {
    // find better pixmaps for this...
    pixmap = this->camerapixmaps.ortho;
    pixmap_ins = this->camerapixmaps.ortho_ins;
  } else if ( camera->isOfType( SoPerspectiveCamera::getClassTypeId() ) ) {
    pixmap = this->camerapixmaps.perspective;
    pixmap_ins = this->camerapixmaps.perspective_ins;
    this->setRightWheelString( "Dolly" );
  } else if ( camera->isOfType( SoOrthographicCamera::getClassTypeId() ) ) {
    pixmap = this->camerapixmaps.ortho;
    pixmap_ins = this->camerapixmaps.ortho_ins;
    this->setRightWheelString( "Zoom" );
  } else {
    SoDebugError::postWarning( "SoXtExaminerViewer::setCamera",
      "unknown camera type - got no pixmap" );
    // find better pixmaps for this...
    pixmap = this->camerapixmaps.ortho;
    pixmap_ins = this->camerapixmaps.ortho_ins;
  }

  XtUnmanageChild( this->camerabutton );
#if HAVE_LIBXPM
  XtVaSetValues( this->camerabutton,
    XmNlabelType, XmPIXMAP,
    XmNlabelPixmap, pixmap,
    XmNselectPixmap, pixmap,
    XmNlabelInsensitivePixmap, pixmap_ins,
    XmNselectInsensitivePixmap, pixmap_ins,
    NULL );
#endif // HAVE_LIBXPM
  XtVaSetValues( this->camerabutton,
    XmNwidth, 30, XmNheight, 30, NULL );
  XtManageChild( this->camerabutton );

  inherited::setCamera( camera );
} // setCamera()

// *************************************************************************

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
  This method is overloaded to draw the poin-of-rotation axis cross on
  each redraw, if user-configured.
*/

void
SoXtExaminerViewer::actualRedraw( // virtual
  void )
{
  inherited::actualRedraw();
  if ( this->isFeedbackVisible() )
    common->drawAxisCross();
} // actualRedraw()

// *************************************************************************

void SoXtExaminerViewer::setAnimationEnabled( const SbBool enable ) {
  common->setAnimationEnabled( enable );
  if ( this->spinanimtoggle ) {
    Boolean enabled = False;
    XtVaGetValues( this->spinanimtoggle, XmNset, &enabled, NULL );
    if ( enable != enabled )
      XtVaSetValues( this->spinanimtoggle,
        XmNset, enable ? True : False,
        NULL );
  }
} // setAnimationEnabled()

SbBool SoXtExaminerViewer::isAnimationEnabled(void) const {
  return common->isAnimationEnabled();
} // isAnimationEnabled()

void SoXtExaminerViewer::stopAnimating(void) {
  common->stopAnimating();
} // stopAnimating()

SbBool SoXtExaminerViewer::isAnimating(void) const {
  return common->isAnimating();
} // isAnimating()

void SoXtExaminerViewer::setFeedbackVisibility( const SbBool enable ) {
  common->setFeedbackVisibility( enable );
} // setFeedbackVisibility()

SbBool SoXtExaminerViewer::isFeedbackVisible(void) const {
  return common->isFeedbackVisible();
} // isFeedbackVisible()

void SoXtExaminerViewer::setFeedbackSize(
  const int size )
{
  if ( this->axessizefield ) {
    char buf[8];
    sprintf( buf, "%d", size );
    XmTextSetString( this->axessizefield, buf );
    XmTextSetCursorPosition( this->axessizefield, strlen(buf) );
  }
  common->setFeedbackSize( size );
} // setFeedbackSize()

int SoXtExaminerViewer::getFeedbackSize(void) const {
  return common->getFeedbackSize();
} // getFeedbackSize()

// *************************************************************************

/*!
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
*/

Widget
SoXtExaminerViewer::createFramedSpinAnimPrefSheetGuts(
  Widget parent )
{
  Widget frame = XtVaCreateManagedWidget( "spinanimframe",
    xmFrameWidgetClass, parent, NULL );

  XmString labelstring = SoXt::encodeString( "Spin Animation Settings" );
  Widget label = XtVaCreateManagedWidget( "spinanimlabel",
    xmLabelGadgetClass, frame,
    XmNchildType, XmFRAME_TITLE_CHILD,
    XmNchildVerticalAlignment, XmALIGNMENT_CENTER,
    XmNlabelString, labelstring,
    NULL );
  XtFree( (char *) labelstring );

  this->createSpinAnimPrefSheetGuts( frame );

  return frame;
} // createFramedSpinAnimPrefSheetGuts()

/*!
*/

Widget
SoXtExaminerViewer::createSpinAnimPrefSheetGuts(
  Widget parent )
{
  //  [] enable spin animation
  Widget form = XtVaCreateManagedWidget( "spinanimguts",
    xmFormWidgetClass, parent, NULL );

  XmString labelstring = SoXt::encodeString( "enable spin animation" );
  this->spinanimtoggle = XtVaCreateManagedWidget( "spinanimtoggle",
    xmToggleButtonWidgetClass, form,
    XmNtopAttachment, XmATTACH_FORM,
    XmNleftAttachment, XmATTACH_FORM,
    XmNbottomAttachment, XmATTACH_FORM,
    XmNlabelString, labelstring,
    XmNset, common->isAnimationEnabled(),
    NULL );
  XtFree( (char *) labelstring );

  XtAddCallback( this->spinanimtoggle, XmNvalueChangedCallback,
    SoXtExaminerViewer::spinanimtoggledCB, (XtPointer) this );

  return form;
} // createSpinAnimPrefSheetGuts()

/*!
*/

SOXT_WIDGET_CALLBACK_IMPLEMENTATION(
  SoXtExaminerViewer,
  spinanimtoggled )
{
  Boolean enable = False;
  XtVaGetValues( this->spinanimtoggle, XmNset, &enable, NULL );
  this->setAnimationEnabled( enable ? TRUE : FALSE );
  if ( ! enable && this->isAnimating() )
    this->stopAnimating();
} // spinanimtoggled()

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

  XmString labelstring = SoXt::encodeString( "Rotation Point Axes Settings" );
  Widget label = XtVaCreateManagedWidget( "rotaxislabel",
    xmLabelGadgetClass, frame,
    XmNchildType, XmFRAME_TITLE_CHILD,
    XmNchildVerticalAlignment, XmALIGNMENT_CENTER,
    XmNlabelString, labelstring,
    NULL );
  XtFree( (char *) labelstring );

  this->createRotAxisPrefSheetGuts( frame );

  return frame;
} // createFramedRotAxisPrefSheetGuts()

/*!
*/

Widget
SoXtExaminerViewer::createRotAxisPrefSheetGuts(
  Widget parent )
{
  //  [] display point of rotation
  //  axes size   |||||||||||||||||||||||||  |NN| pixels

  Widget form = XtVaCreateManagedWidget( "rotaxisguts",
    xmFormWidgetClass, parent, NULL );

  XmString labelstring = SoXt::encodeString( "display rotation point" );
  this->rotpointaxestoggle = XtVaCreateManagedWidget( "rotpointaxestoggle",
    xmToggleButtonWidgetClass, form,
    XmNtopAttachment, XmATTACH_FORM,
    XmNleftAttachment, XmATTACH_FORM,
    XmNlabelString, labelstring,
    XmNset, this->isFeedbackVisible(),
    NULL );
  XtFree( (char *) labelstring );
  XtAddCallback( this->rotpointaxestoggle, XmNvalueChangedCallback,
    SoXtExaminerViewer::rotpointtoggledCB, (XtPointer) this );

  labelstring = SoXt::encodeString( "overlay graphics" );
  this->rotaxesoverlaytoggle = XtVaCreateManagedWidget( "rotaxesoverlaytoggle",
    xmToggleButtonWidgetClass, form,
    XmNtopAttachment, XmATTACH_FORM,
    XmNrightAttachment, XmATTACH_FORM,
    XmNlabelString, labelstring,
    XmNset, True,
    NULL );
  XtFree( (char *) labelstring );
  XtAddCallback( this->rotaxesoverlaytoggle, XmNvalueChangedCallback,
    SoXtExaminerViewer::rotaxesoverlaytoggledCB, (XtPointer) this );

  labelstring = SoXt::encodeString( "pixels" );
  Widget pixelslabel = XtVaCreateWidget( "pixelslabel",
    xmLabelWidgetClass, form,
    XmNlabelString, labelstring,
    NULL );
  XtFree( (char *) labelstring );

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
  XmTextSetCursorPosition( this->axessizefield, strlen(buffer) );

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

  labelstring = SoXt::encodeString( "axes size" );
  Widget label = XtVaCreateManagedWidget( "axeslabel",
    xmLabelWidgetClass, form,
    XmNtopAttachment, XmATTACH_OPPOSITE_WIDGET,
    XmNtopWidget, this->axessizewheel,
    XmNrightAttachment, XmATTACH_WIDGET,
    XmNrightWidget, this->axessizewheel,
    XmNrightOffset, 2,
    XmNbottomAttachment, XmATTACH_OPPOSITE_WIDGET,
    XmNbottomWidget, this->axessizewheel,
    XmNlabelString, labelstring,
    NULL );
  XtFree( (char *) labelstring );

  return form;
} // createRotAxisPrefSheetGuts()

/*!
*/

SOXT_WIDGET_CALLBACK_IMPLEMENTATION(
  SoXtExaminerViewer,
  rotpointtoggled )
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
*/

SOXT_WIDGET_CALLBACK_IMPLEMENTATION(
  SoXtExaminerViewer,
  axeswheelmoved )
{
  SoXtThumbWheelCallbackData * data = (SoXtThumbWheelCallbackData *) call_data;

  int size = this->getFeedbackSize() + data->ticks;
  if ( size < 3 )
    size = 3;
  else if ( size > 200 )
    size = 200;
  this->setFeedbackSize( size );
} // axeswheelmoved()

/*!
*/

SOXT_WIDGET_CALLBACK_IMPLEMENTATION(
  SoXtExaminerViewer,
  axesfieldchanged )
{
  int size = atoi( XmTextGetString( this->axessizefield ) );
  if ( size < 3 )
    size = 3;
  else if ( size > 200 )
    size = 200;
  this->setFeedbackSize( size );
} // axesfieldchanged()

/*!
  This callback should toggle wether the point of rotation should be embedded
  in the model Z-buffer wise, or be printed as overlay data.
*/

SOXT_WIDGET_CALLBACK_IMPLEMENTATION(
  SoXtExaminerViewer,
  rotaxesoverlaytoggled )
{
#if SOXT_DEBUG
  SOXT_STUB_ONCE();
#endif // SOXT_DEBUG
  XtVaSetValues( this->rotaxesoverlaytoggle, XmNset, True, NULL );
} // rotpointoverlaytoggled()

// *************************************************************************
