@echo off

if not exist build mkdir build

set compiler_args=^
-fcolor-diagnostics ^
-MT ^
-nologo ^
-GR- ^
-EHa- ^
-Od -Oi ^
-WX -W4 ^
-FC ^
-Zi ^
-Wno-writable-strings ^
-Wno-unused ^
-Wno-unused-parameter ^
-Wno-unused-variable ^
-DSLOW ^
-LD ^
-IE:/raylib/src

pushd build

clang-cl %compiler_args% ../src/test.cpp raylib.lib && echo [32mBuild successfull[0m || echo [31mBuild failed[0m

popd

