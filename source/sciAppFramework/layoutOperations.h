
#pragma once

// ======================================================

class QWidget;
class QLayout;
class QLayoutItem;
class QBoxLayout;

// ======================================================

namespace sciAppFramework
{

  // ======================================================

  namespace layoutOperations
  {
    void clearLayout( QLayout *Layout );
    bool removeLastSpacing( QBoxLayout *Layout );
    void appendStretch( QBoxLayout *Layout, int Stretch = 0 );
    void moveContent( QBoxLayout *To, QBoxLayout *From );
    void replaceLayout( QWidget *Widget, QLayout *Layout );
    void appendLayoutItem( QWidget *Widget, QLayoutItem *Item, int Stretch = 0 );
    void appendLayoutItem( QBoxLayout *Layout, QLayoutItem *Item, int Stretch = 0 );
  };
  
  // ======================================================

}

