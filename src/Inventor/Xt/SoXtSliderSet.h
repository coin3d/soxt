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

// $Id$

#ifndef SOXT_SLIDERSET_H
#define SOXT_SLIDERSET_H

#include <Inventor/Xt/SoXtSliderSetBase.h>

class SoXtSlider;
struct SoXtSliderSetClosure;

// *************************************************************************

class SOXT_DLL_API SoXtSliderSet : public SoXtSliderSetBase {
  SOXT_OBJECT_ABSTRACT_HEADER(SoXtSliderSet, SoXtSliderSetBase);

public:
  SoXtSliderSet(
    const Widget parent,
    const char * const name,
    const SbBool embed,
    SoNode * const node);

  virtual void setNode(SoNode * node);
  void updateLayout(void);

protected:
  SoXtSliderSet(
    const Widget parent,
    const char * const name,
    const SbBool embed,
    SoNode * const node,
    const SbBool build);
  ~SoXtSliderSet(void);

  Widget buildWidget(Widget parent);

private:
  void constructor(const SbBool build);

  void buildSliderModule(const Widget rowcolumn);
  void buildSliderSet(const Widget rowcolumn);

  void folding(SoXtSliderSetClosure * closure, Widget button);
  static void folding_cb(Widget, XtPointer, XtPointer);
  void style(SoXtSliderSetClosure * closure);
  static void style_cb(Widget, XtPointer, XtPointer);
  void slider(SoXtSliderSetClosure * closure, char * title, float value);
  static void slider_cb(void * closure, char * title, float value);

private:
  int numSliders;
  SoXtSlider ** sliders;

  SbPList * closures;

}; // class SoXtSliderSet

// *************************************************************************

#endif // ! SOXT_SLIDERSET_H
