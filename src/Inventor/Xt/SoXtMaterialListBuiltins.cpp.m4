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

static const char rcsid[] =
  "$Id$";

/**************************************************************************
 * IMPORTANT NOTICE
 * Be aware that the file SoXtMaterialListBuiltins.cpp is generated from
 * the file SoXtMaterialListBuiltins.cpp.m4, so changes has to be done in
 * the source file with the m4 macros.  The generation is done manually,
 * since it is no point in making this project depend on the user having
 * m4 installed.
 **************************************************************************/

#include <assert.h>
#include <string.h>

#include <Inventor/Xt/SoXtMaterialList.h>

#include <materials/materials.h>

include(libm4.m4)
divert(0)dnl
// *************************************************************************

#define SOXT_BUILTIN_MATERIALS 0x0001
define([SOXT_BUILTIN_MATERIAL_GROUPS], [13])dnl
define([SOXT_BUILTIN_MATERIAL_GROUP_SIZE], [35])dnl

struct SoXtMaterial {
  const char * name;
  const char * data;
};

struct SoXtMaterialGroup {
  const char * name;
  short numMaterials;
  SoXtMaterial ** materials;
};

struct SoXtMaterialDirectory {
  short flags;
  short numGroups;
  SoXtMaterialGroup ** groups;
};

// *************************************************************************

static
const char *
changequote(,)dnl
builtin_material_names[] = {
changequote([,])dnl
define([matnum], 0)dnl
m4_foreach([material], [artdeco, autumn, glass, metal, neon, rococo, santafe,
                        sheen, silky, spring, summer, tropical, winter], [dnl
  "material",
m4_for([num], 0, m4_eval(SOXT_BUILTIN_MATERIAL_GROUP_SIZE-1),,
[dnl
    "material.num",
])dnl
define([matnum], m4_eval(matnum+1))dnl
])dnl
  NULL
}; // builtin_material_names

// *************************************************************************

/*!
  \internal

  This method fills in the SoXtMaterialDirectory structure with the builtin
  material data.
*/

void
SoXtMaterialList::setupBuiltinMaterials( // private
  SoXtMaterialDirectory * index ) const
{
  assert( index != NULL );
changequote(,)dnl
  index->flags |= SOXT_BUILTIN_MATERIALS;
  index->groups = new SoXtMaterialGroup * [ SOXT_BUILTIN_MATERIAL_GROUPS ];
  index->numGroups = SOXT_BUILTIN_MATERIAL_GROUPS;
  for ( int i = 0; i < index->numGroups; i++ ) {
    index->groups[i] = new SoXtMaterialGroup;
    index->groups[i]->numMaterials = SOXT_BUILTIN_MATERIAL_GROUP_SIZE;
    index->groups[i]->materials = new SoXtMaterial * [ SOXT_BUILTIN_MATERIAL_GROUP_SIZE ];
    for ( int j = 0; j < SOXT_BUILTIN_MATERIAL_GROUP_SIZE; j++ ) {
      index->groups[i]->materials[j] = new SoXtMaterial;
    }
  }

changequote([,])dnl
define([matnum], 0)dnl
m4_foreach([material], [artdeco, autumn, glass, metal, neon, rococo, santafe,
                        sheen, silky, spring, summer, tropical, winter], [dnl
changequote(<,>)dnl
  index->groups[matnum]->name = builtin_material_names[m4_eval(matnum*(SOXT_BUILTIN_MATERIAL_GROUP_SIZE+1))];
changequote([,])dnl
m4_for([num], 0, m4_eval(SOXT_BUILTIN_MATERIAL_GROUP_SIZE-1),,
[changequote(<,>)dnl
  index->groups[matnum]->materials[num]->name = builtin_material_names[m4_eval(matnum*(SOXT_BUILTIN_MATERIAL_GROUP_SIZE+1)+num+1)];
  index->groups[matnum]->materials[num]->data = material<>num<>_iv;
changequote([,])dnl
])dnl
define([matnum], m4_eval(matnum+1))dnl
])dnl
} // setupBuiltinMaterials()

// *************************************************************************
