
#pragma once

// ======================================================

#include <QMap>

#include <typeinfo>
#include <cstddef>

class QVariant;

// ======================================================

namespace sciAppFramework
{

  // ======================================================
  
  class inputWidget;
  
  // ======================================================

  class structInputConnection
  {
    private:
      class fieldProperties
      {
        private:
          size_t Shift;
          const std::type_info *Type;

        public:
          fieldProperties();
          fieldProperties( size_t Shift, const std::type_info &Type );
          QVariant valueFromStruct( const void *StructPointer ) const;
          void setValueToStruct( void *StructPointer, const QVariant &Value ) const;
      };

      QMap< inputWidget*, fieldProperties > InputConnection;

    public:
      structInputConnection();

      void connectField( inputWidget *Input, size_t Shift, const std::type_info &Type );
      void fillStructByInputs( void *Struct ) const;
      void fillInputsFromStruct( const void *Struct ) const;
  };
  
  // ======================================================

#define STRUCT_INPUT_CONNECT_FIELD( Connection, InputWidget, StructType, FieldName )    \
  do {                                                                                  \
    size_t Shift = offsetof( StructType, FieldName );                                   \
    const std::type_info &Type = typeid( static_cast<StructType*>(NULL)->FieldName );   \
    ( Connection )->connectField( (InputWidget), Shift, Type );                         \
  } while ( false )

  // ======================================================
  
}

