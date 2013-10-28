
#pragma once

// ======================================================

#include <QApplication>
#include <clocale>

// ======================================================

namespace sciAppFramework
{

  // ======================================================

  template < class widget > int main( int argc, char* argv[] )
  {
    QApplication app(argc,argv);
    
    std::setlocale( LC_NUMERIC, "C" );
    
    widget Widget;
    Widget.show();
    return app.exec();
  }

  // ======================================================
  
}

// ======================================================

