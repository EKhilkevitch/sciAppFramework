
#pragma once

// ======================================================

#warning "This file exists only for backward compatibility with old programs"

// ======================================================

#include "sciAppFramework/inputWidget.h"
using namespace sciAppFramework;

typedef labelEditWidget labelEdit;
typedef labelSpinWidget labelSpin;
typedef labelPathEditWidget labelEditPath;
typedef labelComboWidget labelCombo;

#define LABELINPUT_SAVE_SETTINGS( Name ) \
        do { (Name)->setSettingsName(#Name); (Name)->saveSettings( Settings ); } while (false)

#define LABELINPUT_LOAD_SETTINGS( Name, Default ) \
        do { (Name)->setSettingsName(#Name); (Name)->loadSettings( Settings ); } while (false)

// ======================================================

