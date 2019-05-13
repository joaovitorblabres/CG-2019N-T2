#include <GL/freeglut.h>
#include <iostream>

#include "Point.h"
#include "pieces.cpp"
using namespace std;

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

#define n 8
#define x1 0
#define x2 8
#define y1 0
#define y2 8
#define DX ((x2 - x1) / n)
#define DY ((y2 - y1) / n)
piece* pieces[8][8];

// draw checkerboard floor
static void drawFloor(void)
{
	glBegin(GL_QUADS);
	// same normal for everything
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

// function called by GLUT whenever a redraw is needed
static void display()
{
	// clear the window with the predefined color
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glClearColor(0.3, 0.3, 0.3, 1.0);
	// setup viewing transformation
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

	// make sure everything gets drawn
	glFlush();
}

// we recompute projection matrix on every resize, and reset the viewport
static void resizeCB(int w, int h)
{
	// setup perspective transformation
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, double(w) / h, 0.1, 100);
	//glOrtho(-10.0, 10.0, -10.0, 10.0, 5, 100);

	glViewport(0, 0, w, h);
}

// when mouse button is clicked, we determine which viewing mode to
// initialize and also remember where the mouse was clicked
static void mouseClickCB(int button, int state, int x, int y)
{
	globals.mouseX = x;
	globals.mouseY = y;
	//cout << (x - 195)/50 << ' ' << 7-(y - 130)/43 << " tem: " << tab[(y - 130)/43][7-(x - 195)/50] << endl;
	if(pieces[(y - 130)/43][7-(x - 195)/50] != NULL && (y - 130)/43 >= 0 && (y - 130)/43 <= 7 && (x - 195)/50 >= 0 && (x - 195)/50 <= 7){
		pieces[(y - 130)/43][7-(x - 195)/50]->select();
	}

	if (state == GLUT_UP) {
		globals.viewingMode = globals.NONE;
	}
	else if (button == GLUT_LEFT_BUTTON) {
		globals.viewingMode = globals.ROTATING;
	}
	else if (button == GLUT_MIDDLE_BUTTON) {
		globals.viewingMode = globals.ZOOMING;
	}
	else {
		globals.viewingMode = globals.NONE;
	}
}

// when user drags the mouse, we either rotate or zoom
/*static void mouseMotionCB(int x, int y)
{
	int dx = x - globals.mouseX;
	int dy = y - globals.mouseY;
	globals.mouseX = x;
	globals.mouseY = y;

	if (globals.viewingMode == globals.ROTATING) {
		globals.alpha -= dx / 10.0;
		globals.beta += dy / 10.0;
		if (globals.beta < -80) globals.beta = -80;
		if (globals.beta > 80) globals.beta = 80;
		glutPostRedisplay();
	}
	else if (globals.viewingMode == globals.ZOOMING) {
		globals.dist = std::max(1.0, globals.dist - dy / 10.0);
		glutPostRedisplay();
	}
}*/

static void init(){
	globals.alpha = 90;
	globals.beta = 90;
	globals.dist = 10;
	globals.viewingMode = globals.NONE;

	// initial window size
	glutInitWindowSize(800, 600);

	// what buffers we need
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB
		| GLUT_MULTISAMPLE | GLUT_DEPTH);

	// create a window
	glutCreateWindow("CG-2019-T2 - JOAO VITOR B LABRES");
	loadPieces();
	glClearColor(0.3, 0.3, 0.3, 1.0);
	// register callbacks
	glutDisplayFunc(display);
	glutReshapeFunc(resizeCB);
	glutMouseFunc(mouseClickCB);
	//glutMotionFunc(mouseMotionCB);
	glutIdleFunc(display);

	// use black as the background color
	//glClearColor(0, 0, 0, 0);

	// enable depth buffer
	glEnable(GL_DEPTH_TEST);

	// this is for drawing transparencies
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// enable antialiasing (just in case)
	//    glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
	glEnable(GL_POLYGON_SMOOTH);

	// enable lighting
	glEnable(GL_LIGHTING);

	// enable use of glColor() to specify ambient & diffuse material properties
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

	// set some common light & material properties, which don't have to
	// be re-specified later
	glEnable(GL_LIGHT0);
	GLfloat ambientLight[] = { 0.1, 0.1, 0.1, 1.0 };
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
	GLfloat specularLight[] = { 1.0, 1.0, 1.0, 1.0 };
	glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);
	GLfloat specularColor[] = { 0.7, 0.7, 0.7, 1.0 };
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specularColor);
	GLfloat shininess[] = { 95.0 };
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shininess);

	// since we are going to use scaling, and possibly non-uniform, we'll
	// ask OpenGL to re-normalize our normals
	glEnable(GL_NORMALIZE);
}

// main function
int main(int argc, char ** argv)
{
	glutInit(&argc, argv);
	init();
	glutMainLoop();
	return 0;
}
