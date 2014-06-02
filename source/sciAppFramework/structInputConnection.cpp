
// ======================================================

#include "sciAppFramework/inputWidget.h"
#include "sciAppFramework/structInputConnection.h"

#include <QVariant>
#include <QString>
#include <QDebug>

#include <string>
#include <stdexcept>

using namespace sciAppFramework;

// ======================================================

namespace
{
  template <class T> T& valueFromVoidPointer( void* Pointer, size_t Shift )
  {
    Q_ASSERT( Pointer != NULL );
    return * reinterpret_cast< T* >( reinterpret_cast< char* >( Pointer ) + Shift );
  }
  
  template <class T> const T& valueFromVoidPointer( const void* Pointer, size_t Shift )
  {
    Q_ASSERT( Pointer != NULL );
    return * reinterpret_cast< const T* >( reinterpret_cast< const char* >( Pointer ) + Shift );
  }
}

// ======================================================

structInputConnection::fieldProperties::fieldProperties() :
  Shift( 0 ),
  Type( NULL )
{
}

// ------------------------------------------------------

structInputConnection::fieldProperties::fieldProperties( size_t S, const std::type_info &T ) :
  Shift( S ),
  Type( &T )
{
}

// ------------------------------------------------------
          
QVariant structInputConnection::fieldProperties::valueFromStruct( const void *StructPointer ) const
{
  if ( StructPointer == NULL )
    return QVariant();

  if ( Type == NULL )
    return QVariant();

#define VALUE_FROM_STRUCT( T )  \
  ( valueFromVoidPointer< T >( StructPointer, Shift ) )

#define RETURN_IF_TYPE( T )     \
  do {                          \
    if ( *Type == typeid(T) )   \
      return VALUE_FROM_STRUCT(T); \
  } while ( false )             \

  RETURN_IF_TYPE( bool );
  RETURN_IF_TYPE( char );
  RETURN_IF_TYPE( short );
  RETURN_IF_TYPE( int );
  RETURN_IF_TYPE( unsigned );
  RETURN_IF_TYPE( float );
  RETURN_IF_TYPE( double );
  RETURN_IF_TYPE( QString );
  
  if ( *Type == typeid(std::string) )
    return QString::fromStdString( VALUE_FROM_STRUCT(std::string) );

  qWarning() << "structInputConnection::fieldProperties::valueFromStruct: invalid type for conversion: " << Type->name();
  return QVariant();

#undef RETURN_IF_TYPE
#undef VALUE_FROM_STRUCT
}

// ------------------------------------------------------

void structInputConnection::fieldProperties::setValueToStruct( void *StructPointer, const QVariant &Value ) const
{
  if ( StructPointer == NULL )
    return;

  if ( Type == NULL )
    return;

#define VALUE_FROM_STRUCT( T )          \
  ( valueFromVoidPointer< T >( StructPointer, Shift ) )

#define SET_AND_RETURN_IF_TYPE( T )     \
  do {                                  \
    if ( *Type == typeid(T) )           \
    {                                   \
      VALUE_FROM_STRUCT( T ) = Value.value<T>(); \
      return;                           \
    }                                   \
  } while ( false )

  SET_AND_RETURN_IF_TYPE( bool );
  SET_AND_RETURN_IF_TYPE( char );
  SET_AND_RETURN_IF_TYPE( short );
  SET_AND_RETURN_IF_TYPE( int );
  SET_AND_RETURN_IF_TYPE( unsigned );
  SET_AND_RETURN_IF_TYPE( float );
  SET_AND_RETURN_IF_TYPE( double );
  SET_AND_RETURN_IF_TYPE( QString );

  if ( *Type == typeid(std::string) )
  {
    VALUE_FROM_STRUCT( std::string ) = Value.value<QString>().toStdString();
    return;
  }
  
  qWarning() << "structInputConnection::fieldProperties::setValueToStruct: invalid type for conversion: " << Type->name();

#undef SET_AND_RETURN_IF_TYPE
#undef VALUE_FROM_STRUCT
}

// ======================================================

structInputConnection::structInputConnection()
{
}

// ------------------------------------------------------

void structInputConnection::connectField( inputWidget *Input, size_t Shift, const std::type_info &Type )
{
  if ( Input == NULL )
    return;
  InputConnection[ Input ] = fieldProperties( Shift, Type );
}

// ------------------------------------------------------

void structInputConnection::fillStructFromInputs( void *Struct ) const
{
  if ( Struct == NULL )
    return;

  for ( QMap< inputWidget*, fieldProperties >::const_iterator Iterator = InputConnection.begin(); Iterator != InputConnection.end(); ++Iterator )
  {
    const inputWidget *Input = Iterator.key();
    QVariant Value = Input->getVariantValue();
    const fieldProperties &FieldProperties = Iterator.value();
    FieldProperties.setValueToStruct( Struct, Value );
  }

}

// ------------------------------------------------------

void structInputConnection::fillInputsFromStruct( const void *Struct ) const
{
  if ( Struct == NULL )
    return;
  
  for ( QMap< inputWidget*, fieldProperties >::const_iterator Iterator = InputConnection.begin(); Iterator != InputConnection.end(); ++Iterator )
  {
    const fieldProperties &FieldProperties = Iterator.value();
    QVariant Value = FieldProperties.valueFromStruct( Struct );
    inputWidget *Input = Iterator.key();
    Input->setVariantValue( Value );
  }
}

// ======================================================

