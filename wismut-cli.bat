set "WISMUT_ROOT=%~dp0"
set "CALL_LOCATION=%cd%"
pushd %WISMUT_ROOT%

cmake --preset=windows-release
cmake --build --preset=windows-release

popd

%~dp0Build\Release\Windows\bin\wismut-cli.exe --wismut-path=%~dp0 --location=%CALL_LOCATION% %*
