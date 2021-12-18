


# ImStudio
[![Linux Build Status](https://github.com/Raais/ImStudio/actions/workflows/linux.yml/badge.svg)](https://github.com/Raais/ImStudio/actions?workflow=linux)
[![Windows Build Status](https://github.com/Raais/ImStudio/actions/workflows/windows.yml/badge.svg)](https://github.com/Raais/ImStudio/actions?workflow=windows)
[![macOS Build Status](https://github.com/Raais/ImStudio/actions/workflows/macos.yml/badge.svg)](https://github.com/Raais/ImStudio/actions?workflow=macOS)

Real-time GUI layout creator/editor for [Dear ImGui](https://github.com/ocornut/imgui)

![Screenshot with all windows](https://user-images.githubusercontent.com/64605172/140092885-4e1970d6-2518-4c42-a9a0-c14725931aea.png)

![Making a simple layout](https://user-images.githubusercontent.com/64605172/140092697-c7760d6e-6f5a-4dd0-b208-919e9647c117.gif)

Inspired by [Code-Building/ImGuiBuilder](https://github.com/Code-Building/ImGuiBuilder)

## Features

 - Drag edit
 - Property edit
 - Covers most of the commonly used default widgets (primitives, data inputs, and other miscellaneous)
 - Child windows
 - Real-time generation
 - Export to clipboard
 - Useful tools (Style & Color export, Demo Window, etc.)
 - Helpful resources (external)
 
## Installation

### Dependencies

 - [CMake](https://cmake.org/download)
 - [GLFW (Linux only)](https://www.glfw.org/download) \
 <sub><sup> **Ubuntu Linux:** sudo apt-get install libglfw3 libglfw3-dev </sub></sup>\
 <sub><sup> **Arch Linux:** sudo pacman -S glfw </sub></sup>

### Instructions

#### Linux
```bash
git clone --depth 1 https://github.com/Raais/ImStudio.git
cd ImStudio
````
Have a look at create_build.sh file, and change what you want :

- build directory is build (the name is harmless)
- OPENGL_GL_PREFERENCE is defaulted to GLVND (recent distributions will work). If nothing work, try LEGACY.
- if you have admin rights, and REALY WANT to install ImStudio : uncomment the lines starting with sudo in the script

Then do :

````
chmod a+x create_build.sh
./create_build.sh

```
Eric Bachard  17 dec 2021

## Credits
Thanks to [Omar](https://github.com/ocornut) for [Dear ImGui](https://github.com/ocornut/imgui).\
Thanks to [Code-Building](https://github.com/Code-Building) for the inspiration.
