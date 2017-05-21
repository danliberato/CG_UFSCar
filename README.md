Requirements:

apt-get install libglew-dev
apt-get install libglfw3
apt-get install libglfw3-dev
apt-get install libglm-dev
apt-get install cmake
apt-get install make
apt-get install g++
apt-get install libx11-dev
apt-get install libxi-dev 
apt-get install libgl1-mesa-dev 
apt-get install libglu1-mesa-dev 
apt-get install libxrandr-dev 
apt-get install libxext-dev 
apt-get install libxi-dev

To compile: g++ -o exec_name exec.cpp shader.cpp objloader.cpp texture.cpp -lGLEW -lglfw -lGL -lX11 -lpthread -lXrandr -lXi
To execute: ./exec_name 

