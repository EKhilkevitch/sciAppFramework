
@echo off

rem =========================================

set ROOT_DIR=%~dp0
set BUILD_DIR=build
set CURRENT_DIR=%CD%

rem =========================================

cd %ROOT_DIR% || goto End

if not exist %BUILD_DIR% ( 
  mkdir %BUILD_DIR% || goto End
)

cd %BUILD_DIR% || goto End
cmake -G "NMake Makefiles" %1 %2 %3 %4 %5 %6 %7 %8 %9 .. || goto End
nmake || goto End

rem =========================================

:End

if %ERRORLEVEL% GTR 0 (
  echo Build failure.
  goto Exit
)

echo.
echo Successfully build
echo.
goto Exit

rem =========================================

:Exit

cd %CURRENT_DIR%

if %ERRORLEVEL% GTR 0 (
  exit /b 1 
) else (
  exit /b 0
)

