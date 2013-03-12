
#pragma once

// ======================================================

#include <list>
#include <string>
#include <QString>

#include "sciAppFramework/inputWidget.h"

class QBoxLayout;
class QLayout;
class QLayoutItem;

// ======================================================

namespace sciAppFramework
{

  class labelComboWidget;

  // ======================================================

  struct layoutOperations
  {
    static bool removeLastSpacing( QBoxLayout *Layout );
    static void appendStretch( QBoxLayout *Layout, int Stretch = 0 );
    static void moveContent( QBoxLayout *To, QBoxLayout *From );
    static void replaceLayout( QWidget *Widget, QLayout *Layout );
    static void appendLayoutItem( QWidget *Widget, QLayoutItem *Item, int Stretch = 0 );
    static void appendLayoutItem( QBoxLayout *Layout, QLayoutItem *Item, int Stretch = 0 );
  };

  // ======================================================

  template <class comportType> inline labelComboWidget* createComboWithAviableComPorts( QWidget *Parent = NULL, const QString &Name = "ComPort", 
    const QString &Label = "COM ports" )
  {
    const std::list< std::string > &AviablePorts = comportType::aviablePorts();
    labelComboWidget *Combo = new labelComboWidget(Label,Parent); 
    Combo->setName(Name);
    for ( std::list< std::string >::const_iterator p = AviablePorts.begin(); p != AviablePorts.end(); ++p )
      Combo->addItem( p->c_str(), QString(p->c_str()) );
    return Combo;
  }

}

// ======================================================
