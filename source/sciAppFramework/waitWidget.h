
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
      virtual QString title() const;

    protected:
      void initWidget();

    public:
      explicit waitWidget( QWidget *Parent = NULL );
      virtual ~waitWidget() = 0;
  };

  // ======================================================

}

