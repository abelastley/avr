@echo off
rem clpr.bat rev 1.02
rem new in 1.00 is retreval of info from scratch/env.txt
rem new in 1.01 is moving the brains of this operation to ..\common instead of having them in this file
rem new in 1.02 is:
rem     changing own file name from everything.bat to clpr.bat, and
rem     changing name of called file from everything_common.bat to clpr_CM.bat


if not "%1" == "" goto usage
call ..\common\clpr_CM.bat

goto end

:usage
echo.
echo.
echo ***********************************************************************
echo ********                                                       ********
echo ********                  %0 usage                       ********
echo ********                                                       ********
echo ***********************************************************************
echo.
echo.
echo ******** description: ********
echo.
echo %0 will Complile your program, Link it, Program the target, and Run the 
echo newly built program on the target.  %0 is designed to be low-maintenance
echo so that updates can be made to the real script (..\common\clpr_CM.bat) without
echo having to update %0 in every project directory.  See clpr_CM.bat for more
echo info
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
echo --there are no cmd line options, other than this help screen which appears
echo any time anything is entered after the command
echo.
echo.
goto end

:end
echo %0 INFO: %0 just finished