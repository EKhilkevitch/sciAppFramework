

#pragma once

// =========================================

#include <QWidget>
#include <QGroupBox>

#include "sciAppFramework/measurementThread.h"

class QSettings;

// =========================================

namespace sciAppFramework
{

  class multiInputWidget;
  
  // =========================================

  class parametersWidget : public QGroupBox, public measurementParameters
  {
    Q_OBJECT

    private:
      multiInputWidget *MultiInputWidget;

    private:
      QString settingsGroupName();
      
    public:
      parametersWidget( QWidget *Parent, multiInputWidget *MIW );
      ~parametersWidget();
      
      void saveSettings( QSettings *Settings );
      void loadSettings( QSettings *Settings );

      QVariant getVariantValue( const QString &Name ) const;

    signals:
      void changed();
  };

  // =========================================

}

