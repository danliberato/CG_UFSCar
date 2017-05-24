#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <SOIL/SOIL.h>
 
GLfloat angle = 0.0;       // Current turn angle.
GLfloat angle_step = 0.1;  // How much the angle changes between frames.
GLfloat angle_step2 = 0.1;
GLfloat change = 0.1;      // How much angle_step changes when arrows pressed.
GLuint texture[1];
 
GLfloat vertices[][3] =
  {  {1.0, 1.0, 0.0},
     {1.5, 1.0, 0.0},
     {1.5, -1.0, 0.0},
     {1.0, -1.0, 0.0},
     {1.5, 0.5, 0.0},
     {-1.5, 0.5, 0.0},
     {-1.5, 1.0, 0.0},
     {-1.0, 1.0, 0.0},
     {-1.0, -1.0, 0.0},
     {-1.5, -1.0, 0.0},
     {-1.5, -0.5, 0.0},
     {1.5, -0.5, 0.0},
     {-1.5, 1.0, -0.25},
     {1.5, 1.0, -0.25},
     {-0.75, 0.25, -2.0},
     {0.75, 0.25, -2.0},
     {-0.75, -1.0, -2.0},
     {0.75, -1.0, -2.0},
     {1.5, 1.0, -0.25},
     {1.5, -1.0, -0.25},
     {-1.5, -1.0, -0.25},
     {-1.5, 1.0, -0.25},
     {-1.0, 0.5, -0.2},
     {-1.0, -0.5, -0.2},
     {1.0, -0.5, -0.2},
     {1.0, 0.5, -0.2},
     {-1.25, -1.0, 0.5},
     {1.25, -1.0, 0.5},
     {1.25, -1.0, 1.5},
     {-1.25, -1.0, 1.5}
 
     };
 
GLfloat colors[][3] =
  {  {1.0, 1.0, 1.0},
     {0.0, 0.0, 0.0},
  };
 
  FILE *File=NULL;                           // File Handle
 
 
 
 
 
int LoadGLTextures()                           // Load Bitmaps And Convert To Textures
{
  bool Status= false ;                           // Status Indicator
 
               // Create Storage Space For The Texture
 
                // Set The Pointer To NULL
texture[1] = SOIL_load_OGL_texture
	(
		"textures/lego_face.png",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
	);
 
	if( 0 == texture[1] )
{
	printf( "SOIL loading error: '%s'\n", SOIL_last_result() );
}
 
  // Load The Bitmap, Check For Errors, If Bitmap's Not Found Quit
  if ( texture[1] = SOIL_load_OGL_texture
	(
		"textures/lego_face.png",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
	))
  {
     Status=true;                           // Set The Status To TRUE
 
     glGenTextures(1, &(texture[0]));               // Create The Texture
 
     // Typical Texture Generation Using Data From The Bitmap
     glBindTexture(GL_TEXTURE_2D, texture[0]);
 
     glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
     glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
  }
 
 
 
                           return Status;// Return The Status
 
}
 
void init(void)
{
  glClearColor(0.0, 0.0, 0.5, 1.0);
 
  /*if (!LoadGLTextures())                        // Jump To Texture Loading Routine ( NEW )
  {
     return false;                           // If Texture Didn't Load Return FALSE
  }*/
  LoadGLTextures();
  glEnable(GL_TEXTURE_2D);
}
 
void face4(int v0, int v1, int v2, int v3, int c)
  /* Render face with 4 vertices v1,v2,v3,v4 and colour c. */
{
  glColor3fv(colors[c]);
  glBegin(GL_POLYGON);
     glVertex3fv(vertices[v0]);
     glVertex3fv(vertices[v1]);
     glVertex3fv(vertices[v2]);
     glVertex3fv(vertices[v3]);
  glEnd();
}
 
void cable(int x1, int y1, int z1, int x2, int y2, int z2)
{
glColor3f(0.0, 0.0, 0.0);
glBegin(GL_LINES);
glVertex3f((x1),(y1),(z1));
glVertex3f((x2),(y2),(z2));
glEnd();
}
 
 
 
void comp(int v0, int v1, int v2)
/*render a computer at location v0,v1,v2 */
{ glTranslatef(v0, v1, v2);
 
  face4(0,1,2,3,0);
  face4(1,4,5,6,0);
  face4(6,7,8,9,0);
  face4(9,10,11,2,0);
  face4(6,12,13,1,0);
  face4(13,12,14,15,0);
  face4(15,14,16,17,0);
  face4(1,18,19,2,0);
  face4(6,9,20,21,0);
  face4(21,14,16,20,0);
  face4(18,15,17,19,0);
  face4(20,19,17,16,0);
  face4(22,23,24,25,1);
  face4(26,27,28,29,0);
 
   }
 
 
void network(int x, int y, int z)
{
    glTranslatef(x, y , z);
 
    glPushMatrix();
  glScalef(0.25, 0.25, 0.25);
 
  glPushMatrix();
  glTranslatef(-7.0, 0.0, -7.0);
  glRotatef(45.0, 0.0, 1.0, 0.0);
  comp(0,0,0);
  glPopMatrix();
 
  glPushMatrix();
  glTranslatef(-7.0, 0.0, 7.0);
  glRotatef(135.0, 0.0, 1.0, 0.0);
  comp(0,0,0);
  glPopMatrix();
 
 glPushMatrix();
  glTranslatef(7.0, 0.0, 7.0);
  glRotatef(-135.0, 0.0, 1.0, 0.0);
  comp(0,0,0);
  glPopMatrix();
 
  glPushMatrix();
  glTranslatef(7.0, 0.0, -7.0);
  glRotatef(-45.0, 0.0, 1.0, 0.0);
  comp(0,0,0);
  glPopMatrix();
 
  glPushMatrix();
  glLineWidth(5.0);
  cable(8,-0.1,-8,8,-0.1,8);
  cable(-8,-0.1,-8,-8,-0.1,8);
  cable(8,-0.1,-8,-8,-0.1,-8);
  cable(8,-0.1,8,-8,-0.1,8);
  glPopMatrix();
 
  glPopMatrix();
 
}void lan(void)
{  glPushMatrix();
 
  glTranslatef(-10.0, 0.0, 0.0);
  glRotatef(angle, 0.0, 1.0, 0.0);
  glScalef(1.5, 1.5, 1.5);
  network(0,0,0);
  glPopMatrix();
}
 
void wan(void)
{
  glTranslatef(-12.5, 0.0, 0.0);
  glPushMatrix();
  glScalef(0.5, 0.5, 0.5);
 
  glPushMatrix();
  network(10,0,-5);
  glPopMatrix();
 
  glPushMatrix();
  network(10,0,-10);
  glPopMatrix();
 
  glPushMatrix();
  network(15,0,-5);
  glPopMatrix();
 
  glPushMatrix();
  network(15,0,-10);
  glPopMatrix();
 
  glPushMatrix();
  network(15,0,10);
  glPopMatrix();
 
  glPushMatrix();
  network(15,0,15);
  glPopMatrix();
 
  glPushMatrix();
  network(10,0,10);
  glPopMatrix();
 
  glPushMatrix();
  network(10,0,15);
  glPopMatrix();
 
  glPushMatrix();
  network(30,0,10);
  glPopMatrix();
 
  glPushMatrix();
  network(30,0,15);
  glPopMatrix();
 
  glPushMatrix();
  network(35,0,10);
  glPopMatrix();
 
  glPushMatrix();
  network(35,0,15);
  glPopMatrix();
 
   glPushMatrix();
  network(30,0,-5);
  glPopMatrix();
 
  glPushMatrix();
  network(30,0,-10);
  glPopMatrix();
 
  glPushMatrix();
  network(35,0,-5);
  glPopMatrix();
 
  glPushMatrix();
  network(35,0,-10);
  glPopMatrix();
 
  glPushMatrix();
  glLineWidth(5.0);
  cable(10,-0.1,-3,10,-0.1,8);
  cable(17,-0.1,-10,28,-0.1,-10);
  cable(17,-0.1,15,28,-0.1,15);
  glPopMatrix();
 
  glPopMatrix();
}
void display(void)
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
     glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(100.0, 1.4, 5.0, 50.0);
 
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
 
  glPushMatrix();
  glTranslatef(0.0, 0.0, -5.0);
//Need to combine bmp texture to this polygon!
  /*glBindTexture(GL_TEXTURE_2D, texture[0]);*/
  glBegin(GL_POLYGON);
     glVertex2f(8.0, -3.0);
     glVertex2f(-8.0, -3.0);
     glVertex2f(-8.0, -6.0);
     glVertex2f(8.0, -6.0);
  glEnd();
  glPopMatrix();
 
  gluLookAt(0.0, 5.0, 15.0, 3.0, 0.0, 0.0, 0.0, 1.0, 0.0);
  /*glRotatef(angle, 0.0, 1.0, 0.0);*/
 
  glPushMatrix();
  lan();
  glPopMatrix();
 
  glPushMatrix();
  glTranslatef(12.5, 0.0, 0.0);
  glRotatef(angle, 0.0, 1.0, 0.0);
  wan();
  glPopMatrix();
 
 
  glutSwapBuffers();
 
 
 
}
 
void specialKeys(int key, int x, int y)
{
  if (key == GLUT_KEY_LEFT)
     angle_step = angle_step + change;
  else if (key == GLUT_KEY_RIGHT)
     angle_step = angle_step - change;
}
 
void keyboard(unsigned char code, int x, int y)
{
  if (code == ' ')
     angle_step = 0.0;
  else if (code == 27)
     exit(0);
}
 
void update(void)
{
  angle = angle + angle_step2;
  if (angle >= 360.0)
     angle = angle - 360.0;
  else if (angle < 0.0)
     angle = angle + 360.0;
  glutPostRedisplay();
}
void reshape(int w, int h)
{
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(40.0, (GLdouble)w/(GLdouble)h, 0.5, 20.0);
  glViewport(0, 0, (GLsizei)w, (GLsizei)h);
}
 
int main(int argc, char** argv)
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGB);
  glutInitWindowSize(1000,700);
  glutInitWindowPosition(12.5,10);
  glutCreateWindow("Square");
  init();
  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutIdleFunc(update);
  glutSpecialFunc(specialKeys);
  glutKeyboardFunc(keyboard);
  glutMainLoop();
  return 0;
}
