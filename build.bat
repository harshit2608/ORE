mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
cd ..
@REM call scripts/msbuildpath.bat
@REM "%MSBUILDDIR%msbuild.exe" build/ORE.sln /p:Configuration=Release