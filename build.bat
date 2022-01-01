@echo off

set code=%cd%

set link_opts=-opt:ref user32.lib -subsystem:console
set opts=-MT -FC -GR- -EHsc- -nologo -Zi -W4 -WX -D_CRT_SECURE_NO_WARNINGS

if not exist build mkdir build
pushd build
cl %opts% %code%\main.c -Fetetris -link %link_opts%
popd
