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

#include <Inventor/errors/SoDebugError.h>
#include <Inventor/nodes/SoOrthographicCamera.h>
#include <Inventor/nodes/SoPerspectiveCamera.h>
#include <Inventor/projectors/SbSphereSheetProjector.h>
#include <Inventor/sensors/SoTimerSensor.h>

#include <Inventor/Xt/SoXt.h>
#include <Inventor/Xt/common/SoAnyExaminerViewer.h>
#include <Inventor/Xt/viewers/SoXtExaminerViewer.h>

// *************************************************************************

SoXtExaminerViewer::SoXtExaminerViewer(
  Widget parent,
  const char * name,
  SbBool inParent,
  SoXtFullViewer::BuildFlag flag,
  SoXtViewer::Type type )
: inherited( parent, name, inParent, flag, type, FALSE )
{
  this->constructor( TRUE );
} // SoXtExaminerViewer()

SoXtExaminerViewer::SoXtExaminerViewer( // protected
  Widget parent,
  const char * name,
  SbBool inParent,
  SoXtFullViewer::BuildFlag flag,
  SoXtViewer::Type type,
  SbBool build )
: inherited( parent, name, inParent, flag, type, FALSE )
{
  this->constructor( build );
} // SoXtExaminerViewer()

void
SoXtExaminerViewer::constructor( // private
  SbBool build )
{
// don't uncomment this until constructor sends FALSE to FullViewer class

  this->projector = new SbSphereSheetProjector;
  SbViewVolume volume;
  volume.ortho( -1, 1, -1, 1, -1, 1 );
  this->projector->setViewVolume( volume );

  this->mode = EXAMINE;

  this->animatingallowed = TRUE;
  this->spinanimating = FALSE;
  this->spindetecttimerId = 0;
  this->spindetecttimerActive = FALSE;
  this->spinsamplecounter = 0;
  this->spinincrement = SbRotation::identity();
  this->timertrigger = new SoTimerSensor( SoXtExaminerViewer::timertriggerCB, this );
  this->timertrigger->setInterval( 1.0f/30.0f );

  this->setClassName( "SoXtExaminerViewer" );

  if ( build ) {
    Widget viewer = this->buildWidget( this->getParentWidget() );
    this->setBaseWidget( viewer );
  }
} // constructor()

/*!
  The destructor.
*/

SoXtExaminerViewer::~SoXtExaminerViewer(
  void )
{
} // ~SoXtExaminerViewer()

// *************************************************************************

void
SoXtExaminerViewer::processEvent(
  XAnyEvent * event )
{
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
    if ( ((XButtonEvent *) event)->button == Button3 )
      break;

    if ( ((XButtonEvent *) event)->button == Button4 ) {
      this->zoom( 0.1f );
      break;
    }
      
    if ( ((XButtonEvent *) event)->button == Button5 ) {
      this->zoom( -0.1f );
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
         this->animatingallowed &&
         this->spindetecttimerActive ) {
      XtRemoveTimeOut( this->spindetecttimerId );
      this->spindetecttimerId = 0;
      this->spindetecttimerActive = FALSE;
      this->spinanimating = TRUE;
      this->timertrigger->schedule();
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
        this->spin( norm_mousepos );
      break;

    case PANNING:
      this->pan( norm_mousepos );
      break;

    case ZOOMING:
      this->zoomByCursor( norm_mousepos );
      break;

    default:
      break;
    }
    break;

  case KeyPress:
    switch ( ((XKeyPressedEvent *) event)->keycode ) {
    default:
      break;
    } // switch ( ((XKeyPressedEvent *) event)->keycode )
    break;

  case KeyRelease:
    switch ( ((XKeyReleasedEvent *) event)->keycode ) {
    default:
      break;
    } // switch ( ((XKeyReleasedEvent *) event)->keycode )
    break;

  default:
    break;

  } // switch ( event )

  this->lastmouseposition = norm_mousepos;

  inherited::processEvent( event );
} // processEvent()

// *************************************************************************

/*!
*/

void
SoXtExaminerViewer::leftWheelMotion( // virtual, protected
  float value )
{
  if ( this->isAnimating() )
    this->stopAnimating();
  inherited::leftWheelMotion(
    SoAnyExaminerViewer::rotXWheelMotion( value, this->getLeftWheelValue(),
                                          this->getCamera() ) );
} // leftWheelMotion()

/*!
*/

void
SoXtExaminerViewer::bottomWheelMotion( // virtual, protected
  float value )
{
  if ( this->isAnimating() )
    this->stopAnimating();
  inherited::bottomWheelMotion(
    SoAnyExaminerViewer::rotYWheelMotion( value, this->getLeftWheelValue(),
                                          this->getCamera() ) );
} // bottomWheelMotion()

/*!
*/

void
SoXtExaminerViewer::rightWheelMotion( // virtual, protected
  float value )
{
  this->zoom( this->getRightWheelValue() - value );
  inherited::rightWheelMotion( value );
} // rightWheelMotion()

// *************************************************************************

void
SoXtExaminerViewer::zoom(
  float diffvalue )
{
  SoCamera * camera = this->getCamera();
  assert( camera != NULL );
  SoType type = camera->getTypeId();

  // This will be in the range of <0, ->>.
  float multiplicator = exp(diffvalue);

  if ( type.isDerivedFrom( SoOrthographicCamera::getClassTypeId() ) ) {
    SoOrthographicCamera * ortho = (SoOrthographicCamera *) camera;
    ortho->height = ortho->height.getValue() * multiplicator;
  } else if ( type.isDerivedFrom( SoPerspectiveCamera::getClassTypeId()) ) {
    float oldfocaldist = camera->focalDistance.getValue();
    camera->focalDistance = oldfocaldist * multiplicator;

    SbVec3f direction;
    camera->orientation.getValue().multVec( SbVec3f( 0, 0, -1 ), direction );
    camera->position =
      camera->position.getValue() +
      (camera->focalDistance.getValue() - oldfocaldist) * -direction;
  } else {
    assert( 0 );
  }
} // zoom()

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
*/

void
SoXtExaminerViewer::setAnimationEnabled(
  SbBool enable )
{
#if SOXT_DEBUG
  if ( this->animatingallowed == enable )
    SoDebugError::postInfo( "SoXtExaminerViewer::setAnimationEnabled",
      "current state and argument the same" );
#endif // SOXT_DEBUG
  if ( ! enable && this->isAnimating() )
    this->stopAnimating();
  this->animatingallowed = enable;
} // setAnimationEnabled()

/*!
*/

SbBool
SoXtExaminerViewer::isAnimationEnabled(
  void ) const
{
  return this->animatingallowed;
} // isAnimationEnabled()

/*!
*/

SbBool
SoXtExaminerViewer::isAnimating(
  void ) const
{
  return this->spinanimating;
} // isAnimating()

/*!
*/

void
SoXtExaminerViewer::stopAnimating(
  void )
{
  if ( this->spinanimating ) {
    this->timertrigger->unschedule();
    this->interactiveCountDec();
    this->spinanimating = FALSE;
  }
#if SOXT_DEBUG
  else {
    SoDebugError::postWarning( "SoQtExaminerViewer::stopAnimating",
                               "not animating" );
  }
#endif // SOXT_DEBUG
} // stopAnimating()

// *************************************************************************

void
SoXtExaminerViewer::timertriggerCallback(
  SoSensor * sensor )
{
#if 0 // debug
  SoDebugError::postInfo("SoXtExaminerViewer::timertriggeredCB",
                         "spin samples: %d", this->spinsamplecounter);
#endif // debug

  if ( this->spinsamplecounter < 2 ) {
    // FIXME: won't the first check here always equal TRUE? 990501
    // mortene.
    if ( this->isAnimating() )
      this->stopAnimating();
#if 0 // check hypothesis from above FIXME statement.
    else
      this->timertrigger->unschedule();
#else
    else
      assert(0);
#endif
    return;
  }

  this->reorientCamera( this->spinincrement );
} // timertrigger()

void
SoXtExaminerViewer::timertriggerCB( // static
  void * user,
  SoSensor * sensor )
{
  assert( user != NULL );
  ((SoXtExaminerViewer *)user)->timertriggerCallback( sensor );
} // timertriggerCB()

// *************************************************************************

void
SoXtExaminerViewer::reorientCamera(
  const SbRotation & rotation )
{
  SoCamera * camera = this->getCamera();
  assert( camera != NULL );

  SbVec3f direction;
  camera->orientation.getValue().multVec( SbVec3f( 0, 0, -1 ), direction );
  SbVec3f focalpoint = camera->position.getValue() +
      camera->focalDistance.getValue() * direction;

  camera->orientation = rotation * camera->orientation.getValue();
  camera->orientation.getValue().multVec( SbVec3f( 0, 0, -1 ), direction );
  camera->position = focalpoint - camera->focalDistance.getValue() * direction;
} // reorientCamera()

void
SoXtExaminerViewer::spin(
  const SbVec2f & mousepos )
{
  assert( this->projector != NULL );

  SbRotation rotation;
  this->projector->projectAndGetRotation( mousepos, rotation );
  rotation.invert();
  this->reorientCamera( rotation );

  SbVec3f dummy_axis, newaxis;
  float acc_angle, newangle;
  this->spinincrement.getValue( dummy_axis, acc_angle );
  acc_angle *= this->spinsamplecounter; // weight
  rotation.getValue( newaxis, newangle );
  acc_angle += newangle;

  this->spinsamplecounter++;
  acc_angle /= this->spinsamplecounter;

  this->spinincrement.setValue( newaxis, acc_angle );
  if ( this->spinsamplecounter > 3 )
    this->spinsamplecounter = 3;
} // spin()

// *************************************************************************

void
SoXtExaminerViewer::setModeFromState(
  const unsigned int state )
{
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

void
SoXtExaminerViewer::setMode(
  ViewerMode mode )
{
  switch ( mode ) {

  case INTERACT:
    if ( this->isAnimating() )
      this->stopAnimating();
    while ( this->getInteractiveCount() )
      this->interactiveCountDec();
    break;

  case DRAGGING:
    this->projector->project( this->lastmouseposition );
    break;

  case PANNING:
    do {
      SoCamera * camera = this->getCamera();
      SbViewVolume volume = camera->getViewVolume( this->getGLAspectRatio() );
      this->panningplane = volume.getPlane( camera->focalDistance.getValue() );
    } while ( FALSE );
    break;

  default:
    break;

  } // switch ( mode )

  this->mode = mode;
} // setMode()

// *************************************************************************

void
SoXtExaminerViewer::spindetecttimerCB( // static
  XtPointer user,
  XtIntervalId * id )
{
  SoXtExaminerViewer * that = (SoXtExaminerViewer *) user;
  that->spindetecttimerId = 0;
  that->spindetecttimerActive = FALSE;
} // spindetecttimerCB()

// *************************************************************************

void
SoXtExaminerViewer::pan(
  const SbVec2f & mousepos )
{
  SoCamera * cam = this->getCamera();
  assert(cam);

  // Find projection points for the last and current mouse
  // coordinates.
  SbViewVolume vv = cam->getViewVolume(this->getGLAspectRatio());
  SbLine line;
  vv.projectPointToLine(mousepos, line);
  SbVec3f current_planept;
  this->panningplane.intersect(line, current_planept);
  vv.projectPointToLine(this->lastmouseposition, line);
  SbVec3f old_planept;
  this->panningplane.intersect(line, old_planept);

  // Reposition camera according to the vector difference between the
  // projected points.
  cam->position = cam->position.getValue() - (current_planept - old_planept);
} // pan()

void
SoXtExaminerViewer::zoomByCursor(
  const SbVec2f & mousepos )
{
  this->zoom( (mousepos[1] - this->lastmouseposition[1]) * 20.0f );
} // zoomByCursor()

