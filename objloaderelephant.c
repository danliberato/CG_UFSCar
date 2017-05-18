//header


#define STB_IMAGE_IMPLEMENTATION

#include<GL/gl.h>
#include<GL/glut.h>
#include"thirdpartysrc/stb_image.h"
#include<stdio.h>

//globals
GLdouble obsX=20, obsY=50, obsZ=0; //Initial position of obj
GLuint model;
GLfloat angle =0, fAspect;
char ch='1';

//------------ Funcs -------------- \\

//read what key was pressed and moves the obj according to it
void SpecialKeys(int key, int x, int y){
	switch (key) {
		case GLUT_KEY_LEFT : 
			obsY +=10;
			break;
		case GLUT_KEY_RIGHT : 
			obsY -=10;
			break;
		case GLUT_KEY_UP : 
			obsX -=10;
			break;
		case GLUT_KEY_DOWN : 
			obsX +=10;
			break;
		case GLUT_KEY_HOME : 
			obsZ +=10;
			break;
		case GLUT_KEY_END : 
			obsZ -=10;
			break;
		case GLUT_KEY_PAGE_UP : 
			obsZ +=10;
			break;
		case GLUT_KEY_PAGE_DOWN : 
			obsZ -=10;
			break;
	}
	glLoadIdentity();
	//gluLookAt(obsX,obsY,obsZ, 0,0,0, 0,1,0);
	glutPostRedisplay();
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
	glTranslatef(0,0,-5);
 	glColor3f(1.0,2.23,0.27);
 	glScalef(1,1,1);
 	glRotatef (obsX, 1,0,0);  // Up and down arrow keys 'tip' view.
	glRotatef (obsY, 0,1,0);  // Right/left arrow keys 'turn' view.
	glRotatef (obsZ,0,0,1);
 	glCallList(model);
 	glPopMatrix();
 	//modelrot=modeltrot+0.6;
 	//if(modelrot>360)modeltrot = modeltrot-360;
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
	// (eyex,eyey,eyez, centerx,centery,centerz, upx,upy,upz)
	gluLookAt(obsX,obsY,obsZ, 0,0,0, 0,0,0);
}

// Specifies the view volume
void setProjection(void)
{
	// Specifies projection coordinates
	glMatrixMode(GL_PROJECTION);
	// Initialize projection coordinates
	glLoadIdentity();
	
	// Especifica a projeção perspectiva(angulo,aspecto,zMin,zMax)
	//fovy é o ângulo, em graus, na direção y (usada para determinar a "altura" do volume de visualização); 
	//aspect é a razão de aspecto que determina a área de visualização na direção x, e seu valor é a razão em x (largura) e y (altura); 
	//zNear, que sempre tem que ter um valor positivo, é a distância do observador até o plano de corte mais próximo (em z);
	//zFar, que também sempre tem que ter um valor positivo, é a distância do observador até o plano de corte mais afastado (em z).
	gluPerspective(angle,fAspect,0.5,1000);

	ViewerPosition();
}

void MouseHandler(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON)
		if (state == GLUT_DOWN) {  // Zoom-in
			if (angle >= 10) angle -= 5;
		}
	if (button == GLUT_RIGHT_BUTTON)
		if (state == GLUT_DOWN) {  // Zoom-out
			if (angle <= 130) angle += 5;
		}
	setProjection();
	glutPostRedisplay();
}
void WindowSizeChange(int w,int h){
	// Para previnir uma divisão por zero
	if ( h == 0 ) h = 1;
    
	glViewport(0,0,w,h);
	
	// Calcula a correção de aspecto
	fAspect = (GLfloat)w/(GLfloat)h;
	
	setProjection();
	//glOrtho(-25,25,-2,2,0.1,100);	
	//glMatrixMode(GL_MODELVIEW);
}

// Inicializa parâmetros de rendering
void initBackground (void){ 
	//vai limpar a tela e pintar de preto.
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	angle=60;
}

int main(int argc,char **argv){
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_DOUBLE);
	glutInitWindowSize(800,450);
	glutInitWindowPosition((glutGet(GLUT_SCREEN_WIDTH)-640)/2,
		(glutGet(GLUT_SCREEN_HEIGHT)-480)/2);
	glutCreateWindow("Vizualizador 3D");
	loadObj("data/Avent.obj");
	glutMouseFunc(MouseHandler);
	glutSetCursor(GLUT_CURSOR_FULL_CROSSHAIR);
	glutDisplayFunc(display);
	glutReshapeFunc(WindowSizeChange);
	glutSpecialFunc(SpecialKeys);

	initBackground();
	//glutIdleFunc(display);
    
	glutMainLoop();
	return 0;
}
