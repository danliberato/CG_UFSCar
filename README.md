Requirements:

apt-get install libglew-dev
apt-get install libglfw3
apt-get install libglfw3-dev
apt-get install libglm-dev

To compile: g++ -o exec_name exec.cpp shader.cpp objloader.cpp texture.cpp -lGLEW -lglfw -lGL -lX11 -lpthread -lXrandr -lXi
To execute: ./exec_name 
