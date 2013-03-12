
#pragma once

// =========================================

#include <QDialog>

// =========================================

namespace sciAppFramework
{

  // =========================================

  class okcancelDialog : public QDialog
  {
    Q_OBJECT

    protected:
      virtual QString acceptButtonName() const { return "Ok"; }
      virtual QString rejectButtonName() const { return "Cancel"; }
      QWidget* createButtonsWidget();

    public:
      okcancelDialog( QWidget *Parent = NULL ) : QDialog(Parent) {}
  };
  
  // =========================================

}

