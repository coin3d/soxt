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
  \class SoXtClipboard Inventor/Xt/SoXtClipboard.h
  \brief The SoXtClipboard class is yet to be documented.
  \ingroup misc

  This class is not implemented yet.
*/

// *************************************************************************

#include <soxtdefs.h>

#include <Inventor/Xt/SoXtClipboard.h>

// *************************************************************************

/*!
  \var Widget SoXtClipboard::widget
  FIXME: write doc
*/

/*!
  \var Atom SoXtClipboard::clipboardAtom
  FIXME: write doc
*/

/*!
  \var SbTime SoXtClipboard::eventTime
  FIXME: write doc
*/

/*!
  \var SoXtClipboardPasteCB SoXtClipboard::callbackFunc
  Paste callback function pointer.
*/

/*!
  \var SbTime SoXtClipboard::userData
  Callback closure.
*/

/*!
  \var SoXtImportInterestList * SoXtClipboard::pasteInterest
  FIXME: write doc
*/

/*!
  \var SbPList * SoXtClipboard::copyInterest
  FIXME: write doc
*/

/*!
  \var SbDict SoXtClipboard::selOwnerList
  FIXME: write doc
*/

// *************************************************************************

/*!
  Constructor.
*/

SoXtClipboard::SoXtClipboard(
  Widget widget,
  Atom selection )
{
} // SoXtClipboard()

/*!
  Destructor.
*/

SoXtClipboard::~SoXtClipboard(
  void )
{
} // ~SoXtClipboard()

// *************************************************************************

/*!
  FIXME: write doc
*/

void
SoXtClipboard::copy(
  SoNode * node,
  Time when )
{
  SOXT_STUB();
} // copy()

/*!
  FIXME: write doc
*/

void
SoXtClipboard::copy(
  SoPath * path,
  Time when )
{
  SOXT_STUB();
} // copy()

/*!
  FIXME: write doc
*/

void
SoXtClipboard::copy(
  SoPathList * paths,
  Time when )
{
  SOXT_STUB();
} // copy()

/*!
  FIXME: write doc
*/

void
SoXtClipboard::copy(
  Atom type,
  void * data,
  uint32_t size,
  Time when )
{
  SOXT_STUB();
} // copy()

/*!
  FIXME: write doc
*/

void
SoXtClipboard::copy( // protected
  SoByteStream * byteStream,
  Time when )
{
  SOXT_STUB();
} // copy()

/*!
  FIXME: write doc
*/

void
SoXtClipboard::paste(
  Time when,
  SoXtClipboardPasteCB * pastedone,
  void * userdata )
{
  SOXT_STUB();
} // paste()

/*!
  FIXME: write doc
*/

void
SoXtClipboard::addPasteInterest(
  Atom datatype,
  SoXtClipboardImportCB * importfunc,
  void * userdata )
{
  SOXT_STUB();
} // addPasteInterest()

/*!
  FIXME: write doc
*/

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

/*!
  FIXME: write doc
*/

void
SoXtClipboard::setEmptyListOK(
  SbBool enable )
{
  SOXT_STUB();
} // setEmptyListOK()

/*!
  FIXME: write doc
*/

SbBool
SoXtClipboard::isEmptyListOK(
  void ) const
{
  SOXT_STUB();
  return FALSE;
} // isEmptyListOK()

/*!
  FIXME: write doc
*/

void
SoXtClipboard::getExportTargets( // protected
  char ** value,
  uint32_t * length )
{
  SOXT_STUB();
} // getExportTargets()

/*!
  FIXME: write doc
*/

Atom
SoXtClipboard::chooseFromImportTargets( // protected
  Atom * supportedTargets,
  int length )
{
  SOXT_STUB();
  return _XA_CLIPBOARD_;
} // chooseFromImportTargets()

/*!
  FIXME: write doc
*/

SbBool
SoXtClipboard::writeToFile( // static, protected
  SbString & tmpfile,
  void * srcdata,
  uint32_t size )
{
  SOXT_STUB();
  return FALSE;
} // writeToFile()

/*!
  FIXME: write doc
*/

void
SoXtClipboard::readFile( // static, protected
  SoPathList * & paths,
  const char * filename )
{
  SOXT_STUB();
} // readFile()

/*!
  FIXME: write doc
*/

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

