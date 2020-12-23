@ECHO OFF
setlocal EnableDelayedExpansion

for %%f in (tests\rosetta\*.kaos) do (
    ECHO.
    ECHO.
    chaos -c %%f -o %%~nf
    IF errorlevel 1 (
        ECHO Fail
        EXIT /B 1
    ) ELSE (
        ECHO OK
    )
)

for %%f in (tests\rosetta\*.kaos) do (
    ECHO.
    ECHO.
    build\%%~nf.exe > tmpFile
    TYPE tmpFile
    IF NOT "%%~nf" == "string" (
        powershell -Command "(gc tmpFile) -replace '\\', '/' | Out-File -encoding ASCII tmpFile"
    )
    FC tests\rosetta\%%~nf.out tmpFile
    IF errorlevel 1 (
        DEL tmpFile
        ECHO Fail
        EXIT /B 1
    ) ELSE (
        DEL tmpFile
        ECHO OK
    )
)
EXIT /B 0
