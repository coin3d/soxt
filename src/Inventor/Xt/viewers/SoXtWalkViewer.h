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

#ifndef SOXT_WALKVIEWER_H
#define SOXT_WALKVIEWER_H

#include <Inventor/Xt/viewers/SoXtConstrainedViewer.h>

class SoAnyWalkViewer;

// *************************************************************************

class SOXT_DLL_API SoXtWalkViewer : public SoXtConstrainedViewer {
  SOXT_OBJECT_HEADER(SoXtWalkViewer, SoXtConstrainedViewer);
  friend class SoAnyWalkViewer;

public:
  SoXtWalkViewer(
    Widget parent = NULL,
    const char * const name = NULL,
    SbBool embed = TRUE,
    SoXtFullViewer::BuildFlag flag = BUILD_ALL,
    SoXtViewer::Type type = BROWSER);
  ~SoXtWalkViewer(void);

  virtual void setViewing(SbBool enable);
  virtual void setCamera(SoCamera * camera);
  virtual void setCursorEnabled(SbBool enable);
  virtual void setCameraType(SoType type);

protected:
  SoXtWalkViewer(
    Widget parent,
    const char * const name,
    SbBool embed,
    SoXtFullViewer::BuildFlag flag,
    SoXtViewer::Type type,
    SbBool buildNow);

  virtual const char * getDefaultWidgetName(void) const;
  virtual const char * getDefaultTitle(void) const;
  virtual const char * getDefaultIconTitle(void) const;

  virtual void processEvent(XAnyEvent * event);
  virtual void setSeekMode(SbBool enable);
  virtual void actualRedraw(void);
  virtual void rightWheelMotion(float value);

  virtual void leftWheel2Start(void);
  virtual void leftWheel2Motion(float value);
  virtual void leftWheel2Finish(void);
  float getLeftWheel2Value(void) const;
  void setLeftWheel2String(char * string);

  virtual Widget buildLeftTrim(Widget parent);
  virtual void createPrefSheet(void);
  virtual void openViewerHelpCard(void);

private:
  void constructor(SbBool build);

  SoAnyWalkViewer * const common;
  Widget * prefparts;
  int numprefparts;

  Widget heightwheel;
  float heightvalue;

  void wheelarmed(void);
  static void wheelarmedCB(Widget, XtPointer, XtPointer);
  void wheelchanged(float value);
  static void wheelchangedCB(Widget, XtPointer, XtPointer);
  void wheeldisarmed(void);
  static void wheeldisarmedCB(Widget, XtPointer, XtPointer);

}; // class SoXtWalkViewer

// *************************************************************************

#endif // ! SOXT_WALKVIEWER_H
