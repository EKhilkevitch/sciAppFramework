
// ======================================================

#include "sciAppFramework/stackedSelectorWidget.h"

using namespace sciAppFramework;

// ======================================================

comboMultiWidget::comboMultiWidget( const QString &Label, QWidget *Parent, const QString &SettingsName ) :
  stackedSelectorWidget<labelComboWidget>( Label, Parent, SettingsName )
{
}

// ======================================================

radioMultiWidget::radioMultiWidget( const QString &Label, QWidget *Parent, const QString &SettingsName ) :
  stackedSelectorWidget<radioButtonWidget>( Label, Parent, SettingsName )
{
}

// ======================================================


