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

//  $Id$

#ifndef SOXT_CLIPBOARD_H
#define SOXT_CLIPBOARD_H

// FIXME: this is not compatible with SGI/TGS Inventor. 20000528 mortene.
#include <Inventor/system/inttypes.h>

#include <X11/Xatom.h>
#include <X11/Intrinsic.h>
#include <Inventor/SbBasic.h>

class SbDict;
class SoByteStream;
class SoNode;
class SoPath;
class SoPathList;
class SoSelection;
class SoXtImportInterestList;
class SbString;
class SbPList;

// *************************************************************************

typedef void SoXtClipboardPasteCB(
               void * userdata, SoPathList * pathlist );
typedef void SoXtClipboardImportCB(
               void * userdata, Atom type, void * data, uint32_t size );

#define _XA_CLIPBOARD_ ((Atom) 0)

class SoXtClipboard {
public:
  SoXtClipboard( Widget widget, Atom selection = _XA_CLIPBOARD_ );
  ~SoXtClipboard(void);

  void copy( SoNode * node, Time when );
  void copy( SoPath * path, Time when );
  void copy( SoPathList * paths, Time when );
  void copy( Atom type, void * data, uint32_t size, Time when );

  void paste( Time when, SoXtClipboardPasteCB * pastedone,
    void * userdata = NULL );

  void addPasteInterest( Atom datatype, SoXtClipboardImportCB * importfunc,
    void * userdata = NULL );

  static SbBool convertData( Widget widget, void * srcdata, uint32_t size,
    Atom desiredType, char ** retdata, uint32_t * retsize );

  void setEmptyListOK( SbBool enable );
  SbBool isEmptyListOK(void) const;

protected:
  Widget widget;
  Atom clipboardAtom;
  Time eventTime;

  SoXtClipboardPasteCB * callbackFunc;
  void * userData;

  static SbDict * selOwnerList;

  void copy( SoByteStream * byteStream, Time when );

  SoXtImportInterestList * pasteInterest;
  SbPList * copyInterest;

  void getExportTargets( char ** value, uint32_t * length );
  Atom chooseFromImportTargets( Atom * supportedTargets, int length );
  static SbBool writeToFile( SbString & tmpfile, void * srcdata, uint32_t size );
  static void readFile( SoPathList * & paths, const char * filename );

  static SoPathList * readData( Widget w, Atom target, void * data, uint32_t size );

}; // class SoXtClipboard

// *************************************************************************

#endif // ! SOXT_CLIPBOARD_H
