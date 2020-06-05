
// ======================================================

#include "sciAppFramework/stackedSelectorWidget.h"

using namespace sciAppFramework;

// ======================================================

comboMultiWidget::comboMultiWidget( const QString &Name, const QString &Label, QWidget *Parent ) :
  stackedSelectorWidget<labelComboWidget>( Name, Label, Parent )
{
}

// ======================================================

radioMultiWidget::radioMultiWidget( const QString &Name, const QString &Label, QWidget *Parent ) :
  stackedSelectorWidget<radioButtonWidget>( Name, Label, Parent )
{
}

// ======================================================


