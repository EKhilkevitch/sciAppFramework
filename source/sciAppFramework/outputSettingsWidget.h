
#pragma once

// =========================================

#include <QWidget>

class qt4plot;
class qt4plotManager;

class QStackedLayout;

// =========================================

namespace sciAppFramework
{

  class outputWidget;
  class settingsStorage;

  // =========================================

  class outputSettingsWidget : public QWidget
  {
    Q_OBJECT

    private:
      QStackedLayout* getCurrentLayout();

    public:
      explicit outputSettingsWidget( QWidget *Parent = NULL );
      explicit outputSettingsWidget( outputWidget *OutputWidget, QWidget *Parent = NULL );
      ~outputSettingsWidget();

      void addSettingsWidget( QWidget *Widget );
      void addSettingsWidgets( const QList<QWidget*> &Widgets );

      void saveSettings( settingsStorage* ) {}
      void loadSettings( settingsStorage* ) {}
      
    public slots:
      void setCurrentSettings( int Index );
  };

  // =========================================

}

