@echo off

set code=%cd%

set link_opts=-opt:ref user32.lib -subsystem:console
set opts=-MT -FC -GR- -EHsc- -nologo -O2 -W4 -WX -D_CRT_SECURE_NO_WARNINGS

if not exist build_rel mkdir build_rel
pushd build_rel
cl %opts% %code%\main.c -Fetetris -link %link_opts%
popd
