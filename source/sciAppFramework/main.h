
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
    std::setlocale( LC_NUMERIC, "C" );

    QApplication app(argc,argv);
    widget Widget;
    Widget.show();
    return app.exec();
  }

  // ======================================================
}

// ======================================================

