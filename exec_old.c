//header

#include<GL/gl.h>
#include<GL/glut.h>
#include<stdio.h>

//globals
GLdouble obsX, obsY, obsZ, objScaleX, objScaleY, objScaleZ;//Initial position of observer and obj
GLint windowWidth = 800, windowHeight = 600;
GLuint model;
GLfloat fov, fAspect;
//------------ Funcs ----------------\\
//set the default position
void setDefaultPosition(){
	obsX = 0;
	obsY = 0;
	obsZ = 0;
	objScaleX = 0.1;
	objScaleY = 0.1;
	objScaleZ = 0.1;
	fov = 60;
}

//other functions and main

//.obj loader code begins
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

//.obj loader code ends here

void reshape(int w,int h){
	    
	glViewport(0,0,w,h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	fAspect = (GLfloat)w / (GLfloat)h;
    gluPerspective (60, fAspect, 0.1, 1000.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void drawModel(){
	
 	glPushMatrix();
 	glTranslatef(0,-20,-55);//model position on scene
 	glColor3f(1.0,0,0);//model color
 	glScalef(objScaleX, objScaleY, objScaleZ);//model scale
 	glRotatef(obsX,1,0,0);//rotate model on X axis
 	glRotatef(obsY,0,1,0);//rotate model on Y axis
 	glRotatef(obsZ,0,0,1);//rotate model on Z axis
 	glCallList(model);
 	glPopMatrix();
}

void display(void)
{
   	glClearColor (0.0,0.0,0.0,1.0); 
   	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   	glLoadIdentity();
   	drawModel();
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
	//zFar, always positive, is the distance from the observer to the further cut plan.
	gluPerspective(fov,fAspect,0.5,1000);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
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
			objScaleX +=0.01;
			break;
		case 'd' : 
			objScaleX -=0.01;
			break;
		case 'w' : 
			objScaleY +=0.01;
			break;
		case 's' : 
			objScaleY -=0.01;
			break;
		case 'q' : 
			objScaleZ +=0.01;
			break;
		case 'e' : 
			objScaleZ -=0.01;
			break;
		case ' ' : 
			setDefaultPosition();
			break;
	}
	glLoadIdentity();
	glutPostRedisplay();
}

int main(int argc,char **argv){
	
	glutInit(&argc,argv);
	setDefaultPosition();
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB|GLUT_DEPTH);
	glutInitWindowSize(windowWidth, windowHeight);
	glutInitWindowPosition((glutGet(GLUT_SCREEN_WIDTH)-windowWidth) / 2, (glutGet(GLUT_SCREEN_HEIGHT)-windowHeight)/2);
	glutCreateWindow("3D Visualizer");
	glutReshapeFunc(reshape);
	glutMouseFunc(MouseHandler);
	glutSetCursor(GLUT_CURSOR_FULL_CROSSHAIR);
    glutDisplayFunc(display);
	glutIdleFunc(display);
	glutSpecialFunc(specialKeys);
	glutKeyboardFunc(keyPressed);
    loadObj(argv[1]);//replace porsche.obj with radar.obj or any other .obj to display it
	glutMainLoop();
	return 0;
}
