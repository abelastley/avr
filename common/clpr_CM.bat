@echo off
rem clpr_CM.bat rev 1.02
rem new in 1.00 is retreval of info from scratch/env.txt
rem new in 1.01 is moving this file to common dir instead of project dir
rem new in 1.02 is:
rem     name change from everything_common.bat to clpr_CM.bat
rem     pass device type to program.bat so it won't complain

echo CLPR_CM.BAT INFO: clpr_CM.bat is starting

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
echo ********               clpr_CM.bat.bat usage                   ********
echo ********                                                       ********
echo ***********************************************************************
echo.
echo.
echo ******** description: ********
echo.
echo clpr_CM.bat will Complile your program, Link it, Program the target, and Run the 
echo newly built program on the target.  clpr_CM.bat needs a makefile, from which it
echo will get lots of info.  clpr.bat supports programming of the flash, eeprom,
echo fuses, and lockbits.
echo.
echo.
echo.
echo ******** general usage: ********
echo.
echo clpr_CM.bat
echo.
echo.
echo ******** examples: ********
echo.
echo clpr_CM.bat
echo.
echo.
echo ******** notes: ********
echo.
echo --clpr_CM.bat is designed to be called by a secondary batch file that resides in
echo a project directory which is adjacent to the directory that clpr_CM.bat is in
echo.
echo.
goto end

:end
echo CLPR_CM.BAT INFO: clpr_CM.bat is finished