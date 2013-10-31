
#pragma once

// ======================================================

#include <QApplication>
#include <QString>

#include <clocale>

// ======================================================

namespace sciAppFramework
{

  // ======================================================

  template < class widget > int main( int argc, char* argv[], const QString &AppName = QString() )
  {
    QApplication app(argc,argv);
    
    std::setlocale( LC_NUMERIC, "C" );
    if ( ! AppName.isEmpty() )
      QApplication::setApplicationName( AppName );
    
    widget Widget;
    Widget.show();
    return app.exec();
  }

  // ======================================================
  
}

// ======================================================

