
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
    void moveContent( QLayout *To, QLayout *From );
    void replaceLayout( QWidget *Widget, QLayout *Layout );
    void appendLayoutItem( QWidget *Widget, QLayoutItem *Item, int Stretch = 0 );
    void appendLayoutItem( QBoxLayout *Layout, QLayoutItem *Item, int Stretch = 0 );
    void appendLayoutItem( QBoxLayout *Layout, QWidget *Widget, int Stretch = 0 );
  };
  
  // ======================================================

}

