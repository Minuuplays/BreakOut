# Breakout Game

A classic Breakout/Arkanoid game clone built with C++ and SDL2.

## Description

This is a recreation of the classic arcade game Breakout, where players control a paddle to bounce a ball and break bricks. The game features smooth gameplay, collision detection, and progressive difficulty.

## Features

- Classic Breakout gameplay mechanics
- Smooth paddle and ball physics
- Brick collision detection
- Score tracking
- Multiple levels (if applicable)
- SDL2-based rendering

## Technologies Used

- **Language:** C++
- **Graphics Library:** SDL2
- **IDE:** Visual Studio
- **Platform:** Windows

## Prerequisites

Before running this project, you need:

- Visual Studio 2019 or later
- SDL2 development libraries
- SDL2_image (if using textures)
- SDL2_ttf (if using custom fonts)
- SDL2_mixer (if using audio)

## Installation

### 1. Clone the Repository

```bash
git clone https://github.com/yourusername/Breakout.git
cd Breakout
```

### 2. Install SDL2

Download SDL2 development libraries from [libsdl.org](https://www.libsdl.org/download-2.0.php)

- Download the Visual C++ development libraries (SDL2-devel-2.x.x-VC.zip)
- Extract to a known location (e.g., `C:/SDL2`)

### 3. Configure Visual Studio

1. Open the `.sln` file in Visual Studio
2. Right-click the project → Properties
3. Under **C/C++ → General → Additional Include Directories**, add:
   - `C:/SDL2/include` (or your SDL2 path)
4. Under **Linker → General → Additional Library Directories**, add:
   - `C:/SDL2/lib/x64` (for 64-bit) or `C:/SDL2/lib/x86` (for 32-bit)
5. Under **Linker → Input → Additional Dependencies**, add:
   - `SDL2.lib`
   - `SDL2main.lib`

### 4. Copy SDL2.dll

Copy `SDL2.dll` from the SDL2 lib folder to your project's output directory (where the .exe is generated).

## How to Build and Run

1. Open the solution file in Visual Studio
2. Set the build configuration (Debug/Release)
3. Build the solution (F7 or Build → Build Solution)
4. Run the project (F5 or Debug → Start Debugging)

## Controls

- **Left Arrow / A:** Move paddle left
- **Right Arrow / D:** Move paddle right
- **Space:** Launch ball / Pause
- **ESC:** Exit game

## Project Structure

```
Breakout/
├── src/              # Source files (.cpp)
├── include/          # Header files (.h)
├── assets/           # Game assets (images, sounds, fonts)
├── SDL2/             # SDL2 libraries (not included in repo)
└── README.md
```

## Gameplay

- Use the paddle to keep the ball in play
- Break all bricks to advance to the next level
- Don't let the ball fall below the paddle
- Earn points for each brick destroyed

## Known Issues

- [List any known bugs or limitations]

## Future Improvements

- [ ] Add power-ups
- [ ] Multiple ball types
- [ ] Sound effects and music
- [ ] Level editor

## Contributing

Feel free to fork this project and submit pull requests for any improvements!

## License

This project is open source and available under the [MIT License](LICENSE).

## Acknowledgments

- SDL2 library by Sam Lantinga
- Inspired by the classic Atari Breakout game

## Contact

TahsinRafi - [tahsinrafi007@gmail.com]

Project Link: [https://github.com/Minuuplays/BreakOut]
