@echo off

if not exist build mkdir build

set compiler_args=^
-MTd ^
-nologo ^
-GR- ^
-EHa- ^
-O2 -Oi ^
-WX -W4 ^
-FC ^
-Zi ^
-diagnostics:caret ^
-wd4201 ^
-wd4100 ^
-wd4505 ^
-wd4189 ^
-wd4146 ^
-wd4324 ^
-LD ^
-IE:/Tools/raylib/src

set files=^
../src/drawing.cpp ^
../src/kmeans.cpp ^
../src/dbscan.cpp ^
../src/hierarchical.cpp

pushd build

cl %compiler_args% %files% /link /OUT:clustering.dll raylib.lib && echo [32mBuild successfull[0m || echo [31mBuild failed[0m

popd

