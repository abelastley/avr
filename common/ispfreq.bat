@echo off
echo %0 INFO: %0 is starting

if defined stkexe (goto stkexe_is_defined) else (goto stkexe_not_defined)
goto unknownerror


:stkexe_not_defined
if exist C:\PROGRA~1\Atmel\AVRTOO~1\STK500\stk500.exe set stkexe=C:\PROGRA~1\Atmel\AVRTOO~1\STK500\stk500.exe
if exist C:\PROGRA~2\Atmel\AVRTOO~1\STK500\stk500.exe set stkexe=C:\PROGRA~2\Atmel\AVRTOO~1\STK500\stk500.exe
if defined stkexe (goto stkexe_is_defined) else (goto stkexe_not_found_error)
goto unknownerror


:stkexe_is_defined
echo stkexe is %stkexe%
if "%1" == "set" goto SetFreq
if "%1" == "get" goto GetFreq
goto usage

:SetFreq

if "%2" == "" goto usage
set NewFreq=%2
rem C:\PROGRA~1\Atmel\AVRTOO~1\STK500\stk500.exe -cUSB -I%NewFreq%
%stkexe% -cUSB -I%NewFreq%
goto end

:GetFreq

rem C:\PROGRA~1\Atmel\AVRTOO~1\STK500\stk500.exe -cUSB -J
%stkexe% -cUSB -J
goto end


:stkexe_not_found_error
echo %0 ERROR: cannot find the application stk500.exe
goto end

:unknownerror
echo %0 ERROR: there was an unknown error while programming the device
goto end


:usage
echo.
echo.
echo ***********************************************************************
echo ********                                                       ********
echo ********                 %0 usage                     ********
echo ********                                                       ********
echo ***********************************************************************
echo.
echo.
echo ******** general usage is: ********
echo.
echo %0 [get ^| set [new_freq_value]]
echo.
echo.
echo ******** examples: ********
echo.
echo ispfreq.bat get
echo.
echo ispfreq.bat set 8MHz
echo ispfreq.bat set 4MHz
echo ispfreq.bat set 2MHz
echo ispfreq.bat set 1MHz
echo ispfreq.bat set 500kHz
echo ispfreq.bat set 250kHz
echo ispfreq.bat set 125kHz [This one works fine most of the time]
echo ispfreq.bat set 6.48kHz
echo ispfreq.bat set 100Hz [won't write to flash/eeprom (only fuses)]
echo ispfreq.bat set 52Hz [won't write to flash/eeprom (only fuses]
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

:end
echo %0 INFO: %0 is finished