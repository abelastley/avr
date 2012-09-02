@echo off
rem newproject.bat rev 0.80
rem 0.80 is first attempt
rem as of 8/14/2010, 0.80 has not been tested at all

echo NEWPROJECT.BAT INFO: newproject.bat is starting

if "%1" == "" goto usage

set projname=%1
rem if exist ..\%projname% goto error_projalreadyexists
mkdir ..\%projname%
rem if %ERRORLEVEL% GEQ 1 goto error_cantmkdir
copy ..\template_avr\template_avr.c ..\%projname%\%projname%.c
rem if %ERRORLEVEL% GEQ 1 goto error_noCfile
copy ..\template_avr\clpr.bat ..\%projname%\clpr.bat
rem if %ERRORLEVEL% GEQ 1 goto error_noBATfile
copy ..\template_avr\Makefile MakefileTemplate
rem if %ERRORLEVEL% GEQ 1 goto error_noMAKEfile
perl newMakefile.pl %projname%
rem if %ERRORLEVEL% GEQ 1 goto error_perl
move Makefile ..\%projname%\
rem if %ERRORLEVEL% GEQ 1 goto error_perl
del MakefileTemplate
rem if %ERRORLEVEL% GEQ 1 goto error_cantdelfile
goto end

:error_projalreadyexists
echo.
echo NEWPROJECT.BAT ERROR: the project %projname% already exists, please choose a different name
echo.
goto end

:error_cantmkdir
echo.
echo NEWPROJECT.BAT ERROR: could not create a directory called %projname% in src
echo.
goto end

:error_noCfile
echo.
echo NEWPROJECT.BAT ERROR: could not copy template_acr.c to new directory
echo.
goto end

:error_noBATfile
echo.
echo NEWPROJECT.BAT ERROR: could not copy clpr.bat template to new directory
echo.
goto end

:error_noMAKEfile
echo.
echo NEWPROJECT.BAT ERROR: could not copy makefile template to temp location
echo.
goto end

:error_perl
echo.
echo NEWPROJECT.BAT ERROR: could not create new makefile
echo.
goto end

:error_cantdelfile
echo.
echo NEWPROJECT.BAT ERROR: could not delete MakefileTemplate
echo.
goto end


:usage
echo.
echo.
echo ***********************************************************************
echo ********                                                       ********
echo ********              newproject.bat usage                     ********
echo ********                                                       ********
echo ***********************************************************************
echo.
echo.
echo ******** description: ********
echo.
echo newproject.bat will make a new project directory with the name you specify
echo.
echo.
echo ******** general usage: ********
echo.
echo newproject.bat PROJECT_NAME
echo.
echo.
echo ******** examples: ********
echo.
echo newproject.bat MyProject
echo.
echo.
echo ******** notes: ********
echo.
echo --don't put a .c or anything after PROJECT_NAME
echo.
echo.
goto end

:end
echo NEWPROJECT.BAT INFO: newproject.bat is finished