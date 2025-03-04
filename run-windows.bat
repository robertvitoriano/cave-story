@echo off
cd "build"

:: Run CMake configuration
cmake ..

:: Build the project
cmake --build .

cd .. 

:: Create "windows-build" folder if it doesn't exist
if not exist "windows-build" mkdir "windows-build"

:: Copy built files from Debug folder to "windows-build"
xcopy /Y "Debug\Cavestory.exe" "windows-build\"

:: Copy "content" folder and its content to "windows-build"
xcopy /E /Y "content" "windows-build\content"

:: Copy "maps" folder and its mapss to "windows-build"
xcopy /E /Y "maps" "windows-build\maps"

:: Copy "maps" folder and its mapss to "windows-build"
xcopy /Y "windows-libraries\lib\SDL2.dll" "windows-build\"
xcopy /Y "windows-libraries\lib\SDL2_image.dll" "windows-build\"
xcopy /Y "windows-libraries\lib\SDL2_mixer.dll" "windows-build\"
xcopy /Y "windows-libraries\lib\SDL2_ttf.dll" "windows-build\"

:: Navigate to "windows-build" and run the game
cd "windows-build"

"Cavestory.exe"
