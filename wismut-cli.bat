cmake %~dp0 --preset=windows-release
cmake --build --preset=windows-release
%~dp0\Engine\Tools\CLI\bin\Release\wismut-cli.exe --wismut-path=%~dp0 --location=%cd% %*
