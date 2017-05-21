//header

#define STB_IMAGE_IMPLEMENTATION

#include <GL/gl.h>
#include <GL/glut.h>
#include "thirdpartysrc/stb_image.h"
#include <stdio.h>
#include <string.h>
//globals
GLdouble obsX, obsY, obsZ, objScaleX, objScaleY, objScaleZ;//Initial position of observer and obj
GLint windowWidth = 800, windowHeight = 450;
GLuint model;
GLfloat fov, fAspect;

//------------ Funcs ----------------\\

//set the default position
void setDefaultPosition(){
	obsX = 0;
	obsY = 0;
	obsZ = 0;
	objScaleX = 1;
	objScaleY = 1;
	objScaleZ = 1;
	fov = 60;
}

//open the obj file and populates the matrix
void loadObj(char *fname){
	FILE *fp;
	int read;
	GLfloat x, y, z;
	char ch;
	model=glGenLists(1);
	fp=fopen(fname,"r");
	if (!fp) {
		printf("can't open file %s\n", fname);
		exit(1);
	}
	glPointSize(2.0);
	glNewList(model, GL_COMPILE);
	{
		glPushMatrix();
		glBegin(GL_POINTS);
		while(!(feof(fp))){
			read=fscanf(fp,"%c %f %f %f",&ch,&x,&y,&z);
			if(read==4&&ch=='v'){
				glVertex3f(x,y,z);
			}
		}
		glEnd();
	}
	glPopMatrix();
	glEndList();
	fclose(fp);
}


//draw the obj according to translation, scale and rotation
void draw(){
 	glPushMatrix();
	glTranslatef(0,-40,-105);
 	glColor3f(1.0,2.23,0.27);
 	glScalef(objScaleX, objScaleY, objScaleZ);
 	glRotatef (obsX, 1,0,0);  // Up and down arrow keys 'tip' view.
	glRotatef (obsY, 0,1,0);  // Right/left arrow keys 'turn' view.
	glRotatef (obsZ,0,0,1);
 	glCallList(model);
 	glPopMatrix();
}

//shows view
void display(void){  

   	glClear (GL_COLOR_BUFFER_BIT);
   	glLoadIdentity();
   	draw();
   	glutSwapBuffers(); //swap the buffers
}

//defines the position of the observer and model
void ViewerPosition(void)
{
	// Specifies model coordinates
	glMatrixMode(GL_MODELVIEW);
	// Initialize model coordinates
	glLoadIdentity();
	// Specifies observer and target position
	// (eyeX,eyeY,eyeZ, centerX,centerY,centerZ, upX,upY,upZ)
	gluLookAt(obsX,obsY,obsZ, 0,0,0, 0,1,0);
}

// Specifies the view volume
void setProjection(void)
{
	// Specifies projection coordinates
	glMatrixMode(GL_PROJECTION);
	// Initialize projection coordinates
	glLoadIdentity();
	
	// Specifies the projection perspective (angle,aspect,zNear,zFar)
	//fovY is the angle, in degrees, on Y plan (it is used to set the "height" of the view volume);
	//aspect determinas the view area on X plan, its value is determined by X and Y ratio; 
	//zNear, always positive, is the distance from de observer to the closer cut plan;
	//zFar, always positive, is the distance from the observer to the further.
	gluPerspective(fov,fAspect,0.5,1000);

	ViewerPosition();
}

//manage the events from mouse
void MouseHandler(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON)
		if (state == GLUT_DOWN) {  // Zoom-in
			if (fov >= 10) fov -= 5;
		}
	if (button == GLUT_RIGHT_BUTTON)
		if (state == GLUT_DOWN) {  // Zoom-out
			if (fov <= 130) fov += 5;
		}
	setProjection();
	glutPostRedisplay();
}


//read what key was pressed and moves the obj according to it
void specialKeys(int key, int x, int y){
	switch (key) {
		case GLUT_KEY_LEFT : 
			obsY +=10;
			break;
		case GLUT_KEY_RIGHT : 
			obsY -=10;
			break;
		case GLUT_KEY_UP : 
			obsX +=10;
			break;
		case GLUT_KEY_DOWN : 
			obsX -=10;
			break;
		case GLUT_KEY_HOME : 
			obsZ +=10;
			break;
		case GLUT_KEY_END : 
			obsZ -=10;
			break;
	}
	glLoadIdentity();
	glutPostRedisplay();
}


//read alphanumeric inputs from keyboard
void keyPressed(unsigned char key, int x, int y) {  
	switch (key) {
		case 'a' : 
			objScaleX +=0.1;
			break;
		case 'd' : 
			objScaleX -=0.1;
			break;
		case 'w' : 
			objScaleY +=0.1;
			break;
		case 's' : 
			objScaleY -=0.1;
			break;
		case 'q' : 
			objScaleZ +=0.1;
			break;
		case 'e' : 
			objScaleZ -=0.1;
			break;
		case ' ' : 
			setDefaultPosition();
			break;
	}
	glLoadIdentity();
	glutPostRedisplay();
}


void WindowSizeChange(int w,int h){
	// Prevines division by zero
	if ( h == 0 ) h = 1;
    
	glViewport(0,0,w,h);
	
	// Aspect correction
	fAspect = (GLfloat)w/(GLfloat)h;
	
	setProjection();
}

// Initialize render parameters
void initBackground (void){ 
	//cleans the screen and set the object angle
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

int main(int argc,char **argv){
	if(argc < 2){
		printf("Error! Correct use: ./exec \"path/modelFile.obj\"");
	}
	else{
		setDefaultPosition();
		glutInit(&argc,argv);
		glutInitDisplayMode(GLUT_DOUBLE);
		glutInitWindowSize(windowWidth,windowHeight);
		glutInitWindowPosition((glutGet(GLUT_SCREEN_WIDTH)-windowWidth) / 2, (glutGet(GLUT_SCREEN_HEIGHT)-windowHeight)/2);
		glutCreateWindow("3D Visualizer");
		loadObj(argv[1]);
		glutMouseFunc(MouseHandler);
		glutSetCursor(GLUT_CURSOR_FULL_CROSSHAIR);
		glutDisplayFunc(display);
		glutReshapeFunc(WindowSizeChange);
		glutSpecialFunc(specialKeys);
		glutKeyboardFunc(keyPressed);
		glutIdleFunc(display);

		initBackground();
		
		glutMainLoop();
	}
	return 0;
}