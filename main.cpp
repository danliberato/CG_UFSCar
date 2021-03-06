// Std. Includes
#include <string>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// GL includes
#include "Shader.h"
#include "Camera.h"
#include "Model.h"

// GLM Mathemtics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Other Libs
#include "SOIL2/SOIL2.h"

// Properties
const GLuint WIDTH = 1280, HEIGHT = 720;
int SCREEN_WIDTH, SCREEN_HEIGHT;
float movement_car_foward = 0.0f;
float rotate_car = 1.6f;
float rotate_road = 0.0f;
float movement_car_right = 0.0f;
// Function prototypes
void KeyCallback( GLFWwindow *window, int key, int scancode, int action, int mode );
void MouseCallback( GLFWwindow *window, double xPos, double yPos );
void DoMovement( );
// Camera
Camera camera( glm::vec3( 7.15f, 0.3f, 0.0f ) );
bool keys[1024];
GLfloat lastX = WIDTH/2, lastY = HEIGHT/2;
bool firstMouse = true;

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

int main(){
	
	double lastTime = glfwGetTime();
	int nbFrames = 0;
    // Init GLFW
    glfwInit();
    // Set all the required options for GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    
    // Create a GLFWwindow object that we can use for GLFW's functions
    GLFWwindow *window = glfwCreateWindow( WIDTH, HEIGHT, "LearnOpenGL", NULL, NULL );
    
    if (NULL == window){
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate( );
        
        return EXIT_FAILURE;
    }
    
    glfwMakeContextCurrent(window);
    
    glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);
    
    // Set the required callback functions
    glfwSetKeyCallback(window, KeyCallback);
    glfwSetCursorPosCallback( window, MouseCallback);
    
    // GLFW Options
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    
    // Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
    glewExperimental = GL_TRUE;
    // Initialize GLEW to setup the OpenGL Function pointers
    if(GLEW_OK != glewInit()){
        std::cout << "Failed to initialize GLEW" << std::endl;
        return EXIT_FAILURE;
    }
    
    // Define the viewport dimensions
    glViewport( 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT );
    
    // OpenGL options
    glEnable( GL_DEPTH_TEST );
    
    // Setup and compile our shaders
    Shader shader( "res/shaders/modelLoading.vs", "res/shaders/modelLoading.frag" );
    
    // Load models
    Model ourModel2("res/models/Ferrari/ferrari-f1-race-car.obj");
    
    // Load models
	Model ourModel1( "res/models/Roads/AIC.obj");
	
	
    // Draw in wireframe
    //glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
    
    glm::mat4 projection = glm::perspective( camera.GetZoom( ), ( float )SCREEN_WIDTH/( float )SCREEN_HEIGHT, 0.1f, 500.0f );
     
    
    // Game loop
	while( !glfwWindowShouldClose( window )){
		
		// Measure speed
		double currentTime = glfwGetTime();
		nbFrames++;
		if ( currentTime - lastTime >= 1.0 ){ // If last prinf() was more than 1 sec ago
		// printf and reset timer
			//printf("Framerate: %f ms\n", 1000.0/double(nbFrames));
			nbFrames = 0;
			lastTime += 1.0;
		}
		
        // Set frame time
        GLfloat currentFrame = glfwGetTime( );
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        
        // Check and call events
        glfwPollEvents( );
        DoMovement( );
        
        // Clear the colorbuffer
        //glClearColor( 0.05f, 0.05f, 0.05f, 1.0f );//dark grey
        glClearColor( 1.0f, 1.0f, 1.0f, 1.0f );//blank
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
        
        shader.Use( );
        
        glm::mat4 view = camera.GetViewMatrix( );
        glUniformMatrix4fv( glGetUniformLocation( shader.Program, "projection" ), 1, GL_FALSE, glm::value_ptr( projection ) );
        glUniformMatrix4fv( glGetUniformLocation( shader.Program, "view" ), 1, GL_FALSE, glm::value_ptr( view ) );
        
        //Draw the loaded model
        glm::mat4 modelTrack,modelCar;
        modelTrack = glm::translate( modelTrack, glm::vec3( movement_car_foward, 0.0f, 0.0f)); // Translate it down a bit so it's at the center of the scene
        modelTrack = glm::rotate(modelTrack, rotate_road, glm::vec3( 0, 1, 0 ));
        modelTrack = glm::scale( modelTrack, glm::vec3( 0.8f, 0.8f, 0.8f));	// It's a bit too big for our scene, so scale it down
        
       
        modelCar = glm::translate( modelCar, glm::vec3(7.0f, 0.0f, 0.0f)); // Translate it down a bit so it's at the center of the scene
        modelCar = glm::rotate(modelCar, rotate_car, glm::vec3( 0, 1, 0 ));
        modelCar = glm::scale( modelCar, glm::vec3( 0.4f, 0.4f, 0.4f));	// It's a bit too big for our scene, so scale it down
        glUniformMatrix4fv( glGetUniformLocation( shader.Program, "model" ), 1, GL_FALSE, glm::value_ptr( modelTrack ) );
        ourModel1.Draw( shader );
        glUniformMatrix4fv( glGetUniformLocation( shader.Program, "model" ), 1, GL_FALSE, glm::value_ptr( modelCar ) );
        ourModel2.Draw( shader );
        
        // Swap the buffers
        glfwSwapBuffers( window );
    }
    
    glfwTerminate( );
    return 0;
}

// Moves/alters the camera positions based on user input
void DoMovement( ){
    // Camera controls
    if(keys[GLFW_KEY_W] || keys[GLFW_KEY_UP]){
		movement_car_foward = movement_car_foward - (SPEED * deltaTime);
		//camera.ProcessKeyboard( FORWARD, deltaTime);
    }
    
    if(keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN]){
		movement_car_foward = movement_car_foward + ((SPEED-8.0f) * deltaTime);
        //camera.ProcessKeyboard( BACKWARD, deltaTime );
    }
    
    if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT]){ 
		//movement_car_foward = movement_car_foward - ((SPEED-8.0f) * deltaTime);   
        rotate_road -= 0.001f;
        if( rotate_road > 89.0f ){
			rotate_road = 89.0f;
        }
		if ( rotate_road < -89.0f ){
			rotate_road= -89.0f;
		}
		
		//camera.ProcessKeyboard( LEFT, deltaTime);
    }
    
    if(keys[GLFW_KEY_1]){
		camera.insideCamera();
	}
	
	if(keys[GLFW_KEY_2]){
		camera.outsideCamera();
	}
    
    if(keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT]){
		//movement_car_foward = movement_car_foward +((SPEED-8.0f) * deltaTime);
        rotate_road += 0.001f;      
		if( rotate_road > 89.0f ){
			rotate_road = 89.0f;
        }
		if ( rotate_road < -89.0f ){
			rotate_road= -89.0f;
		}
		//camera.ProcessKeyboard( RIGHT, deltaTime );
    }
}

// Is called whenever a key is pressed/released via GLFW
void KeyCallback( GLFWwindow *window, int key, int scancode, int action, int mode ){
    if(GLFW_KEY_ESCAPE == key && GLFW_PRESS == action){
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
    
    if(key >= 0 && key < 1024){
        if ( action == GLFW_PRESS ){
            keys[key] = true;
        }
        else if (action == GLFW_RELEASE){
            keys[key] = false;
        }
    }
}

void MouseCallback( GLFWwindow *window, double xPos, double yPos ){
    if (firstMouse){
        lastX = xPos;
        lastY = yPos;
        firstMouse = false;
    }
    
    GLfloat xOffset = xPos - lastX;
    GLfloat yOffset = lastY - yPos;  // Reversed since y-coordinates go from bottom to left
    
    lastX = xPos;
    lastY = yPos;
    
    camera.ProcessMouseMovement(xOffset,yOffset);
}

