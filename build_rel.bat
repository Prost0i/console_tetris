@echo off

set code=%cd%
set build_dir=build_rel

set link_opts=-luser32 -Xlinker /subsystem:console -Xlinker /implib:tetris.lib -Xlinker /pdb:tetris.pdb
set opts=-Wall -Wextra -Werror -pedantic -O3 -MD -MT -D_CRT_SECURE_NO_WARNINGS -D_DLL -D_MT -DNDEBUG  -nostdlib -Xclang --dependent-lib=msvcrt 

if not exist %build_dir% mkdir %build_dir%
pushd %build_dir%
clang %opts% %code%\main.c -o tetris.exe %link_opts%
popd
