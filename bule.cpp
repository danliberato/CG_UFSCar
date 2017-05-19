#include <GL/glut.h>
#include <cmath>
#include <iostream>
#include <SDL_image.h>

// Cores
GLfloat WHITE[] = {1, 1, 1};
GLfloat RED[] = {1, 0, 0};
GLfloat GREEN[] = {0, 1, 0};
GLfloat AMARELO[] = {1, 1, 0};
GLfloat G[] = {2, 2, 2};
GLfloat ROXO[] = {1, 0, 1};
GLfloat BLACK[] = {0, 0, 0};
int x_aux = 0;
double moveX = 1;
double moveZ = 1;
bool moveDir=false, moveEsq=false, moveCima=false, moveBaixo=false;//movimentos do jogador
//flag marca que jogador perdeu
bool fim = false;
//flag que controla o reiniciar
bool reinicia = false;
//flag marca que jogador ganhou
bool ganhou = false;
//controla se jogador pode avançar
bool esq = true;
bool cima = true;
bool baixo = true;

bool enter = false;


//pontos de controle da curva de bezier
GLfloat ctrlpoints[4][4][3] = {
   {{-1.5, -1.5, 4.0}, {-0.5, -1.5, 2.0}, 
    {0.5, -1.5, -1.0}, {1.5, -1.5, 2.0}}, 
   {{-1.5, -0.5, 1.0}, {-0.5, -0.5, 3.0}, 
    {0.5, -0.5, 0.0}, {1.5, -0.5, -1.0}}, 
   {{-1.5, 0.5, 4.0}, {-0.5, 0.5, 0.0}, 
    {0.5, 0.5, 3.0}, {1.5, 0.5, 4.0}}, 
   {{-1.5, 1.5, -2.0}, {-0.5, 1.5, -2.0}, 
    {0.5, 1.5, 0.0}, {1.5, 1.5, -1.0}}
};


//classe criacao da camera... visao
class Camera {
  double theta;      // aux no x e z 
  double y;          
  double dTheta;     //incrementa theta... camera
  double dy;         //incrementa y... camera
public:
  Camera(): theta(-0.65), y(18), dTheta(0.04), dy(0.2) {
  }
  double getX() {return 10 * cos(theta);}
  double getY() {return y;}
  double getZ() {return 10 * sin(theta);}
  void moveRight() {theta += dTheta;}
  void moveLeft() {theta -= dTheta;}
  void moveUp() {y += dy;}
  void moveDown() {if (y > dy) y -= dy;}
};


class Checkerboard {
  int displayListId;
  int width;
  int depth;

public:
  double PSpeedY;

  

  Checkerboard(int width, int depth): width(width), depth(depth) {
    PSpeedY = 10;
  }
  double centerx() {return width / 2;}
  double centerz() {return depth / 2;}
  int rx() {return width;}
  int rz() {return depth;}
  void create() {
    displayListId = glGenLists(1);
    glNewList(displayListId, GL_COMPILE);
    //GLfloat lightPosition[] = {18, 3, 7, 1};
    //GLfloat ambiente[] = { 0.2, 0.2, 0.2, 1.0 };
    //GLfloat difusa[] = { 0.7, 0.7, 0.7, 1.0 };
    //GLfloat especular[] = { 1.0, 1.0, 1.0, 1.0 };
    //GLfloat posicao1[] = { -9, 9, -9, 9 };
    GLfloat posicao2[] = { 18, 3, -7, 1};
    //GLfloat direcao[] = {0,0,-1};
    //GLfloat lmodelo_ambiente[] = { 0.2, 0.2, 0.2, 1.0 };
    glLightfv(GL_LIGHT0, GL_POSITION, posicao2);
    glBegin(GL_QUADS);
    glNormal3d(0, 1, 0);

    for (int x = 0; x < width - 1; x++) {
     for (int z = 0; z < depth - 1; z++) {
      glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, (x+z)%2==0 ? BLACK : ROXO);
      glVertex3d(x, 0, z);
      glVertex3d(x+1, 0, z);
      glVertex3d(x+1, 0, z+1);
      glVertex3d(x, 0, z+1);
      }
    }
    glEnd();




    glBegin(GL_QUADS);
    
    glNormal3d(0.5, 0.5, 0.5);
    for (int x = 0; x < 100; x++) {
     for (int z = -50; z < 100; z++) {
      glMaterialfv(GL_FRONT, GL_AMBIENT, ROXO);
      if(x%2 == 0){
        glVertex3d(-x, -8, z);
        glVertex3d(-x+1, -8, z);
        glVertex3d(-x+1, -8, z+1);
        glVertex3d(-x, -8, z+1);
      }
      else{
       glVertex3d(x, -8, z);
        glVertex3d(x+1, -8, z);
        glVertex3d(x+1, -8, z+1);
        glVertex3d(x, -8, z+1); 
      }
      }
    }
    glEnd();



    glEndList();
  }
  void draw() {
    glCallList(displayListId);
  }

  void KeyControl();
};

//variaveis globais
Checkerboard checkerboard(16, 16);
Camera camera;


class Ball {
  
public:
double vx;
double vy;
double w1, w2;
void criaJogador();
void destroiJogador();
void rebate();
void tipoObstaculo1();
void tipoObstaculo2();
void tipoObstaculo3();
void tipoObstaculo4();
void moveJogador();
void colisao();
void curva();


  double radius;
  GLfloat* color;
  double maxH;
  double x;
  double y;
  double z;
  int direction;
  Ball(double r, GLfloat* c, double h, double x, double z):
      radius(r), color(c), maxH(h), direction(-1),
      y(h), x(x), z(z) {
        vx=0.05;
        w1 = 0.5;
        w2 = 0.5;
  }
};

class Imprime {

public:

    Imprime(double theCursx = 0., double theCursy = 0., double theLineht = 0.1){ 
    	setup(theCursx, theCursy, theLineht); 
    }

    void setup(double theCursx,double theCursy,double theLineht = 0.1){ 
    	cursx = theCursx; cursy = theCursy; lineht = theLineht; 
    }

    void print(const std::string & msg)
    {
        glRasterPos2d(cursx, cursy);
        for (std::string::const_iterator ii = msg.begin();
             ii != msg.end();
             ++ii)
        {
            glutBitmapCharacter(GLUT_BITMAP_9_BY_15, *ii);
        }
        cursy -= lineht;
    }

private:

    double cursx, cursy;  
    double lineht;        

}; 


Ball balls[] = {
  Ball(0.5, GREEN, 6.5, 5.5, 1.5),//0
  Ball(1.5, WHITE, 12, 5.5, 4.5),//1
  Ball(1.5, RED, 10, 9.5, 5.5),//2
  Ball(0.4, AMARELO, 0.5, 1.5, 0.5),//jogador
  Ball(3.0, ROXO, 20, 0, 10),//4
  Ball(1.5, ROXO, 13, 0.5, 7.5),//5
  Ball(0.5, RED, 15.5, 13.5, 2.5),//6
  Ball(0.5, RED, 1.8, 3.5, 10.5),//7
  Ball(0.5, WHITE, 15.5, 14.5, 9.5)//8
};

int t=0;


void Ball::colisao(){
	//colisao da bola gde branca
	if(balls[3].w1 == balls[1].x && balls[3].w2 == balls[1].z && balls[1].y == 1.5)
		fim = true;

	//colisao da bola gde vermelha
	if(balls[3].w1 == balls[2].x && balls[3].w2 == balls[2].z && balls[2].y == 1.5)
		fim = true;

	//colisao da bola gde roxa
	if(balls[3].w1 == balls[5].x && balls[3].w2 == balls[5].z && balls[5].y == 1.5)
		fim=true;

	//colisao da bolinha verde
	if(balls[0].x == balls[3].w1 && balls[0].z == balls[3].w2)
		fim=true;


	//colisao da bolinha vermelha
	if(balls[6].x == balls[3].w1 && balls[6].z == balls[3].w2)
		fim = true;

  //colisao da bolinha branca
  if(balls[8].x == balls[3].w1 && balls[8].z == balls[3].w2)
    fim = true;

  	//colisao com a rede
  	if(balls[7].x == balls[3].w1 && balls[3].w2 >= 10.5 && balls[3].w2 <= 12.5)
		fim = true;
	
	//impede o jogador de atravessar o cubo da direita pra esquerda
	if(balls[3].w1 == 11.5 && balls[3].w2>=4.5 && balls[3].w2 <=6.5)
		esq = false;
	else
		esq = true;

	//impede o jogador de atravessar o cubo de baixo pra cima
	if(balls[3].w1 >= 12.5 && balls[3].w1<=14.5 && balls[3].w2 ==3.5)
		cima = false;
	else
		cima = true;	

	//impede o jogador de atravessar o cubo de cima pra baixo
	if(balls[3].w1 >= 12.5 && balls[3].w1<=14.5 && balls[3].w2 ==7.5)
		baixo = false;
	else
		baixo = true;

	if(balls[3].w2 == 14.5)
		ganhou = true; 
}



void Ball::moveJogador(){
  if(moveEsq == true && moveDir == false){
    if(w1<14)
    w1 += moveX;
  
    moveEsq = false;
  }
  if(moveDir == true && moveEsq == false){
    if(w1>1)
      w1 -= moveX;
    moveDir = false;
  }

  if(moveCima == true && moveBaixo == false){
    if(w2<14)
      w2 += moveX;
    moveCima = false;
  }

  if(moveCima == false && moveBaixo == true){
    if(w2>1)
    w2 -= moveX;
    moveBaixo = false;
  }

  
}



void Ball::criaJogador() {

    glPushMatrix();
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color);

    glTranslated(w1, y,w2);
    x = w1;
    glutSolidTeapot(radius);    
    glPopMatrix();
}

//bascimanente muda a cor do jogador quando perde...
void Ball::destroiJogador(){
	glPushMatrix();
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, BLACK);

    glTranslated(w1, y, w2);
    x = w1;
    glutSolidTeapot(radius);    
    glPopMatrix();	
}


//desliza bola
void Ball::tipoObstaculo1() {
    y = radius;
    
    if (x > checkerboard.rx()-1) {
      x = checkerboard.rx()-1; direction = -1;
      //vx+=0.01;
    } else if (x < radius) {
      //vx+=0.01;
      x = radius; direction = 1;
    }
    if(direction == 1 && !fim)
      x += 0.25;
    else
   	  if(!fim)
      x += -0.25;
    glPushMatrix();
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color);
    glTranslated(x, y, z);
    glutSolidSphere(radius, 30 , 30);

    glPopMatrix();
  }


//bola pula
void Ball::tipoObstaculo2() {
    y += direction * 0.5;
    if (y >= maxH) {
      y = maxH; direction = -1;
      if(fim)
      	y=1;
    } 
    else{ 
      if (y <= 1.5) {
      	y = radius; direction = 1;
  		if(fim)
  			y=1;
    }
	}
    glPushMatrix();
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color);
    glTranslated(x, y, z);
    glutSolidSphere(radius, 30, 30);
    glPopMatrix();
}


//cria o obstaculo em forma de rede, utilizando a curva de bezier e desliza...
void Ball::curva(){
  
  glMap2f(GL_MAP2_VERTEX_3, 0, 1, 3, 4, 0, 1, 12, 4, &ctrlpoints[0][0][0]);
  glEnable(GL_MAP2_VERTEX_3);
  glMapGrid2f(20, 0.0, 1.0, 20, 0.0, 1.0);
  glEnable(GL_DEPTH_TEST);
  //glShadeModel(GL_FLAT);
  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color);
  glTranslated(x, y, z);

	glRotatef(85.0, 1.0, 1.0, 4);

    if (x > checkerboard.rx()-1) {
      x = checkerboard.rx()-1; direction = -1;
    } else if (x < radius) {
      x = radius; direction = 1;
    }
    if(direction == 1 && !fim)
      x += 0.25;
    else
      if(!fim)
      x += -0.25;
   glPushMatrix ();
   
   for (int j = 0; j <= 8; j++) {
      glBegin(GL_LINE_STRIP);
      for (int i = 0; i <= 30; i++)
         glEvalCoord2f((GLfloat)i/30.0, (GLfloat)j/8.0);
      glEnd();
      glBegin(GL_LINE_STRIP);
      for (int i = 0; i <= 30; i++)
         glEvalCoord2f((GLfloat)j/8.0, (GLfloat)i/30.0);
      glEnd();
   }
  

}


//cubo cai e fica
void Ball::tipoObstaculo3() {

    y += direction * 0.5;
    if (y > maxH) {
      y = 1; 
    } else if (y < radius) {
      y = 1; 
    }
    glPushMatrix();
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color);
    glTranslated(13.5, y, 5.5);
    glutSolidCube(radius);
    glPopMatrix();
}

void textura() {
	GLuint TextureID;
	
	SDL_Surface* Surface = IMG_Load("tijolo.jpg");

	
	glGenTextures(1, &TextureID);
	glBindTexture(GL_TEXTURE_2D, TextureID);

	int Mode = GL_RGB;
	 
	if(Surface->format->BytesPerPixel == 4) {
	    Mode = GL_RGBA;
	} 

	glTexImage2D(GL_TEXTURE_2D, 0, Mode, Surface->w, Surface->h, 0, Mode, GL_UNSIGNED_BYTE, Surface->pixels);
	
	if(!enter){
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);//textura aparece
	

	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_BLEND); 
	glEnable(GL_TEXTURE_2D);
	gluPerspective(400.0,1.0,0.0,30.0);


 // Desenha um cubo no qual a textura é aplicada
	glBegin ( GL_QUADS );
		// Face frontal
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);
		// Face posterior
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);
		// Face superior
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f,  1.0f,  1.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f,  1.0f,  1.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);
		// Face inferior
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f, -1.0f, -1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);
		// Face lateral direita
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);
		// Face lateral esquerda
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);
	glEnd();
	 glFlush();
	glutSwapBuffers();
}
free(Surface);
}


void init() {
  glEnable(GL_DEPTH_TEST);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, WHITE);
  glLightfv(GL_LIGHT0, GL_SPECULAR, WHITE);
  glMaterialfv(GL_FRONT, GL_SPECULAR, WHITE);
  glMaterialf(GL_FRONT, GL_SHININESS, 30);
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);

  checkerboard.create();


}

void display() {

	  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	  glClearColor(0, 0, 0, 1.0);
	  glLoadIdentity();
	
	  gluLookAt(camera.getX(), camera.getY(), camera.getZ(),
	            checkerboard.centerx(), 0.0, checkerboard.centerz(),
	            0.0, 1.0, 0.0);
	  checkerboard.draw();
	  //for (int i = 0; i < sizeof balls / sizeof(Ball); i++) {
	    balls[0].tipoObstaculo1();
	    balls[1].tipoObstaculo2();
	    balls[2].tipoObstaculo2();
	    if(!fim)
	    	balls[3].criaJogador();
	    else
	    	balls[3].destroiJogador();

	    balls[4].tipoObstaculo3();
	    balls[5].tipoObstaculo2();
	    balls[6].tipoObstaculo1();
	    balls[8].tipoObstaculo1();
	    
	    balls[7].curva();
	    

	    balls[3].colisao();
	  
	  		textura();
	  

	  //}
	   glPopMatrix ();
	  glFlush();

	  glutSwapBuffers();
  
}

void reshape(GLint w, GLint h) {
  glViewport(0, 0, w, h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  
  gluPerspective(40.0, GLfloat(w) / GLfloat(h), 1.0, 150.0);
  glMatrixMode(GL_MODELVIEW);
 
  
  glMatrixMode(GL_MODELVIEW);

}



void timer(int v) {
  balls[3].moveJogador();
  if(enter && !ganhou)
  glutPostRedisplay();

  else{
 	
  	glLoadIdentity();
 
    glMatrixMode(GL_PROJECTION);  // Set up simple ortho projection
 
    glPushMatrix();
 
    glLoadIdentity();
 
    
    if(!ganhou){
		Imprime p(-0.4, 0.3, 0.1);

	    p.print("Enter     para iniciar o jogo");
	    p.print("w         movimenta o bule para cima");
	    p.print("a         movimenta o bule para esquerda");
	    p.print("d         movimenta o bule para direita");
	    p.print("s         movimenta o bule para baixo");
	    p.print("< ^ > v   setas do teclado e o mouse movimentam a camera");
	    p.print("Space     reinicia o jogo");
	    p.print("Esc       para sair");
	    p.print("Desenvolvido por: Fabio Toshiro Nakano Ashikawa");
	    p.print("RA: 552160");
		glPopMatrix();                // Restore prev projection
		glMatrixMode(GL_MODELVIEW);

		glutSwapBuffers();

    }
    else{

    	Imprime p(-0.4, 0.3, 0.1);
    	p.print("    PARABENS, VOCE GANHOU!!!");
    	p.print("Space     reinicia o jogo");
	    p.print("Esc       para sair");

    }

    glPopMatrix();                // Restore prev projection
    glMatrixMode(GL_MODELVIEW);

    glutSwapBuffers();	  

  }


  glutTimerFunc(100/60, timer, v);
}

//movimenta a camera com as setas do teclado
void special(int key, int, int) {
  switch (key) {
    case GLUT_KEY_LEFT: 
      camera.moveLeft();
      break; 
    case GLUT_KEY_RIGHT: 
      camera.moveRight(); 
      break;
    case GLUT_KEY_UP: 
      camera.moveUp(); 
      break;
    case GLUT_KEY_DOWN: 
      camera.moveDown(); 
      break;

  }
  glutPostRedisplay();
}

void mouse(int button, int state, int x, int y) {
  if(button == GLUT_LEFT_BUTTON){
  	camera.moveLeft();
  }
  if(button == GLUT_RIGHT_BUTTON){
  	camera.moveRight();
  }
  if(button == GLUT_MIDDLE_BUTTON){
  	camera.moveUp();
  }
}
void teclado(unsigned char key, int x, int y){
  switch(key){
    case 27:
      exit(0);
      break;
    case 'a':
      case'A':
      if(!fim && esq)
      moveEsq = true;
      break;
    case 'd':
      case'D':
      if(!fim)
      moveDir = true;
      break;
    case 'w':
      case'W':
      if(!fim && cima)
      moveCima = true;
      break;
    case 's':
      case'S':
      if(!fim && baixo)
      moveBaixo = true;
      break;
    case 32:
   	  fim = false;
   	  ganhou = false;
   	  balls[3].w1 = 0.5;
   	  balls[3].w2 = 0.5;
      break;

    case 13:
   	  enter = true;
      break;

	break;

  }
}

int main(int argc, char** argv) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowPosition(100, 100);
  glutInitWindowSize(800, 600);
  glutCreateWindow("OLHA O BULE!!!");
  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutSpecialFunc(special);
  glutKeyboardFunc(teclado);
  glutMouseFunc(mouse);
  glutTimerFunc(10, timer, 0);
  
  init();
  glutMainLoop();
}