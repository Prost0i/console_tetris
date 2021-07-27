@echo off

REM +------------------+
REM |MSVC Windows build|
REM +------------------+

REM set code=%cd%
REM
REM set link_opts=-opt:ref user32.lib -subsystem:console
REM REM set opts=-MT -FC -GR- -EHsc- -nologo -O2 -W4 -WX -D_CRT_SECURE_NO_WARNINGS
REM set opts=-MT -FC -GR- -EHsc- -nologo -Zi -W4 -WX -D_CRT_SECURE_NO_WARNINGS
REM
REM pushd build
REM cl %opts% %code%\main.c -Fetetris -link %link_opts%
REM popd


REM +-------------------+
REM |Clang Windows build|
REM +-------------------+

set code=%cd%

set link_opts=-luser32 -Xlinker /subsystem:console -Xlinker /implib:tetris.lib -Xlinker /pdb:tetris.pdb
REM set opts=-Wall -Wextra -Werror -pedantic -O3 -MD -MT -D_CRT_SECURE_NO_WARNINGS -D_DLL -D_MT -DNDEBUG  -nostdlib -Xclang --dependent-lib=msvcrt 
set opts=-Wall -Wextra -Werror -pedantic -g -O0 -MD -MT -D_CRT_SECURE_NO_WARNINGS -D_DLL -D_MT -nostdlib -Xclang --dependent-lib=msvcrtd

pushd build
clang %opts% %code%\main.c -o tetris.exe %link_opts%
popd
