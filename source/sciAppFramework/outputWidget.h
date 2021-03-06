
#pragma once

// =========================================

#include <QWidget>

namespace scigraphics
{
  class qt4plotManager;
}

class QTabWidget;

// =========================================

namespace sciAppFramework
{
  
  // =========================================
  
  class settingsStorage;
  
  // =========================================
  
  class outputWidgetItem 
  {
    private:
      QString Title;

    public:
      virtual QWidget* outputWidget() = 0;
      virtual QWidget* outputSettingsWidget() = 0;
      virtual QString title() const { return Title; }

      virtual void saveSettings( settingsStorage* ) {}
      virtual void loadSettings( settingsStorage* ) {}

      outputWidgetItem( const QString &T ) : Title(T) {}
      virtual ~outputWidgetItem() {}
  };
  
  // -----------------------------------------
  
  class simpleOutputWidgetItem : public outputWidgetItem
  {
    private:
      QWidget *OutputWidget;
      QWidget *SettingsWidget;

    public:
      simpleOutputWidgetItem( QWidget *O, QWidget *S, const QString &Title ) :
        outputWidgetItem(Title),
        OutputWidget(O),
        SettingsWidget(S) {}
      ~simpleOutputWidgetItem() {}

      QWidget* outputWidget() { return OutputWidget; }
      QWidget* outputSettingsWidget() { return SettingsWidget; }
  };
  
  // -----------------------------------------
  
  class plotManagerOutputWidgetItem : public outputWidgetItem
  {
    private:
      scigraphics::qt4plotManager *PlotManager;

    public:
      plotManagerOutputWidgetItem( scigraphics::qt4plotManager *PlotManager, const QString &Title );
      QWidget* outputWidget();
      QWidget* outputSettingsWidget();
      void saveSettings( settingsStorage* );
      void loadSettings( settingsStorage* );
  };
  
  // =========================================
  
  class outputWidget : public QWidget
  {
    Q_OBJECT

    private:
      QList< outputWidgetItem* > OutputWidgetItems;

    private:
      QTabWidget* createTabWidget();
      void addToTabWidget( QWidget *Widget, const QString &Title );

    protected:
      void appendOutputWidgetItem( outputWidgetItem *Item );

    public:
      explicit outputWidget( QWidget *Parent = NULL, const QList<outputWidgetItem*> &OutputWidgets = QList<outputWidgetItem*>() );
      ~outputWidget();

      void saveSettings( settingsStorage* );
      void loadSettings( settingsStorage* );

      const QList<QWidget*> listOfSettingsWidgets();

    signals:
      void currentOutputChanged( int Index );
  };

  // =========================================

}

