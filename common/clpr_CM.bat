@echo off
rem clpr_CM.bat rev 1.02
rem new in 1.00 is retreval of info from scratch/env.txt
rem new in 1.01 is moving this file to common dir instead of project dir
rem new in 1.02 is:
rem     name change from everything_common.bat to clpr_CM.bat
rem     pass device type to program.bat so it won't complain

echo %0 INFO: %0 is starting

if not "%1" == "" goto usage
if /I not exist Makefile goto usage

make clean
make

for /f "tokens=1,2 delims= " %%i in (scratch\env.txt) do set %%i=%%j
if %MYFREQ% LSS 1000000 call ..\common\ispfreq.bat set 6.48kHz
if %MYFREQ% GTR 1000000 call ..\common\ispfreq.bat set 125kHz

call ..\common\program.bat %Project%.elf %MYAVR%

goto end

:usage

echo.
echo.
echo ***********************************************************************
echo ********                                                       ********
echo ********               %0.bat usage                   ********
echo ********                                                       ********
echo ***********************************************************************
echo.
echo.
echo ******** description: ********
echo.
echo %0 will Complile your program, Link it, Program the target, and Run the 
echo newly built program on the target.  %0 needs a makefile, from which it
echo will get lots of info.  clpr.bat supports programming of the flash, eeprom,
echo fuses, and lockbits.
echo.
echo.
echo.
echo ******** general usage: ********
echo.
echo %0
echo.
echo.
echo ******** examples: ********
echo.
echo %0
echo.
echo.
echo ******** notes: ********
echo.
echo --%0 is designed to be called by a secondary batch file that resides in
echo a project directory which is adjacent to the directory that %0 is in
echo.
echo.
goto end

:end
echo %0 INFO: %0 is finished