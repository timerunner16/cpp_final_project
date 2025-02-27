# C++ 1 Honors Final Project
## Design
This project has several core features that need to be implemented for it to be considered 'finished'.
This list will be updated as the project continues.

First, on the technical side:
- SDL2-based core
- OpenGL based renderer
- OpenAL based audio
- Lua embedded into the engine for game scripting

Second, on the game side:
- Changing labyrinth dungeon generation (load a room between two hallways, delete once in the next room)
- ✨ Magic ✨ (several spells that can be learned and swapped between) 
- Enemies (berserker/melee, archer/ranged)
- 1996 style DOS graphics (Quake) using shaders

## Engine Architecture
To keep the engine simple, an OOP approach will be taken for each of the engine's components.

The `Game` class will handle: 
- initializing engine components
- handling the game loop
- deleting engine components

The `Window` class will handle:
- initializing & managing the OpenGL context
- integration with SDL2
- providing functions for drawing objects and managing the display

The `Renderer` class will handle:
- determining parts of the scene to render
- sending data to the `Window`

The `Resource Manager` class will handle:
- loading in assets from storage
- unloading assets from storage when no references are left

The `Workspace` class will handle:
- storage of current game data
- running lua game scripts

Game data will be stored through several different classes that represent different data types.

The `GameObject` class will include:
- a reference to mesh data
- a reference to a texture
- a transform
- a reference to a lua script with:
	- an init function (optional)
	- a process function (optional)

The `Camera` class will include:
- a transform
- a FOV
and will handle:
- providing the projection & view matrices to the `Renderer`

The `Mesh` class will include:
- a reference to a Vertex Buffer Object
- a reference to an Index Buffer Object

The `Texture` class will include:
- a reference to a Texture Buffer

## Build Process
This game was designed and programmed on Linux (specifically Ubuntu 24.04), and as such I only have build instructions for Linux.
To build on linux, simply:

1. Install build requirements
`sudo apt install cmake libsdl2-dev libglew-dev libgl libgl-dev liblua5.3 liblua5.3-dev`
2. Clone the repository
`git clone https://github.com/timerunner16/cpp-final-project`
`cd cpp-final-project`
3. Prepare build directory
`mkdir build && cd build`
4. Create build files
`cmake ..`
(plus additional)
5. Build the project
`make`
