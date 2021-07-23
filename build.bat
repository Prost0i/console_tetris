@echo off

REM set opts=-MT -FC -GR- -Gm- -EHsc- -nologo -Zi -W4

set link_opts=-opt:ref user32.lib -subsystem:console
set code=%cd%
set opts=-MT -FC -GR- -EHsc- -nologo -Zi -W4 -WX -I"%code%\include" -D_CRT_SECURE_NO_WARNINGS
pushd build
clang-cl %opts% %code%\main.c -Fetetris -link %link_opts%
popd
