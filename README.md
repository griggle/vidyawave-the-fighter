# Vidya-wave, the Fighter

## Building on Windows
[CMake](https://cmake.org/) and a C++ compiler are required. [Visual Studio 20XX](https://visualstudio.microsoft.com/) is the easiest way to get both of these on windows, but alternatives include [MinGW](https://www.mingw-w64.org/) and the [WSL](https://docs.microsoft.com/en-us/windows/wsl/install). 

1. Clone the repository.
2. Run `deps/fetch_windows_deps.bat`.
3. Build with your CMake build method of choice.

## Building on Linux
1. Install requirements:
   - Debian / Ubuntu: `sudo apt install gcc make cmake libsdl2-dev`
   - Arch / Manjaro: `sudo pacman -Syuu gcc make cmake sdl2`
2. Build:
```bash
git clone https://github.com/griggle/vidyawave-the-fighter
cd vidyawave-the-fighter
mkdir build
cmake .. && make
```