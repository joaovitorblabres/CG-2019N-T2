#include <GL/freeglut.h>
#include <iostream>
#include "Point.h"
#include "pieces.cpp"
using namespace std;

#define n 8
#define x1 0
#define x2 8
#define y1 0
#define y2 8
#define DX ((x2 - x1) / n)
#define DY ((y2 - y1) / n)

// global variables
static struct {
	// current parameters for controlling glulookat
	double alpha, beta, dist;
	// viewing mode state (depending on which mouse button is being used)
	enum { NONE, ROTATING, ZOOMING } viewingMode;
	// last mouse position
	int mouseX, mouseY;
	// current recursion for drawing the spheres
} globals;

static void update(int value);
static void drawFloor(void);
static void loadPieces();
static void display();
static void resizeCB(int w, int h);
static void mouseClickCB(int button, int state, int x, int y);
static void update(int value);
static void rotation(float deltaTime);

piece* pieces[8][8];
int giro = 0;
int gTimeLastUpdateMs = 0;

const unsigned int FRAMES_PER_SECOND = 30;
const unsigned int UPDATE_INTERVAL_MS = 1000 / FRAMES_PER_SECOND;

static void drawFloor(void){
	glBegin(GL_QUADS);
	glNormal3d(0, 0, 1);

	for (int ix = 0; ix < n; ix++) {
		for (int iy = 0; iy < n; iy++) {
			if ((ix + iy) % 2)
				glColor3f(0.7, 0.7, 0.7);
			else
				glColor3f(0.1, 0.1, 0.1);

			glVertex3d(x1 + ix * DX, y1 + iy * DY, 0);
			glVertex3d(x1 + (ix + 1) * DX, y1 + iy * DY, 0);
			glVertex3d(x1 + (ix + 1) * DX, y1 + (iy + 1) * DY, 0);
			glVertex3d(x1 + ix * DX, y1 + (iy + 1) * DY, 0);
		}
	}
	glEnd();
}

static void loadPieces(){
	for(int i = 0; i < n; i++){
		for(int j = 0; j < n; j++){
			if (tab[i][j] == 11) {
				pieces[i][j] = new pawn(j*DX + DX/2, i*DY + DY/2, 0.5, 1.0, 1.0, 1.0, 1.0);
			}
			if(tab[i][j] == 21){
				pieces[i][j] = new pawn(j*DX + DX/2, i*DY + DY/2, 0.5, 1.0, 0.0, 0.0, 0.0);
			}

			if(tab[i][j] == 12){
				pieces[i][j] = new rook(j*DX + DX/2, i*DY + DY/2, 0.5, 1.0, 1.0, 1.0, 1.0);
			}
			if(tab[i][j] == 22){
				pieces[i][j] = new rook(j*DX + DX/2, i*DY + DY/2, 0.5, 1.0, 0.0, 0.0, 0.0);
			}

			if(tab[i][j] == 13){
				pieces[i][j] = new knight(j*DX + DX/2, i*DY + DY/2, 0.5, 1.0, 1.0, 1.0, 1.0);
			}
			if(tab[i][j] == 23){
				pieces[i][j] = new knight(j*DX + DX/2, i*DY + DY/2, 0.5, 1.0, 0.0, 0.0, 0.0);
			}

			if(tab[i][j] == 14){
				pieces[i][j] = new bishop(j*DX + DX/2, i*DY + DY/2, 0.5, 1.0, 1.0, 1.0, 1.0);
			}
			if(tab[i][j] == 24){
				pieces[i][j] = new bishop(j*DX + DX/2, i*DY + DY/2, 0.5, 1.0, 0.0, 0.0, 0.0);
			}

			if(tab[i][j] == 15){
				pieces[i][j] = new queen(j*DX + DX/2, i*DY + DY/2, 0.5, 1.0, 1.0, 1.0, 1.0);
			}
			if(tab[i][j] == 25){
				pieces[i][j] = new queen(j*DX + DX/2, i*DY + DY/2, 0.5, 1.0, 0.0, 0.0, 0.0);
			}

			if(tab[i][j] == 16){
				pieces[i][j] = new king(j*DX + DX/2, i*DY + DY/2, 0.5, 1.0, 1.0, 1.0, 1.0);
			}
			if(tab[i][j] == 26){
				pieces[i][j] = new king(j*DX + DX/2, i*DY + DY/2, 0.5, 1.0, 0.0, 0.0, 0.0);
			}
		}
	}
}

static void display(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	double r = globals.dist;
	double a = globals.alpha * M_PI / 180.0;
	double b = globals.beta * M_PI / 180.0;
	Point p(r * cos(a) * cos(b), r*sin(a)*cos(b), r*sin(b));
	Point c(n/2, n/2, 1.0);
	p = p + c;

	GLfloat lightPos[] = { 5.0, 5.0, 10.0, 1.0 };
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

	gluLookAt(p.x(), p.y(), p.z(), c.x(), c.y(), c.z(), 0, 0, 1);

	drawFloor();

	for (int ix = 0; ix < n; ix++) {
		for (int iy = 0; iy < n; iy++) {
			if(pieces[ix][iy] == NULL) continue;
			pieces[ix][iy]->update();
		}
	}

	glFlush();
}

static void resizeCB(int w, int h){
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, double(w) / h, 0.1, 100);

	glViewport(0, 0, w, h);
}

static void mouseClickCB(int button, int state, int x, int y){
	globals.mouseX = x;
	globals.mouseY = y;
	if(pieces[(globals.mouseY - 130)/43][7-(globals.mouseX - 195)/50] != NULL && (globals.mouseY - 130)/43 >= 0 && (globals.mouseY - 130)/43 <= 7 && (globals.mouseX - 195)/50 >= 0 && (globals.mouseX - 195)/50 <= 7){
		pieces[(globals.mouseY - 130)/43][7-(globals.mouseX - 195)/50]->select();
		giro = 0;
		gTimeLastUpdateMs = glutGet(GLUT_ELAPSED_TIME);
		glutTimerFunc(UPDATE_INTERVAL_MS, update, 0);
	}
}


static void update(int value){
	int timeNowMs = glutGet(GLUT_ELAPSED_TIME);
	double deltaSeconds = (timeNowMs - gTimeLastUpdateMs) / 1000.0;

	gTimeLastUpdateMs = timeNowMs;
	rotation(deltaSeconds);

	glutPostRedisplay();

	if (deltaSeconds <= 15){
		glutTimerFunc(UPDATE_INTERVAL_MS, update, 0);
	}else{
		deltaSeconds = 0;
		pieces[(globals.mouseY - 130)/43][7-(globals.mouseX - 195)/50]->deselect();
		giro = 0;
	}
}

static void rotation(float deltaTime){
	if(!giro && globals.beta <= 90)
		globals.beta -= deltaTime * 24.0;
	if(!giro && globals.beta < 45){
		globals.alpha += deltaTime * 360 / 10;
		globals.beta = 45;
	}

	if(globals.alpha - 90 > 360){
		globals.alpha = (int)globals.alpha % 360;
		giro = 1;
	}

	if(giro && globals.alpha - 90 >= 0){
		globals.beta += deltaTime * 24.0;
	}

	if(globals.beta > 90){
		globals.beta = 90;
	}

	if(globals.beta >= 90 && globals.alpha >= 90 && giro){
		pieces[(globals.mouseY - 130)/43][7-(globals.mouseX - 195)/50]->deselect();
	}

	glutPostRedisplay();
}

static void init(){
	globals.alpha = 90;
	globals.beta = 90;
	globals.dist = 10;
	globals.viewingMode = globals.NONE;

	glutInitWindowSize(800, 600);

	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB
		| GLUT_MULTISAMPLE | GLUT_DEPTH);

	glutCreateWindow("CG-2019-T2 - JOAO VITOR B LABRES");
	loadPieces();
	glClearColor(0.3, 0.3, 0.3, 1.0);

	glutDisplayFunc(display);
	glutReshapeFunc(resizeCB);
	glutMouseFunc(mouseClickCB);
	glutIdleFunc(display);

	glEnable(GL_DEPTH_TEST);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_POLYGON_SMOOTH);

	glEnable(GL_LIGHTING);

	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

	glEnable(GL_LIGHT0);
	GLfloat ambientLight[] = { 0.1, 0.1, 0.1, 1.0 };
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
	GLfloat specularLight[] = { 1.0, 1.0, 1.0, 1.0 };
	glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);
	GLfloat specularColor[] = { 0.7, 0.7, 0.7, 1.0 };
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specularColor);
	GLfloat shininess[] = { 95.0 };
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shininess);

	glEnable(GL_NORMALIZE);
}

int main(int argc, char ** argv){
	glutInit(&argc, argv);
	init();
	glutMainLoop();
	return 0;
}
