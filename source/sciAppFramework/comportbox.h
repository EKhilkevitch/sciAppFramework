
#pragma once

// ======================================================

#include <list>
#include <string>
#include <QString>

#include "sciAppFramework/inputWidget.h"

// ======================================================

namespace sciAppFramework
{
  
  // ======================================================
  
  template <class comportType> inline labelComboWidget* createComboWithAviableComPorts( QWidget *Parent = NULL, const QString &Name = "ComPort", 
    const QString &Label = "COM ports" );
  
  // ======================================================

  template <class comportType> labelComboWidget* createComboWithAviableComPorts( QWidget *Parent, const QString &Name, const QString &Label )
  {
    const std::list< std::string > &AviablePorts = comportType::aviablePorts();
    labelComboWidget *Combo = new labelComboWidget(Label,Parent); 
    Combo->setSettingsName(Name);
    for ( std::list< std::string >::const_iterator p = AviablePorts.begin(); p != AviablePorts.end(); ++p )
      Combo->addItem( p->c_str(), QString(p->c_str()) );
    return Combo;
  }

  // ======================================================

}



