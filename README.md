This is just a simple race based game;
More information in ReadMe.pdf (Brazilian portuguese - PT_BR)

Requirements:

apt-get install cmake make g++ libglew-dev libglfw3 libglfw3-dev libglm-dev libx11-dev libxi-dev libgl1-mesa-dev libglu1-mesa-dev libxrandr-dev libxext-dev libxi-dev libsoil-dev assimp-utils libassimp-dev

To compile: g++ -o exec main.cpp Model.h Shader.h Camera.h -lGLEW -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -lassimp -lSOIL

To execute: ./exec

