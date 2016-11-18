
#pragma once

// ======================================================

#include <QDialog>

class QTimer;
class QThread;

// ======================================================

namespace sciAppFramework
{

  // ======================================================

  class waitWidget : public QDialog
  { 
    Q_OBJECT

    private:
      QThread *Thread;

    private:
      void setWidgetPropertis();
      virtual QString title() const;

    private slots:
      void acceptOnThreadDone();

    protected:
      void initWidget();

    public:
      explicit waitWidget( QWidget *Parent = NULL );
      virtual ~waitWidget() = 0;

    public slots:
      int exec();
      int execUntilThreadRun( QThread *Thread );
  };

  // ======================================================

}

