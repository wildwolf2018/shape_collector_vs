# shape_collector_vs
A video game where the aim is to collect basic geometry shapes at each level

The libraries and headers needed to build the game in Visual Studio availble from the folder External Libraries at https://drive.google.com/open?id=13jxca0WcykwURvGFxKbihLtS7TnMOBwp

The following settings to should applied in Visual Studio 15 or later. The External Libraries folder should be placed in the same directory as the .sln file.

Headers Locations

$(SolutionDir)/External Libraries/irrKlang/irrKlang-64bit-1.5.0/include;
$(SolutionDir)/External Libraries/FREETYPE/freetype-windows-binaries-master/include;
$(SolutionDir)/External Libraries/SOIL/Simple OpenGL Image Library/src;
$(SolutionDir)/External Libraries/ASSIMP/Assimp/include;
$(SolutionDir)/External Libraries/ASSIMP/assimp-4.1.0/include;
$(SolutionDir)/External Libraries/GLM/glm-0.9.8.4/glm;
$(SolutionDir)/External Libraries/GLFW/glfw-3.2.1.bin.WIN64/include;
$(SolutionDir)/External Libraries/glew/include

Libraries' Locations

$(SolutionDir)/External Libraries/GLFW/glfw-3.2.1.bin.WIN64/lib-mingw-w64;
$(SolutionDir)/External Libraries/GLFW/glfw-3.2.1.bin.WIN64/lib-vc2015;
$(SolutionDir)/External Libraries/irrKlang/irrKlang-64bit-1.5.0/lib/Winx64-visualStudio;
$(SolutionDir)/External Libraries/FREETYPE/freetype-windows-binaries-master/win64;
$(SolutionDir)/External Libraries/SOIL/Simple OpenGL Image Library/projects/VC9/x64/Debug;
$(SolutionDir)/External Libraries/ASSIMP/Assimp/code/Debug;
$(SolutionDir)/External Libraries/GLEW/glew-1.5.5/lib;
$(SolutionDir)/External Libraries/GLEW/glew-1.5.5

The Fonts, Sounds, Shaders, Header Files, Source Files,Textures folders, freetype.dll, freetype.lib, assimp-v140-mt.dll, glew32.lib, glew32s.lib, ikpFlac.dll, ikpMP3.dll, irrKlang.dll, 03.MemoryPlayback.ilk and soil2-debug.lib libaries should be placed in the same folder as the .vcxproj file.
