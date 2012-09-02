@echo off
echo GETDEVICEINFO.BAT INFO: getDeviceInfo.bat is starting

if "%1" == "set" goto SetFreq
if "%1" == "get" goto GetFreq
goto usage


:usage
echo.
echo.
echo ***********************************************************************
echo ********                                                       ********
echo ********                 getDeviceInfo.bat usage                     ********
echo ********                                                       ********
echo ***********************************************************************
echo.
echo.
echo ******** general usage is: ********
echo.
echo getDeviceInfo.bat [get ^| set [new_freq_value]]
echo.
echo.
echo ******** examples: ********
echo.
echo getDeviceInfo.bat get
echo.
echo getDeviceInfo.bat set 8MHz
echo getDeviceInfo.bat set 4MHz
echo getDeviceInfo.bat set 2MHz
echo getDeviceInfo.bat set 1MHz
echo getDeviceInfo.bat set 500kHz
echo getDeviceInfo.bat set 250kHz
echo getDeviceInfo.bat set 125kHz [This one works fine most of the time]
echo getDeviceInfo.bat set 6.48kHz
echo getDeviceInfo.bat set 100Hz [won't write to flash/eeprom (only fuses)]
echo getDeviceInfo.bat set 52Hz [won't write to flash/eeprom (only fuses]
echo.
echo.
echo ******** notes: ******** 
echo.
echo --you can enter any value for frequency and the mkII will select the
echo next-lowest value automatically
echo.
echo --for slower clock speeds (including clocks slowed down by CLKPR),
echo a slower ISP freq is required
echo.
echo.
goto end

:SetFreq

if "%2" == "" goto usage
set NewFreq=%2
C:\PROGRA~1\Atmel\AVRTOO~1\STK500\stk500.exe -cUSB -I%NewFreq%
goto end

:GetFreq

C:\PROGRA~1\Atmel\AVRTOO~1\STK500\stk500.exe -cUSB -J
goto end

:end
echo GETDEVICEINFO.BAT INFO: getDeviceInfo.bat is finished