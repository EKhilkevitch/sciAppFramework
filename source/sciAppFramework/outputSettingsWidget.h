
#pragma once

// =========================================

#include <QWidget>

class qt4plot;
class qt4plotManager;

class QSettings;
class QStackedLayout;

// =========================================

namespace sciAppFramework
{

  class outputWidget;

  // =========================================

  class outputSettingsWidget : public QWidget
  {
    Q_OBJECT

    private:
      QStackedLayout* getCurrentLayout();

    public:
      outputSettingsWidget( QWidget *Parent = NULL );
      outputSettingsWidget( outputWidget *OutputWidget, QWidget *Parent = NULL );
      ~outputSettingsWidget();

      void addSettingsWidget( QWidget *Widget );
      void addSettingsWidgets( const QList<QWidget*> &Widgets );

      void saveSettings( QSettings* ) {}
      void loadSettings( QSettings* ) {}
      
    public slots:
      void setCurrentSettings( int Index );
  };

  // =========================================

}

