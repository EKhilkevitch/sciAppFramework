
#pragma once

// =========================================

#include <QString>
#include <QVariant>

// =========================================

namespace sciAppFramework
{

  // =========================================

  class measurementParameters 
  {
    protected:
      virtual QVariant getVariantValue( const QString &Name ) const = 0;

    public:
      template <class T> T value( const QString &Name ) const;
      template <class T> T value( const QString &Name, const T &Default ) const;
      virtual ~measurementParameters() = 0;
  };

  // =========================================
  
  template <class T> T measurementParameters::value( const QString &Name ) const
  {
    return getVariantValue(Name).value<T>(); 
  }
  
  // -----------------------------------------
  
  template <class T> T measurementParameters::value( const QString &Name, const T &Default ) const
  {
    const QVariant &Value = getVariantValue(Name);
    return ( Value.isValid() && Value.canConvert<T>() ) ? Value.value<T>() : Default; 
  }
  
  // =========================================

}

