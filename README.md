To compile: g++ -o exec_name exec.cpp shader.cpp controls.cpp objloader.cpp texture.cpp -lGLEW -lglfw -lGL -lX11 -lpthread -lXrandr -lXi
To execute: ./exec_name 

Commands:
- "D": stretch model on X axis
- "A": compresses model on X axis
- "W": stretch model on Y axis
- "S": compresses model on Y axis
- "E": stretch model on Z axis
- "Q": compresses model on Z axis

- "Left arrow": rotate model on X axis
- "Right arrow": rotate model on X axis
- "Up arrow": rotate model on Y axis
- "Down arrow": rotate model on Y axis
- "Home": rotate model on Z axis
- "End": rotate model on Z axis

- "Space": reset to default position
