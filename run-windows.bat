@echo off

if not exist "windows-libraries" mkdir "windows-libraries"
if not exist "windows-libraries/include" mkdir "windows-libraries/include"
if not exist "windows-libraries/include/SDL2" mkdir "windows-libraries/include/SDL2"
if not exist "windows-libraries/include/nlohmann" mkdir "windows-libraries/include/nlohmann"
if not exist "windows-libraries/lib" mkdir "windows-libraries/lib"

:: Download SDL
:: Download and extract SDL

curl -LO https://github.com/libsdl-org/SDL/releases/download/release-2.32.2/SDL2-devel-2.32.2-VC.zip
tar -xf SDL2-devel-2.32.2-VC.zip
del SDL2-*.zip

xcopy /E /Y "SDL2-2.32.2/include" "windows-libraries/include/SDL2"
xcopy /E /Y "SDL2-2.32.2/lib/x64" "windows-libraries/lib"

:: Download and extract SDL_mixer
curl -LO https://github.com/libsdl-org/SDL_mixer/releases/download/release-2.8.1/SDL2_mixer-devel-2.8.1-VC.zip
tar -xf SDL2_mixer-devel-2.8.1-VC.zip
del SDL2_mixer-*.zip

xcopy /E /Y "SDL2_mixer-2.8.1/include" "windows-libraries/include/SDL2"
xcopy /E /Y "SDL2_mixer-2.8.1/lib/x64" "windows-libraries/lib"

:: Download and extract SDL_image
curl -LO https://github.com/libsdl-org/SDL_image/releases/download/release-2.8.8/SDL2_image-devel-2.8.8-VC.zip
tar -xf SDL2_image-devel-2.8.8-VC.zip
del SDL2_image-*.zip

xcopy /E /Y "SDL2_image-2.8.8/include" "windows-libraries/include/SDL2"
xcopy /E /Y "SDL2_image-2.8.8/lib/x64" "windows-libraries/lib"

:: Download and extract SDL_ttf
curl -LO https://github.com/libsdl-org/SDL_ttf/releases/download/release-2.24.0/SDL2_ttf-devel-2.24.0-VC.zip
tar -xf SDL2_ttf-devel-2.24.0-VC.zip
del SDL2_ttf-*.zip

xcopy /E /Y "SDL2_ttf-2.24.0/include" "windows-libraries/include/SDL2"
xcopy /E /Y "SDL2_ttf-2.24.0/lib/x64" "windows-libraries/lib"

:: Download json.hpp into its proper folder

curl -o "windows-libraries/include/nlohmann/json.hpp" -LO https://github.com/nlohmann/json/releases/download/v3.11.3/json.hpp

rmdir /s /q "SDL2-2.32.2"
rmdir /s /q "SDL2_ttf-2.24.0"
rmdir /s /q "SDL2_image-2.8.8"
rmdir /s /q "SDL2_mixer-2.8.1"

echo SDL Libraries downloaded!

if not exist "build" mkdir "build"

cd "build"

:: Run CMake configuration
cmake ..

:: Build the project
cmake --build .

cd .. 

:: Create "windows-build" folder if it doesn't exist
if not exist "windows-build" mkdir "windows-build"

xcopy  /Y "windows-libraries\lib\SDL2_ttf.dll" "windows-build\"
xcopy  /Y "windows-libraries\lib\SDL2_image.dll" "windows-build\"
xcopy  /Y "windows-libraries\lib\SDL2_mixer.dll" "windows-build\"
xcopy  /Y "windows-libraries\lib\SDL2.dll" "windows-build\"

rmdir /s /q "windows-libraries"

:: Copy built files from Debug folder to "windows-build"
xcopy /Y "Debug\Cavestory.exe" "windows-build\"
rmdir /s /q "build"
rmdir /s /q "Debug"

:: Copy "assets" folder and its assetss to "windows-build"
xcopy /E /Y /I "content" "windows-build\content"
xcopy /E /Y /I "maps" "windows-build\maps"

:: Navigate to "windows-build" and run the game
cd "windows-build"

"Cavestory.exe"


