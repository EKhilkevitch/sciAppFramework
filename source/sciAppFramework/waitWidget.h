
#pragma once

// ======================================================

#include <QDialog>

// ======================================================

namespace sciAppFramework
{

// ======================================================

  class waitWidget : public QDialog
  { 
    Q_OBJECT

    private:
      void setWidgetPropertis();

    private:
      virtual QString title() const { return "Please wait..."; }

    protected:
      void initWidget();

    public:
      waitWidget( QWidget *Parent = NULL );
      virtual ~waitWidget() = 0;
  };

// ======================================================

}

