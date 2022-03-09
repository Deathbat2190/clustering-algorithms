@echo off

if not exist build mkdir build

set compiler_args=^
-MTd ^
-nologo ^
-GR- ^
-EHa- ^
-Od -Oi ^
-WX -W4 ^
-FC ^
-Zi ^
-diagnostics:caret ^
-wd4201 ^
-wd4100 ^
-wd4505 ^
-wd4189 ^
-wd4146 ^
-LD ^
-IE:/Tools/raylib/src

pushd build

cl %compiler_args% ../src/drawing.cpp ../src/kmeans.cpp /link /OUT:clustering.dll raylib.lib && echo [32mBuild successfull[0m || echo [31mBuild failed[0m

popd

