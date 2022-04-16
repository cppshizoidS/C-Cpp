# sort-visualizer
Cross Platform C++/SFML Sorting Visualization

![Example Image](https://i.imgur.com/uDj7wBr.png)

# Algorithms
- Quick sort
- Bubble sort
- Insertion sort
- Selection sort
- Merge sort
- Radix sort
- ...?

# How to use
### Keybinds:
- [1]: Quick sort
- [2]: Bubble sort
- [3]: Insertion sort
- [4]: Selection sort
- [5]: Merge sort
- [6]: Radix sort
- [R]: Shuffle Current Columns
- [G]: Generate New Columns
- [SPACE]: Start sorting
### Custom Array Size:
- Open `main.cpp` on any text editor
- Locate `int COLUMNS_NUMBER = 100`
- Edit the value to whatever you like (numbers higher than 1000 may cause slow rendering)

# How to run
## Requirements
### MacOS
- XCode (https://osxdaily.com/2014/02/12/install-command-line-tools-mac-os-x/)
- Brew (https://brew.sh/)
- [OPTIONAL] Visual Studio Code & C++ Extentions (https://code.visualstudio.com/)
### Linux 
- GCC, make, and other tools can be installed with `sudo apt install build-essential`
- [OPTIONAL] Visual Studio Code & C++ Extentions (https://code.visualstudio.com/)
### Windows
- MSYS2 (https://www.msys2.org/)
- MinGW C++ Compiler (Through MSYS2, `pacman -S --needed base-devel mingw-w64-x86_64-toolchain`)
- Add MinGW to windows PATH environment variable (https://www.computerhope.com/issues/ch000549.htm)
- [OPTIONAL] Visual Studio Code & C++ Extentions (https://code.visualstudio.com/)

## Installation
Install CMake, to test if you have it installed simply type `cmake --version` in your terminal.
#### After CMake has been installed, follow these steps:
- Download/Clone the project
- Open terminal and set the directory as the project folder
- Type `mkdir build` and press enter to create the build folder
- Type `cd build` and press enter to access the build folder
- Type `cmake ..` and press enter to link the files using cmake (`cmake -G "MinGW Makefiles ..` for Windows)
- Type `make` and press enter to compile & build to the `/build` folder (`mingw32-make.exe` for Windows)
- Type `./a` and press enter to run the program
- Done!

# Not working?
For Windows: make sure the msys installation path is correct in `CMakeLists.txt`
For further help, information, and requirements visit the github of the project template I used: https://github.com/lutrarutra/SFML-Project-Template

# Extra
## SFML
This project uses the SFML library version 2.5.1.
If you wish to update SFML you can download the latest version directly from the github (https://github.com/SFML/SFML) and replace the "SFML-2.5.1" folder with that version

## Credits
Skeleton project folder by lutrarutra https://github.com/lutrarutra/SFML-Project-Template

## To Do:
- Stop sorting mid-sorting
- N. of columns as user input
- More algorithms?
