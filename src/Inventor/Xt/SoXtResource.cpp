/**************************************************************************\
 *
 *  This file is part of the Coin family of 3D visualization libraries.
 *  Copyright (C) 1998-2003 by Systems in Motion.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and / or
 *  modify it under the terms of the GNU General Public License
 *  version 2 as published by the Free Software Foundation.  See the
 *  file LICENSE.GPL at the root directory of this source distribution
 *  for more details.
 *
 *  If you desire to use this library in software that is incompatible
 *  with the GNU GPL, and / or you would like to take advantage of the
 *  additional benefits with regard to our support services, please
 *  contact Systems in Motion about acquiring a Coin Professional
 *  Edition License.  See <URL:http://www.coin3d.org> for more
 *  information.
 *
 *  Systems in Motion, Abels gate 5, Teknobyen, 7030 Trondheim, NORWAY
 *  <URL:http://www.sim.no>, <mailto:support@sim.no>
 *
\**************************************************************************/

#if SOXT_DEBUG
static const char rcsid[] =
  "$Id$";
#endif // SOXT_DEBUG

// *************************************************************************

/*!
  \class SoXtResource Inventor/Xt/SoXtResource.h
  \brief The SoXtResource class is a utility class for fetching X resource
  values for widgets.  Special care is taken for SoXt components.
  \ingroup misc
*/

// *************************************************************************

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include <X11/IntrinsicP.h>
#include <X11/CoreP.h>

#include <Inventor/SoLists.h>
#include <Inventor/errors/SoDebugError.h>

#include <soxtdefs.h>
#include <Inventor/Xt/SoXt.h>
#include <Inventor/Xt/SoXtComponent.h>

#include <Inventor/Xt/SoXtResource.h>

// *************************************************************************

int icstrcmp(const char * str1, const char * str2);

/*!
  Constructor.   sets up the SoXtResource object to fetch resources for
  the \a widget Widget.
*/

SoXtResource::SoXtResource(
  const Widget widget)
{
  static SbBool initialized = FALSE;
  if (! initialized) {
    XrmInitialize();
    initialized = TRUE;
  }

  this->hierarchy_depth = 0;

  if (! widget) {
    this->name_hierarchy = NULL;
    this->class_hierarchy = NULL;
    this->display = SoXt::getDisplay();
    return;
  }

  this->display = XtDisplay(widget);

  SbIntList quarks;
  Widget stop = NULL;

  SoXtComponent * component = SoXtComponent::getComponent(widget);
  if (component != NULL) {
    Widget cwidget = component->getBaseWidget();
    if (cwidget != NULL)
      stop = XtParent(cwidget);
  }

  Widget w = widget;
  while (w && w != stop) {
    quarks.append(((CorePart *) w)->xrm_name);
    quarks.append(((CoreClassPart *) XtClass(w))->xrm_class);
    this->hierarchy_depth++;
    if (XtIsShell(w))
      break;
    if ((component == NULL) &&
         ((component = SoXtComponent::getComponent(w)) != NULL)) {
      Widget cwidget = component->getWidget();
      if (cwidget != NULL)
        stop = XtParent(cwidget);
    }
    w = XtParent(w);
  }

#if SOXT_DEBUG
  if (component == NULL)
    SoDebugError::postInfo("SoXtResource",
      "using SoXtResource for non-component widget (which is OK)");
#endif // SOXT_DEBUG

  this->name_hierarchy = new XrmQuark [ this->hierarchy_depth + 2];
  this->class_hierarchy = new XrmQuark [ this->hierarchy_depth + 2];
  int i;
  for (i = 0; i < this->hierarchy_depth; i++) {
    this->name_hierarchy[this->hierarchy_depth - i - 1] = quarks[i*2];
    this->class_hierarchy[this->hierarchy_depth - i - 1] = quarks[i*2+1];
  }
  this->name_hierarchy[ this->hierarchy_depth ] = 0;
  this->name_hierarchy[ this->hierarchy_depth + 1 ] = 0;
  this->class_hierarchy[ this->hierarchy_depth ] = 0;
  this->class_hierarchy[ this->hierarchy_depth + 1 ] = 0;

#if SOXT_DEBUG && 0
  this->DumpInternals();
#endif // SOXT_DEBUG
} // SoXtResource()

/*!
  Destructor.
*/

SoXtResource::~SoXtResource(
  void)
{
  delete [] this->name_hierarchy;
  delete [] this->class_hierarchy;
} // ~SoXtResource()

// *************************************************************************

/*!
  This method just dumps the name and class hierarchy of the widget the
  SoXtResource object is set to.
*/

void
SoXtResource::DumpInternals(
  void) const
{
  SoDebugError::postInfo("SoXtResource::DumpInternals", "dumping");
  fprintf(stdout, "Classes: ");
  int i;
  for (i = 0; i < this->hierarchy_depth; i++) {
    fprintf(stdout, "%s", XrmQuarkToString(this->class_hierarchy[i]));
    if (i < (this->hierarchy_depth - 1))
      fprintf(stdout, ".");
  }
  fprintf(stdout, "\n");

  fprintf(stdout, "Names:  ");
  for (i = 0; i < this->hierarchy_depth; i++) {
    fprintf(stdout, "%s", XrmQuarkToString(this->name_hierarchy[i]));
    if (i < (this->hierarchy_depth - 1))
      fprintf(stdout, ".");
  }
  fprintf(stdout, "\n");
} // DumpInternals()

// *************************************************************************

#define GET_RESOURCE()                                                         \
  XrmValue value;                                                              \
  XrmRepresentation format;                                                    \
  char * formatstr = NULL;                                                     \
  do {                                                                         \
    SbBool found = FALSE;                                                      \
    XrmDatabase database = XrmGetDatabase(this->display);                    \
    if (this->name_hierarchy != NULL) {                                      \
      this->name_hierarchy[this->hierarchy_depth] = XrmStringToQuark(rname); \
      this->class_hierarchy[this->hierarchy_depth] =                           \
        XrmStringToQuark(rclass);                                            \
      found = XrmQGetResource(database, this->name_hierarchy,                 \
                this->class_hierarchy, &format, &value) ? TRUE : FALSE;       \
      this->name_hierarchy[this->hierarchy_depth] = 0;                         \
      this->class_hierarchy[this->hierarchy_depth] = 0;                        \
    }                                                                          \
    if (! found)                                                             \
      found = XrmGetResource(database, rname,                                 \
                rclass, &formatstr, &value) ? TRUE : FALSE;                   \
    if (! found)                                                             \
      return FALSE;                                                            \
  } while (FALSE)

// *************************************************************************

/*!
  This method retrieves the given X resource and puts it into the
  SbColor object \a retval.

  TRUE is returned if the resource is found, and FALSE otherwise.
*/

SbBool
SoXtResource::getResource(
  const char * const rname,
  const char * const rclass,
  SbColor & retval) const
{
  GET_RESOURCE();
  SOXT_STUB_ONCE();

  XrmQuark stringq = XrmStringToQuark(XmRString);

  if (formatstr != NULL)
    format = XrmStringToQuark(formatstr);

  if (format == stringq) {
    XColor exact, screen;
    Display * dpy = SoXt::getDisplay();
    Colormap cmap = 0; // = SoXt::getColormap();
    if (XLookupColor(dpy, cmap, (char *) value.addr, &exact, &screen)) {
      retval = SbColor(float(exact.red) / 65535.0f,
        float(exact.green) / 65535.0f, float(exact.blue) / 65535.0f);
      return TRUE;
    }
    return FALSE;
  }

#if SOXT_DEBUG
  SoDebugError::postInfo("getResource",
    "resource format \"%s\" not supported\n", XrmQuarkToString(format));
#endif // SOXT_DEBUG
  return FALSE;
} // getResource()

// *************************************************************************

/*!
  This method retrieves the given X resource and puts it into the
  short \a retval.

  TRUE is returned if the resource is found, and FALSE otherwise.
*/

SbBool
SoXtResource::getResource(
  const char * const rname,
  const char * const rclass,
  short & retval) const
{
  GET_RESOURCE();

  XrmQuark shortq = XrmStringToQuark(XmRShort);
  XrmQuark stringq = XrmStringToQuark(XmRString);

  if (formatstr != NULL)
    format = XrmStringToQuark(formatstr);

  if (format == shortq) {
    retval = *((short *) value.addr);
    return TRUE;
  }
  if (format == stringq) {
    retval = atoi((char *) value.addr);
    return TRUE;
  }

#if SOXT_DEBUG
  SoDebugError::postInfo("getResource",
    "resource format \"%s\" not supported\n", XrmQuarkToString(format));
#endif // SOXT_DEBUG
  return FALSE;
} // getResource()

// *************************************************************************

/*!
  This method retrieves the given X resource and puts it into the
  unsigned short \a retval.

  TRUE is returned if the resource is found, and FALSE otherwise.
*/

SbBool
SoXtResource::getResource(
  const char * const rname,
  const char * const rclass,
  unsigned short & retval) const
{
  GET_RESOURCE();

  XrmQuark stringq = XrmStringToQuark(XmRString);
  XrmQuark shortq = XrmStringToQuark(XmRShort);

  if (formatstr != NULL)
    format = XrmStringToQuark(formatstr);

  if (format == shortq) {
    retval = *((unsigned short *) value.addr);
    return TRUE;
  }

  if (format == stringq) {
    retval = atoi((char *) value.addr);
    return TRUE;
  }
  
#if SOXT_DEBUG
  SoDebugError::postInfo("getResource",
    "resource format \"%s\" not supported\n", XrmQuarkToString(format));
#endif // SOXT_DEBUG
  return FALSE;
} // getResource()

// *************************************************************************

/*!
  This method retrieves the given X resource and points the \a retval
  pointer to it's data.

  TRUE is returned if the resource is found, and FALSE otherwise.
*/

SbBool
SoXtResource::getResource(
  const char * const rname,
  const char * const rclass,
  char * & retval) const
{
  GET_RESOURCE();

  XrmQuark stringq = XrmStringToQuark(XmRString);

  if (formatstr != NULL)
    format = XrmStringToQuark(formatstr);

  if (format == stringq) {
    retval = (char *) value.addr;
    return TRUE;
  }

#if SOXT_DEBUG
  SoDebugError::postInfo("getResource",
    "resource format \"%s\" not supported\n", XrmQuarkToString(format));
#endif // SOXT_DEBUG
  return FALSE;
} // getResource()

// *************************************************************************

/*!
  This method retrieves the given X resource and puts it into the
  SbBool \a retval.

  TRUE is returned if the resource is found, and FALSE otherwise.
*/

SbBool
SoXtResource::getResource(
  const char * const rname,
  const char * const rclass,
  SbBool & retval) const
{
  GET_RESOURCE();

  XrmQuark stringq = XrmStringToQuark(XmRString);
  XrmQuark booleanq = XrmStringToQuark(XmRBoolean);

  if (formatstr != NULL)
    format = XrmStringToQuark(formatstr);

  if (format == booleanq) {
    retval = *((Boolean *) value.addr) ? TRUE : FALSE;
    return TRUE;
  }

  if (format == stringq) {
    if (icstrcmp((const char *) value.addr, "true") == 0 ||
         icstrcmp((const char *) value.addr, "on") == 0 ||
         icstrcmp((const char *) value.addr, "yes") == 0 ||
         icstrcmp((const char *) value.addr, "enable") == 0 ||
         icstrcmp((const char *) value.addr, "enabled") == 0 ||
         icstrcmp((const char *) value.addr, "set") == 0 ||
         icstrcmp((const char *) value.addr, "1") == 0) {
      retval = TRUE;
      return TRUE;
    } else if (icstrcmp((const char *) value.addr, "false") == 0 ||
                icstrcmp((const char *) value.addr, "off") == 0 ||
                icstrcmp((const char *) value.addr, "no") == 0 ||
                icstrcmp((const char *) value.addr, "disable") == 0 ||
                icstrcmp((const char *) value.addr, "disabled") == 0 ||
                icstrcmp((const char *) value.addr, "unset") == 0 ||
                icstrcmp((const char *) value.addr, "0") == 0) {
      retval = FALSE;
      return TRUE;
    } else {
      SoDebugError::postWarning("getResource",
        "string \"%s\" not understood", (char *) value.addr);
      return FALSE;
    }
  }

#if SOXT_DEBUG
  SoDebugError::postInfo("getResource",
    "resource format \"%s\" not supported\n", XrmQuarkToString(format));
#endif // SOXT_DEBUG
  return FALSE;
} // getResource()

// *************************************************************************

/*!
  This method retrieves the given X resource and puts it into the
  float \a retval.

  TRUE is returned if the resource is found, and FALSE otherwise.
*/

SbBool
SoXtResource::getResource(
  const char * const rname,
  const char * const rclass,
  float & retval) const
{
  GET_RESOURCE();

  XrmQuark stringq = XrmStringToQuark(XmRString);
  XrmQuark floatq = XrmStringToQuark(XmRFloat);

  if (formatstr != NULL)
    format = XrmStringToQuark(formatstr);

  if (format == floatq) {
    retval = *((float *) value.addr);
    return TRUE;
  }

  if (format == stringq) {
    retval = atof((char *) value.addr);
    return TRUE;
  }

#if SOXT_DEBUG
  SoDebugError::postInfo("getResource",
    "resource format \"%s\" not supported\n", XrmQuarkToString(format));
#endif // SOXT_DEBUG
  return FALSE;
} // getResource()

// *************************************************************************

inline char upcase(char letter) {
  if (letter >= 'a' && letter <= 'z')
    return letter - 'a' + 'A';
  return letter;
}

int
icstrcmp(
  const char * str1,
  const char * str2)
{
  int i = 0;
  while (str1[i] && (upcase(str1[i]) == upcase(str2[i]))) i++;
  return str2[i] - str1[i];
} // icstrcmp()

// *************************************************************************

#if SOXT_DEBUG
static const char * getSoXtResourceRCSId(void) { return rcsid; }
#endif // SOXT_DEBUG

