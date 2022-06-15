@echo off 

Rem Dependancy versions and links
set sdl2_version=2.0.20
set sdl2_dl_link=https://www.libsdl.org/release/SDL2-devel-%sdl2_version%-VC.zip

set sdl2_image_version=2.0.5
set sdl2_image_dl_link=https://www.libsdl.org/projects/SDL_image/release/SDL2_image-devel-%sdl2_image_version%-VC.zip

set sdl2_net_version=2.0.1
set sdl2_net_dl_link=https://www.libsdl.org/projects/SDL_net/release/SDL2_net-devel-%sdl2_net_version%-VC.zip


Rem Download SDL2 for visual studio

powershell.exe "curl %sdl2_dl_link% -o SDL2_temp.zip; Expand-Archive SDL2_temp.zip; Move-Item SDL2_temp/* ./; Remove-Item SDL2_temp; Move-Item SDL2-%sdl2_version% SDL2-vc; Remove-Item SDL2_temp.zip; Copy-Item cmake_configs/sdl2-config.cmake SDL2-vc/sdl2-config.cmake"

powershell.exe "curl %sdl2_image_dl_link% -o SDL2_temp.zip; Expand-Archive SDL2_temp.zip; Move-Item SDL2_temp/* ./; Remove-Item SDL2_temp; Move-Item SDL2_image-%sdl2_image_version% SDL2_image-vc; Remove-Item SDL2_temp.zip; Copy-Item cmake_configs/sdl2_image-config.cmake SDL2_image-vc/sdl2_image-config.cmake"

powershell.exe "curl %sdl2_net_dl_link% -o SDL2_temp.zip; Expand-Archive SDL2_temp.zip; Move-Item SDL2_temp/* ./; Remove-Item SDL2_temp; Move-Item SDL2_net-%sdl2_net_version% SDL2_net-vc; Remove-Item SDL2_temp.zip; Copy-Item cmake_configs/sdl2_net-config.cmake SDL2_net-vc/sdl2_net-config.cmake"