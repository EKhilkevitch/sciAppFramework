
#pragma once

// ======================================================

#include <QDialog>

// ======================================================

namespace sciAppFramework
{

  // ======================================================

  class okcancelDialog : public QDialog
  {
    protected:
      virtual QString acceptButtonName() const;
      virtual QString rejectButtonName() const;
      virtual QWidget* createButtonsWidget();
      virtual QWidget* createContentWidget() = 0;

      virtual bool acceptEnabled();
      virtual bool rejectEnabled();

      void initWidget();

    public:
      explicit okcancelDialog( QWidget *Parent = NULL );
      virtual ~okcancelDialog() = 0;

      void accept();
      void reject();
  };
  
  // ======================================================

}

