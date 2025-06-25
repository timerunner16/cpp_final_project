# C++ 1 Honors Final Project
## Design
This project has several core features that need to be implemented for it to be considered 'finished'.
This list will be updated as the project continues.

First, on the technical side:
- SDL2-based core
- OpenGL based renderer with GLEW for access
- OpenAL based audio with mojoAL for implementation
- Lua embedded into the engine for game scripting with sol2 for integration

Second, on the game side:
- Changing labyrinth dungeon generation (load a room between two hallways, swap map to move into next room)
- ✨ Magic ✨ (several spells that can be learned and swapped between)
- Enemies (berserker/melee, archer/ranged)
- 1993-1996 idSoftware style graphics (DOOM-Quake) using shaders

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

The `Resource Manager` class will handle:
- loading in assets from storage
- accessing assets without unnecessary duplicate loading
- unloading assets from storage when no references are left

The `Input Manager` class will handle:
- managing the states of keyboard keys/mouse buttons
- managing mouse position/delta
- providing tools for easier input math

The `Workspace` class will handle:
- storage of current game data
- processing game objects
- processing events
- processing particle systems

The `Map` class will handle:
- loading a UDMF format map from a WAD file
- parsing the map into
	- sectors/line segments for physics
	- meshes for rendering
	- `GameObject`s to add to the Workspace

Game data will be stored through several different classes that represent different data types.

The `GameObject` class will handle:
- processing its lua script
- processing collisions
and will include:
- a reference to a `Mesh`
- a reference to a `Material`
- a map of child `GameObject`s
- a map of `Event`s
- a `Transform`
- a name
- a reference to a lua script with:
	- an init function (optional)
	- a process function (optional)

The `ParticleSystem` class will handle:
- processing point particles
- setting up buffers for instanced rendering of quads

The `Event` class will handle:
- connecting lua scripts
- triggering connected lua scripts when fired
and will include:
- a map of stored values

The `AudioInstance` class will handle:
- playing of audio tracks
- 3d positioning
- track position and volume control
and will include:
- a reference to an AudioSegment

The `Camera` class will handle:
- constructing a view projection matrix from its values
and will include:
- a `Transform`
- an FOV

The `Mesh` class will include:
- a reference to a Vertex Array Object

The `GLTexture` class will include:
- a reference to a Texture
- a width/height value

The `Material` class will handle:
- setting of `Uniform`s (global and material)
- getting of `Uniform`s
and will include:
- a reference to a `GLTexture`
- a reference to a `Shader`
- a list of `Uniform`s

The `Transform` class will handle:
- constructing a model matrix from its values
and will include:
- a `vec3` position
- a `vec3` pitch, roll, yaw (Euclid Angle) rotation
- a `vec3` scale

The `Shader` class will handle:
- generating Programs from vertex and fragment shaders
- setting shader `Uniform`s
and will include:
- a reference to its Program

The `Uniform` class will include:
- a name
- a data type (INT, VEC3, etc.)
- data matching the data type

The `AudioSegment` class will handle:
- processing a .WAV file into SDL audio
and will include:
- a buffer, audio spec, and audio length in samples

The `vec2` class will include:
- float `x` and `y` values

The `vec3` class will include:
- float `x`, `y` and `z` values

The `ivec2` class will include:
- int `x` and `y` values

The `ivec3` class will include:
- int `x`, `y` and `z` values

## Build Process
### Linux (Ubuntu/Debian)
Tested on Ubuntu 24.04 LTS:

1. Install build requirements
`sudo apt install gcc git cmake libsdl2-dev libsdl2-image-dev libglew-dev libgl-dev liblua5.4-dev`
2. Clone the repository
`git clone https://github.com/timerunner16/cpp_final_project --recursive`
3. Prepare build directory
`mkdir cpp_final_project/build && cd cpp_final_project/build`
4. Create build files
`cmake <OPTIONS> ..`
5. Build the project
`cmake --build .`

### Windows (Visual Studio + vcpkg)
Tested on Windows 11 w/ Visual Studio 2022 v17.14.7

1. Open Visual Studio 2022
2. Clone repo through dashboard
3. Generate CMake files
4. Build/run cpp_final_project.exe target