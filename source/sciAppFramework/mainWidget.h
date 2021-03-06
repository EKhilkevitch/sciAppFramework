
#pragma once

// =========================================

#include <QMainWindow>

// =========================================

namespace sciAppFramework
{
  
  // =========================================

  class settingsStorage;
  class outputWidget;
  class controlWidget;
  class outputSettingsWidget;

  // =========================================

  class mainWidget : public QMainWindow
  {
    private:
      outputWidget *OutputWidget;
      outputSettingsWidget *OutputSettingsWidget;
      controlWidget *ControlWidget;

      QString LastFileName;

    private:
      static settingsStorage* createSettingsObject();

      virtual controlWidget* createControlWidget() { return NULL; }
      virtual outputWidget* createOutputWidget() { return NULL; }
      virtual outputSettingsWidget* createOutputSettingsWidget();
      virtual QString appTitle() const;
      virtual QIcon appIcon() const;
      
    protected:
      virtual void doInitWidget();
      
      virtual void doSaveSettings( settingsStorage *Settings );
      virtual void doLoadSettings( settingsStorage *Settings );
      
      virtual void setupOutputWidget();
      virtual void setupOutputSettingsWidget();
      virtual void setupControlWidget();
      virtual void setupThisWindow();

    protected:
      void initWidget();
      
      template <class output> output* mainOutputWidget() { return dynamic_cast< output* >( OutputWidget ); }
      template <class control> control* mainControlWidget() { return dynamic_cast< control* >( ControlWidget ); }

      QString getSaveFileName( const QString &Filter, const QString &DefaultSuffix );
      QString getSaveDirectory();
      QString getOpenFileName( const QString &Filter );

      virtual void showErrorMessage( const QString &Message );

      void setControlWidgetInCorners( bool OnCorners );

    public:
      mainWidget();
      virtual ~mainWidget() = 0;

      void saveSettings();
      void loadSettings();
  };

  // =========================================

}

