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

#include <soxtdefs.h>

#include <Inventor/Xt/SoXtClipboard.h>

// *************************************************************************

SoXtClipboard::SoXtClipboard(
  Widget widget,
  Atom selection )
{
} // SoXtClipboard()

SoXtClipboard::~SoXtClipboard(
  void )
{
} // ~SoXtClipboard()

// *************************************************************************

void
SoXtClipboard::copy(
  SoNode * node,
  Time when )
{
  SOXT_STUB();
} // copy()

void
SoXtClipboard::copy(
  SoPath * path,
  Time when )
{
  SOXT_STUB();
} // copy()

void
SoXtClipboard::copy(
  SoPathList * paths,
  Time when )
{
  SOXT_STUB();
} // copy()

void
SoXtClipboard::copy(
  Atom type,
  void * data,
  uint32_t size,
  Time when )
{
  SOXT_STUB();
} // copy()

void
SoXtClipboard::copy( // protected
  SoByteStream * byteStream,
  Time when )
{
  SOXT_STUB();
} // copy()

void
SoXtClipboard::paste(
  Time when,
  SoXtClipboardPasteCB * pastedone,
  void * userdata )
{
  SOXT_STUB();
} // paste()

void
SoXtClipboard::addPasteInterest(
  Atom datatype,
  SoXtClipboardImportCB * importfunc,
  void * userdata )
{
  SOXT_STUB();
} // addPasteInterest()

SbBool
SoXtClipboard::convertData( // static
  Widget widget,
  void * srcdata,
  uint32_t size,
  Atom desiredType,
  char ** retdata,
  uint32_t * retsize )
{
  SOXT_STUB();
  return FALSE;
} // convertData()

void
SoXtClipboard::setEmptyListOK(
  SbBool enable )
{
  SOXT_STUB();
} // setEmptyListOK()

SbBool
SoXtClipboard::isEmptyListOK(
  void ) const
{
  SOXT_STUB();
  return FALSE;
} // isEmptyListOK()

void
SoXtClipboard::getExportTargets( // protected
  char ** value,
  uint32_t * length )
{
  SOXT_STUB();
} // getExportTargets()

Atom
SoXtClipboard::chooseFromImportTargets( // protected
  Atom * supportedTargets,
  int length )
{
  SOXT_STUB();
  return _XA_CLIPBOARD_;
} // chooseFromImportTargets()

SbBool
SoXtClipboard::writeToFile( // static, protected
  SbString & tmpfile,
  void * srcdata,
  uint32_t size )
{
  SOXT_STUB();
  return FALSE;
} // writeToFile()

void
SoXtClipboard::readFile( // static, protected
  SoPathList * & paths,
  const char * filename )
{
  SOXT_STUB();
} // readFile()

SoPathList *
SoXtClipboard::readData( // static, protected
  Widget w,
  Atom target,
  void * data,
  uint32_t size )
{
  SOXT_STUB();
  return (SoPathList *) NULL;
} // readData()

// *************************************************************************

#if SOXT_DEBUG
static const char * getSoXtClipboardRCSId(void) { return rcsid; }
#endif // SOXT_DEBUG

