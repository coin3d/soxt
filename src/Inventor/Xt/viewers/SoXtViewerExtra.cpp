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

  if ( keyevent ) {
    keybuflen = XLookupString( (XKeyEvent *) event, keybuf, 8, &keysym, NULL );
    if ( keysym == XK_Escape ) {
      if ( keypress )
        this->setViewing( this->isViewing() ? FALSE : TRUE );
      return TRUE;
    }
  }

  if ( ! this->isViewing() ) {
    inherited::processEvent( event );
    return TRUE;
  }

  if ( ! keypress )
    return FALSE;

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
} // processCommonEvents()

// *************************************************************************
