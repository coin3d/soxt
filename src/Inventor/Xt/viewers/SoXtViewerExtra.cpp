/**************************************************************************\
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
\**************************************************************************/

static const char rcsid[] =
  "$Id$";

#include <X11/keysym.h>

#include <Inventor/errors/SoDebugError.h>

#include <Inventor/Xt/viewers/SoXtViewer.h>

// *************************************************************************

// We want to avoid any application event callback to be invoked twice.
#define SOXTRENDERAREA_PROCESSEVENT(event) \
  do { \
    SoXtRenderAreaEventCB * apphandler = this->appEventHandler; \
    this->appEventHandler = NULL; \
    inherited::processEvent( event ); \
    this->appEventHandler = apphandler; \
  } while ( FALSE )


SbBool
SoXtViewer::processCommonEvents(
  XAnyEvent * event )
{
  if ( inherited::invokeAppCB( event ) )
    return TRUE;

  SbBool keypress    = (event->type == KeyPress);
  SbBool keyrelease  = (event->type == KeyRelease);
  SbBool keyevent    = keypress || keyrelease;

  char keybuf[8];
  int keybuflen = 0;
  KeySym keysym = 0;

  if ( keyevent ) 
    keybuflen = XLookupString( (XKeyEvent *) event, keybuf, 8, &keysym, NULL );

  // First check for Esc key to toggle back and forth between examine
  // and pick modes.
  if ( keypress && keysym == XK_Escape ) {
    this->setViewing( this->isViewing() ? FALSE : TRUE );
    return TRUE;
  }

  // If not in examine mode, always pass events upstream (to the
  // SoXtRenderArea and SoXtGLWidget, and also into the scenegraph).
  if ( ! this->isViewing() ) {
    SOXTRENDERAREA_PROCESSEVENT( event );
    // Do _not_ provide the subclasses with the event.
    return TRUE;
  }

  if ( keypress ) {
    SbVec2f pos( 0.0f, 0.0f );

#if SOXT_DEBUG && 0
    SoDebugError::postInfo( "processCommonEvents", "buflen = %d", keybuflen );
#endif // SOXT_DEBUG

    switch ( keysym ) {
    case XK_S:
    case XK_s:
      this->setSeekMode( this->isSeekMode() ? FALSE : TRUE );
      return TRUE;

    case XK_Home:
      this->resetToHomePosition();
      return TRUE;

    case XK_Left:
      pos.setValue( -0.1f, 0.0f );
      break;

    case XK_Up:
      pos.setValue( 0.0f, 0.1f );
      break;

    case XK_Right:
      pos.setValue( 0.1f, 0.0f );
      break;

    case XK_Down:
      pos.setValue( 0.0f, -0.1f );
      break;

    default:
      return FALSE;
    } // switch ( keysym )

    assert( pos != SbVec2f( 0.0f, 0.0f ) );
    this->moveCameraScreen( pos );
    return TRUE;
  }

  switch ( event->type ) {
  case ButtonPress:
  case ButtonRelease:
  case KeyRelease:
  case MotionNotify:
    // Avoid "fall-through" of events -- SoXtRenderArea would pass
    // these events to the scene graph if we used
    // inherited::processEvent(). Keypresses was handled above.
    return FALSE;
  }

  // Pass all other (non-user-interaction) events upstream (like
  // expose and resize events), aswell as making them available in
  // subclasses.
  SOXTRENDERAREA_PROCESSEVENT( event );
  return FALSE;
} // processCommonEvents()

// *************************************************************************
