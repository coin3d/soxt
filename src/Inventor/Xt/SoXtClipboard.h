/**************************************************************************
 *
 *  This file is part of the Coin SoXt GUI binding library.
 *  Copyright (C) 2000 by Systems in Motion.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License version
 *  2.1 as published by the Free Software Foundation.  See the file
 *  LICENSE.LGPL at the root directory of the distribution for all the
 *  details.
 *
 *  If you want to use Coin SoXt for applications not compatible with the
 *  LGPL, please contact SIM to acquire a Professional Edition License.
 *
 *  Systems in Motion, Prof Brochs gate 6, N-7030 Trondheim, NORWAY
 *  http://www.sim.no/ support@sim.no Voice: +47 22114160 Fax: +47 22207097
 *
 **************************************************************************/

// $Id$

#ifndef SOXT_CLIPBOARD_H
#define SOXT_CLIPBOARD_H

#include <X11/Xatom.h>
#include <X11/Intrinsic.h>

#include <Inventor/SbBasic.h>
#include <Inventor/Xt/SoXtBasic.h>

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

class SOXT_DLL_EXPORT SoXtClipboard {
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
