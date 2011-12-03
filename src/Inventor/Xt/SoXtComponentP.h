/**************************************************************************\
 * Copyright (c) Kongsberg Oil & Gas Technologies AS
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 * 
 * Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 * 
 * Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 * 
 * Neither the name of the copyright holder nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
\**************************************************************************/

#ifndef SOXTCOMPONENTP_H
#define SOXTCOMPONENTP_H

#include <Inventor/Xt/SoGuiComponentP.h>
#include <Inventor/Xt/SoXtCursor.h>

// ************************************************************************

// This class contains private data and methods used within the
// SoXtComponent class.

class SoXtComponentP : public SoGuiComponentP
{
public:
  SoXtComponentP(SoXtComponent * publ);
  ~SoXtComponentP();

  static Cursor getNativeCursor(Display * d, const SoXtCursor::CustomCursor * cc);

  static void wmDeleteWindowCB(Widget w, XtPointer user, XtPointer call);
  static void widgetDestroyedCB(Widget w, XtPointer user, XtPointer call);

  static void structureNotifyOnWidgetCB(Widget w, XtPointer user, XEvent * e, Boolean *);
  static void structureNotifyOnShellCB(Widget w, XtPointer user, XEvent * e, Boolean *);

  void checkVisibilityChange(void);

  Widget parent;
  Widget widget;
  char * widgetname;
  char * widgetclass;
  char * title;
  char * icontitle;

  SbBool embedded;

  SbBool widgetmappedstatus;
  SbBool shellmappedstatus;

  SbVec2s size;

  SbPList * closecbs;
  SbPList * visibilitycbs;
  SbBool visibilitystate;
    
  SbBool fullscreen;

private:
  static SbDict * cursordict;
};

// ************************************************************************

#endif // !SOXTCOMPONENTP_H
