Requirements:


apt-get install cmake make g++ libglew-dev libglfw3 libglfw3-dev libglm-dev libx11-dev libxi-dev libgl1-mesa-dev libglu1-mesa-dev libxrandr-dev libxext-dev libxi-dev libsoil-dev

To compile: g++ -o exec_name exec.cpp shader.cpp objloader.cpp texture.cpp -lGLEW -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -lSOIL

To execute: ./exec_name

