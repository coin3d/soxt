
#include <assert.h>

#include <Inventor/Xt/SoXtTypedObject.h>
#include <Inventor/Xt/devices/SoXtDevice.h>
#include <Inventor/Xt/SoXtComponent.h>

SoType SoXtTypedObject::classTypeId; // static

void
SoXtTypedObject::init( // static
  void )
{
  SoXtTypedObject::initClass();
  SoXtDevice::initClasses();
  SoXtComponent::initClasses();
} // init()

void
SoXtTypedObject::initClass( // static
  void )
{
  assert( SoXtTypedObject::classTypeId == SoType::badType() );
  SoXtTypedObject::classTypeId =
    SoType::createType( SoType::badType(), "SoXtTypedObject");
} // initClass()

SbBool
SoXtTypedObject::isOfType(
  SoType type ) const
{
  return this->getTypeId().isDerivedFrom(type);
} // isOfType()

SoType
SoXtTypedObject::getClassTypeId( // static
  void ) 
{
  return SoXtTypedObject::classTypeId;
} // getClassTypeId()

