/**
 * This demo shows how to periodically call an update() function and calculate
 * the time difference between those updates, i.e. delta time.
 *
 * Author: Fernando Bevilacqua <fernando.bevilacqua@uffs.edu.br>
 * License: MIT
 */

#include <GL/glut.h>
#include <GL/gl.h>
#include <iostream>
#include "math.h"

 // Constants to define the width/height of the window
const int WIDTH = 800;
const int HEIGHT = 800;

// Define how many frames per seconds we want our
// applications to run.
const unsigned int FRAMES_PER_SECOND = 30;
const unsigned int UPDATE_INTERVAL_MS = 1000 / FRAMES_PER_SECOND;

// Control when was the last update() called.
int gTimeLastUpdateMs = 0;

// Control the value of each color channel used to display a square.
GLfloat gValueR = 0.0f;
GLfloat gValueG = 0.0f;
GLfloat gValueB = 0.0f;

GLfloat angley = 0;
GLfloat anglex = 0;

void renderCoordinateAxis(){
	// X axis - green color
	glColor3f(0, 1, 0);
	glBegin(GL_LINES);
		// Left side, negative X
		glVertex2f(-10.0, 0.0);
		glVertex2f(10.0, 0.0);
	glEnd();

	// Y axis - blue color
	glColor3f(0, 0, 1);
	glBegin(GL_LINES);
		glVertex2f(0.0, -10.0);
		glVertex2f(0.0, 10.0);
	glEnd();

	// Z axis - blue color
	glColor3f(1, 0, 0);
	glBegin(GL_LINES);
		glVertex3f(0.0, 0.0, -10.0);
		glVertex3f(0.0, 0.0, 10.0);
	glEnd();

}

void display(){
	// Move the camera away from the origin along the Z axis by 10 pixels.
	glTranslatef(0, 0, -10.0f);

	// Render the X and Y axis to guide ourselves.
	glRotatef(angley, 0, 1, 0);
	glRotatef(anglex, 1, 0, 0);
	renderCoordinateAxis();
	// Render a square using the informed color.
	GLfloat r = sin(gValueR);
	GLfloat g = sin(gValueG);
	GLfloat b = sin(gValueB);
	glColor3f(r, g, b);
	glRectf(-1.0f, 1.0f, 1.0f, -1.0f);
}

// This function is called periodically. The param delta contains the time
// difference between the current and the last frame, in seconds.
void update(double delta){
	// Update the color of the square based on the time
	gValueR += (GLfloat)delta;
	gValueG += (GLfloat)(delta * 1.3);
	gValueB += (GLfloat)(delta * 1.5);
}

void keyboard(unsigned char key, int x, int y){
	if (key == 27) {
		// ESC key
		exit(0);
	}
}

void mouse(int button, int state, int x, int y){
	std::cout << "Mouse pressed: button=" << button << ", state=" << state << ", x=" << x << " y=" << y << std::endl;
}

void internalDisplay(){
	// Clear the screen painting it with the white color
	glClearColor(1, 1, 1, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Inform we want to make changes to the modelview matrix, starting
	// with no transformation at all.
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// Call our display() function
	display();

	// Start the rendering on a new frame
	glutSwapBuffers();
}

void internalUpdate(int value){
	int timeNowMs = glutGet(GLUT_ELAPSED_TIME);
	double deltaSeconds = (timeNowMs - gTimeLastUpdateMs) / 1000.0;

	// Save the time update() was called
	gTimeLastUpdateMs = timeNowMs;
	//std::cout << "timeNOw = " << timeNowMs << ", deltaSeconds = " << deltaSeconds << std::endl;
	// Invoke our update() function
	update(deltaSeconds);

	// Request a new frame rendering
	glutPostRedisplay();

	// Re-schedule the update() function to be called after a few
	// milliseconds again.
	glutTimerFunc(UPDATE_INTERVAL_MS, internalUpdate, 0);
}

void keyboard(int key, int x, int y) {
	if (key == GLUT_KEY_RIGHT) { // GIRA PARA DIREITA
		angley += 5;
	}
	if (key == GLUT_KEY_LEFT) { // GIRA PARA ESQUERDA
		angley -= 5;
	}

	// EXTRA DO EXTRA
	if (key == GLUT_KEY_UP) { // GIRA NO X
		anglex += 5;
	}
	if (key == GLUT_KEY_DOWN) { // GIRA NO X
		anglex -= 5;
	}
}

void initView(){
	// We want a window to the world that is a rectangle starting at (0,0)
	// and ending at (WIDTH, HEIGHT).
	glViewport(0, 0, (GLsizei)WIDTH, (GLsizei)HEIGHT);

	// Adjust the global projection (camera)
	glMatrixMode(GL_PROJECTION);

	// Reset any existing projection settings and adjust the field-of-view (FOV)
	glLoadIdentity();
	glFrustum(-1.0, 1.0, -1.0, 1.0, 1.5, 200.0);

	// From now on, every transformation is to be applied on each object, e.g. modelview.
	glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv){
	// Initialize GLUT
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(WIDTH, HEIGHT);
	glutCreateWindow("Transformation - Simple");

	// Register a few callback functions
	glutDisplayFunc(internalDisplay);
	glutSpecialFunc(keyboard);
	glutMouseFunc(mouse);

	initView();

	// Schedule the update() function to be called after a few
	// milliseconds (calculated as 1000 milliseconds divided by FPS).
	glutTimerFunc(UPDATE_INTERVAL_MS, internalUpdate, 0);

	glutMainLoop();
	return 0;
}
