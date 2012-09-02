@echo off
rem program.bat rev 0.95

rem version history
rem 0.91 is flashing the elf file instead of the hex and eep files
rem 0.92...
rem 	intelligent handling of different file types
rem 	for elf file types, auto-discovery of device type
rem 	for hex and eep file types, acceptance of device as argument to cmd
rem 0.93 better handling of elf files when user does or doesn't specify DEVICE_TYPE
rem 0.94 a tiny bit of error handling, in case target CPU is too fast or slow for ISP freq
rem 0.95 more error handling...
rem		exit with errors if word "failed" is anywhere in stk500 output log
rem		tell user if stk500 failed because of a signature mismatch between elf file and actual device

echo PROGRAM.BAT INFO: program.bat is starting


if "%1" == "" goto usage
set progbin=%1
set progdev=%2
set numretries=0

rem this finds out what type of binary you're trying to program...
for /f "usebackq tokens=2 delims==" %%I in (`set progbin`) do set progext=%%~xI

rem if it's an *.elf, user does not NEED to specify device type from cmd line since
rem we can read it from elf file instead...
if "%progext%" == ".elf" goto ElfFileSetup

rem if the binary is not *.elf, user DOES need to specify the device type...
if not defined progdev goto usage

rem now we can branch to the non-elf branches
if "%progext%" == ".hex" goto ProgramHexFile
if "%progext%" == ".eep" goto ProgramEepFile

rem at this point, if we haven't branched somewhere then somethings wrong
goto usage

:ElfFileSetup

rem get device from elf file
set elffiledev=
for /f "usebackq skip=2 tokens=2" %%i in (`avr-readelf -x=.signature %progbin%`) do set sighex=%%i
if "%sighex%" == "06921e" set elffiledev=attiny45
if "%sighex%" == "07901e" set elffiledev=attiny13A

rem now there are 4 possiblilities: we could have a device from (1) the user, (2) the
rem elf file, (3) both, or (4) neither

rem let's branch for cases 2 and 4
if not defined progdev goto case24

rem now we're left with cases 1 and 3 (user has specified device), let's branch for case 3
if defined elffiledev goto case3

rem okay we must be in case 1
echo PROGRAM.BAT INFO: program.bat can't tell which device elf file is signed for so using user-specified device of %progdev%

set elfprogdev=%progdev%
goto ProgramElfFile


:case3

rem we have devices from both user and elf file
rem we're gonna use the user-specified device regardless...
set elfprogdev=%progdev%

rem however, if the 2 devices are different, we should warn the user
if /I "%elffiledev%" == "%progdev%" goto ProgramElfFile
echo PROGRAM.BAT INFO: !!!WARNING!!! USER SPECIFIED DEVICE OF %progdev% BUT ELF FILE IS MEANT FOR DEVICE %elffiledev%
echo program.bat will procede using user-specified device of %progdev%
goto ProgramElfFile

:case24

rem user didn't specify a device so go with whatevers in the elf file if possible
if not defined elffiledev goto case4

rem okay we must be in case 2
echo PROGRAM.BAT INFO: no device specified by user so using device that elf file is signed for which is %elffiledev%

set elfprogdev=%elffiledev%
goto ProgramElfFile

:case4

rem user didn't specify a device and we don't support the device read from elf file
echo PROGRAM.BAT INFO: program.bat doesn't support the device with signature %sighex%
echo if you know the device name you can enter it on the cmd line
echo for more info type program.bat with no arguments
goto end

:ProgramElfFile

C:\PROGRA~1\Atmel\AVRTOO~1\STK500\stk500.exe -cUSB -d%elfprogdev% -s -e -ip%progbin% -pa -va >scratch\stkoutput.txt
goto errorcheck

:ProgramHexFile

C:\PROGRA~1\Atmel\AVRTOO~1\STK500\stk500.exe -cUSB -d%progdev% -s -e -if%progbin% -pf -vf >scratch\stkoutput.txt
goto errorcheck

:ProgramEepFile

C:\PROGRA~1\Atmel\AVRTOO~1\STK500\stk500.exe -cUSB -d%progdev% -s -e -ie%progbin% -pe -ve >scratch\stkoutput.txt
goto errorcheck

:errorcheck

grep -q "failed" scratch\stkoutput.txt
if %ERRORLEVEL% EQU 1 goto end

echo PROGRAM.BAT INFO: programming failed this time, stand by...

grep -q "Signature verification failed" scratch\stkoutput.txt
if %ERRORLEVEL% EQU 0 goto sigerror

grep -q "Could not enter programming mode" scratch\stkoutput.txt
if %ERRORLEVEL% EQU 0 goto progmodeerror

goto unknownerror

:sigerror

echo PROGRAM.BAT ERROR: the device youre trying to program has a different signature than the elf file
goto end

:progmodeerror

set /A numretries=numretries + 1
if %numretries% GTR 3 goto unknownerror

echo PROGRAM.BAT INFO: the device couldn't enter programming mode; trying a different ISP frequency...

call ..\common\ispfreq.bat get >scratch\ispfreqoutput.txt
for /f "usebackq tokens=4,5 delims= " %%I in (`grep "Getting ISP frequency" scratch\ispfreqoutput.txt`) do (
	set ispfreqval=%%I
	set ispfrequnit=%%J
)

if %ispfreqval% GTR 100.0 call ..\common\ispfreq.bat set 6.48kHz
if %ispfreqval% LSS 100.0 call ..\common\ispfreq.bat set 125kHz

if "%progext%" == ".hex" goto ProgramHexFile
if "%progext%" == ".eep" goto ProgramEepFile
if "%progext%" == ".elf" goto ProgramElfFile

:unknownerror

echo PROGRAM.BAT ERROR: there was an unknown error while programming the device
goto end

:usage

echo.
echo.
echo ***********************************************************************
echo ********                                                       ********
echo ********                 program.bat usage                     ********
echo ********                                                       ********
echo ***********************************************************************
echo.
echo.
echo ******** description: ********
echo.
echo program.bat will program all possible memories in an AVR device that are
echo possible with the given FILE_NAME and, if specified, DEVICE_TYPE
echo.
echo.
echo ******** general usage: ********
echo.
echo program.bat FILE_NAME [DEVICE_TYPE]
echo.
echo.
echo ******** examples: ********
echo.
echo program.bat MyProject.elf
echo program.bat MyProject.hex attiny45
echo.
echo.
echo ******** notes: ********
echo.
echo --if FILE_NAME is of type *.elf, DEVICE_TYPE is not needed.  Otherwise, DEVICE_TYPE is needed.
echo.
echo.
goto end

:end
echo PROGRAM.BAT INFO: program.bat is finished