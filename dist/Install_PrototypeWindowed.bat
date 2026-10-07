@echo off
setlocal EnableExtensions
cd /d "%~dp0"

echo [prototype-windowed] Installing...

if not exist "prototype_fix.asi" (
  echo.
  echo ERROR: prototype_fix.asi was not found in this folder.
  echo Install PrototypeFix 1.8 first, then place this installer and
  echo prototype_windowed.asi in the game folder.
  pause
  exit /b 1
)

if not exist "prototype_windowed.asi" (
  echo ERROR: prototype_windowed.asi was not found beside this installer.
  pause
  exit /b 1
)

if not exist "prototype_fix.ini" (
  echo ERROR: prototype_fix.ini was not found. Install PrototypeFix 1.8 first.
  pause
  exit /b 1
)

copy /y "prototype_fix.ini" "prototype_fix.ini.prototype-windowed-backup" >nul

powershell -NoProfile -ExecutionPolicy Bypass -Command ^
  "$p='prototype_fix.ini'; $s=[IO.File]::ReadAllText($p);" ^
  "if($s -match '(?im)^\s*BorderlessWindow\s*='){ $s=[regex]::Replace($s,'(?im)^\s*BorderlessWindow\s*=.*$','BorderlessWindow = true') } else { $s += [Environment]::NewLine + 'BorderlessWindow = true' };" ^
  "if($s -match '(?im)^\s*WindowResolution\.Width\s*='){ $s=[regex]::Replace($s,'(?im)^\s*WindowResolution\.Width\s*=.*$','WindowResolution.Width = 1920') };" ^
  "if($s -match '(?im)^\s*WindowResolution\.Height\s*='){ $s=[regex]::Replace($s,'(?im)^\s*WindowResolution\.Height\s*=.*$','WindowResolution.Height = 1080') };" ^
  "[IO.File]::WriteAllText($p,$s,(New-Object Text.UTF8Encoding($false)))"

if errorlevel 1 (
  echo ERROR: Could not update prototype_fix.ini.
  echo Your backup is prototype_fix.ini.prototype-windowed-backup
  pause
  exit /b 1
)

echo.
echo Installed successfully.
echo - PrototypeFix remains untouched.
echo - BorderlessWindow was enabled automatically.
echo - prototype_windowed.asi will restore normal borders/title bar.
echo.
pause
