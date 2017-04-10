@ECHO off
SET SOURCE=%2
SET SRC_PARSE=%SOURCE:/=\%
SET TARGET=%4
setlocal enabledelayedexpansion
for /F "skip=1 tokens=*" %%a in ('WMIC LOGICALDISK where "volumename like '%TARGET%%%'" get deviceid') do if not defined id set id=%%a
Call Set "deviceid=%%id: =%%"
if not "%deviceid%" == "" (XCOPY %SRC_PARSE% %deviceid% /Y /Q >NUL 
echo Upload complete ) else ( echo %TARGET% not found. Please ensure the device is correctly connected)