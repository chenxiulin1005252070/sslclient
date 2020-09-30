@echo off
@rem mingw test
@call gcc.exe -v 
if %errorlevel% NEQ 0 (
	echo please install MINGW 
	pause & exit
)
@rem cmake test
@call cmake -version
if %errorlevel% NEQ 0 (
	echo please install cmake 
	pause & exit
)

@set BUILD_DIR=build

IF NOT EXIST %BUILD_DIR%( MD %BUILD_DIR% 
)

@cd %BUILD_DIR%
cmake .. -G"MinGW Makefiles"
call make clean
call make
pause & exit

