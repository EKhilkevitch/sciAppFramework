
#pragma once

// ======================================================

#include <QDialog>

// ======================================================

namespace sciAppFramework
{

  // ======================================================

  class okcancelDialog : public QDialog
  {
    Q_OBJECT

    private:
      virtual QString acceptButtonName() const { return "Ok"; }
      virtual QString rejectButtonName() const { return "Cancel"; }
      virtual QWidget* createButtonsWidget();
      virtual QWidget* createContentWidget() = 0;

    protected:
      void initWidget();

    public:
      okcancelDialog( QWidget *Parent = NULL ) : QDialog(Parent) {}
  };
  
  // ======================================================

}

