@echo off

REM set opts=-MT -FC -GR- -Gm- -EHsc- -nologo -Zi -W4

REM set link_opts=-opt:ref user32.lib -subsystem:console
REM set code=%cd%
REM set opts=-MT -FC -GR- -EHsc- -nologo -Zi -W4 -WX -I"%code%\include" -D_CRT_SECURE_NO_WARNINGS

set link_opts=-luser32 -Xlinker /subsystem:console -Xlinker /implib:tetris.lib -Xlinker /pdb:tetris.pdb
set code=%cd%

REM set opts=-Wall -Wextra -Werror -pedantic -O3 -I"%code%\include" -D_DLL -D_MT -nostdlib -Xclang --dependent-lib=msvcrt -DNDEBUG -MD -MT -D_CRT_SECURE_NO_WARNINGS
set opts=-Wall -Wextra -Werror -pedantic -g -O0 -I"%code%\include" -MD -MT -D_DLL -D_MT -D_CRT_SECURE_NO_WARNINGS -nostdlib -Xclang --dependent-lib=msvcrtd  

pushd build
REM clang-cl %opts% %code%\main.c -Fetetris -link %link_opts%
clang %opts% %code%\main.c -o tetris.exe %link_opts%
popd
