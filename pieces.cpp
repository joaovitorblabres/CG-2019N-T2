#include <GL/freeglut.h>
#include <iostream>
using namespace std;

// 0 = NONE = VOID
// 1X = black pieces
// 2X = white pieces
// X = 1 PAWN; X = 2 ROOK; X = 3 KNIGHT; X = 4 BISHOP; X = 5 QUEEN; X = 6 KING
const int tab[8][8] = {
                        {22, 23, 24, 25, 26, 24, 23, 22},
                        {21, 21, 21, 21, 21, 21, 21, 21},
                        {0, 0, 0, 0, 0, 0, 0, 0},
                        {0, 0, 0, 0, 0, 0, 0, 0},
                        {0, 0, 0, 0, 0, 0, 0, 0},
                        {0, 0, 0, 0, 0, 0, 0, 0},
                        {11, 11, 11, 11, 11, 11, 11, 11},
                        {12, 13, 14, 15, 16, 14, 13, 12}
                      };


class piece{
public:
  GLfloat x, y, z = 0.5, height, R, G, B;
  // x, y, z = posicao no mundo
  // tamnho...
  // R, G, B...
  piece(){}
  piece(GLfloat x1, GLfloat y1, GLfloat z1, GLfloat tam, GLfloat R1, GLfloat G1, GLfloat B1){
    this->x = x1;
    this->y = y1;
    this->z = z1;
    this->height = tam;
    this->R = R1;
    this->G = G1;
    this->B = B1;
  }

  virtual void update() = 0;
  virtual void moving() = 0;
};

class pawn: public piece{
public:
  pawn(GLfloat x1, GLfloat y1, GLfloat z1, GLfloat tam, GLfloat R1, GLfloat G1, GLfloat B1):piece(x1, y1, z1, tam, R1, G1, B1){};
  GLint selected = 0;
  void update() override{
    glPushMatrix();
      glColor3f(R, G, B);
      glTranslatef(x+height/2, y+height/2, z*0.8 + 0.01);
      glScalef(0.5, 0.5, 0.8);
      glutSolidCube(height);
    glPopMatrix();

    glPushMatrix();
      glColor3f(R, G, B);
      glTranslatef(x+height/2, y+height/2, z*1.6 + 0.01);
      glutSolidSphere(0.3, 32, 32);
    glPopMatrix();

    if(selected == 1){
        glColor3f(0, 0, 0.8);
        glBegin(GL_QUADS);
          glVertex3d(x, y, 0.02);
          glVertex3d(x+1, y, 0.02);
          glVertex3d(x+1, y+1, 0.02);
          glVertex3d(x, y+1, 0.02);
        glEnd();
    }
  }

  void moving() override{
    selected = 1;
  }
};

class rook: public piece{
public:
  rook(GLfloat x1, GLfloat y1, GLfloat z1, GLfloat tam, GLfloat R1, GLfloat G1, GLfloat B1):piece(x1, y1, z1, tam, R1, G1, B1){};
  GLint selected = 0;
  void update() override{
    glPushMatrix();
      glColor3f(R, G, B);
      glTranslatef(x+height/2, y+height/2, z*1.4 + 0.01);
      glScalef(0.8, 0.8, 0.2);
      glutSolidCube(height);
    glPopMatrix();

    glPushMatrix();
      glColor3f(R, G, B);
      glTranslatef(x+height/2, y+height/2, z + 0.01);
      glScalef(0.5, 0.5, 1);
      glutSolidCube(height);
    glPopMatrix();

    if(selected){
        glColor3f(0, 0, 0.8);
        glBegin(GL_QUADS);
          glVertex3d(x, y, 0.02);
          glVertex3d(x+1, y, 0.02);
          glVertex3d(x+1, y+1, 0.02);
          glVertex3d(x, y+1, 0.02);
        glEnd();
    }
  }

  void moving() override{
    selected = 1;
  }
};

class knight: public piece{
public:
  knight(GLfloat x1, GLfloat y1, GLfloat z1, GLfloat tam, GLfloat R1, GLfloat G1, GLfloat B1):piece(x1, y1, z1, tam, R1, G1, B1){};
  GLint selected = 0;
  void update() override{

    glPushMatrix();
      glColor3f(R, G, B);
      glTranslatef(x+height/2, y+height/2, z*1.2 + 0.01);
      glScalef(0.5, 0.5, 1.2);
      glutSolidCube(height);
    glPopMatrix();

    glPushMatrix();
      glColor3f(R, G, B);
      if(this->R == 0){
        glTranslatef(x+height/2, y+height/2 + 0.1, z*1.8 + 0.01);
      }else{
        glTranslatef(x+height/2, y+height/2 - 0.1, z*1.8 + 0.01);
      }
      glScalef(0.5, 0.9, 0.5);
      glutSolidCube(height);
    glPopMatrix();

    glPushMatrix();
      glColor3f(R, G, B);
      if(this->R == 0){
        glTranslatef(x+height/2 - 0.2, y+height/2, z*2.2 + 0.01);
      }else{
        glTranslatef(x+height/2 + 0.2, y+height/2, z*2.2 + 0.01);
      }
      glScalef(0.1, 0.2, 0.6);
      glutSolidCube(height);
    glPopMatrix();

    glPushMatrix();
      glColor3f(R, G, B);
      if(this->R == 0){
        glTranslatef(x+height/2 + 0.2, y+height/2, z*2.2 + 0.01);
      }else{
        glTranslatef(x+height/2 - 0.2, y+height/2, z*2.2 + 0.01);
      }
      glScalef(0.1, 0.2, 0.6);
      glutSolidCube(height);
    glPopMatrix();

    if(selected){
      glColor3f(0, 0, 0.8);
      glBegin(GL_QUADS);
        glVertex3d(x, y, 0.02);
        glVertex3d(x+1, y, 0.02);
        glVertex3d(x+1, y+1, 0.02);
        glVertex3d(x, y+1, 0.02);
      glEnd();
    }
  }

  void moving() override{
    selected = 1;
  }
};

class bishop: public piece{
public:
  bishop(GLfloat x1, GLfloat y1, GLfloat z1, GLfloat tam, GLfloat R1, GLfloat G1, GLfloat B1):piece(x1, y1, z1, tam, R1, G1, B1){};
  GLint selected = 0;
  void update() override{
    glPushMatrix();
      glColor3f(R, G, B);
      glTranslatef(x+height/2, y+height/2, z*1.2 + 0.01);
      glScalef(0.4, 0.4, 1.2);
      glutSolidCube(height);
    glPopMatrix();

    glPushMatrix();
      glColor3f(R, G, B);
      glTranslatef(x+height/2, y+height/2, z*2.2 + 0.01);
      glScalef(0.6, 0.6, 0.3);
      glutSolidCube(height);
    glPopMatrix();

    glPushMatrix();
      glColor3f(R, G, B);
      glTranslatef(x+height/2, y+height/2, z*2.9 + 0.01);
      glScalef(0.1, 0.1, 0.4);
      glutSolidCube(height);
    glPopMatrix();

    glPushMatrix();
      glColor3f(R, G, B);
      glTranslatef(x+height/2, y+height/2, z*3 + 0.01);
      glScalef(0.1, 0.4, 0.1);
      glutSolidCube(height);
    glPopMatrix();
    if(selected){
      glColor3f(0, 0, 0.8);
      glBegin(GL_QUADS);
        glVertex3d(x, y, 0.02);
        glVertex3d(x+1, y, 0.02);
        glVertex3d(x+1, y+1, 0.02);
        glVertex3d(x, y+1, 0.02);
      glEnd();
    }
  }

  void moving() override{
    selected = 1;
  }
};

class queen: public piece{
public:
  queen(GLfloat x1, GLfloat y1, GLfloat z1, GLfloat tam, GLfloat R1, GLfloat G1, GLfloat B1):piece(x1, y1, z1, tam, R1, G1, B1){};
  GLint selected = 0;
  void update() override{
    glPushMatrix();
      glColor3f(R, G, B);
      glTranslatef(x+height/2, y+height/2, z*1.4 + 0.01);
      glScalef(0.4, 0.4, 1.4);
      glutSolidCube(height);
    glPopMatrix();

    glPushMatrix();
      glColor3f(R, G, B);
      glTranslatef(x+height/2, y+height/2, z*3.1 + 0.01);
      glScalef(0.1, 0.1, 0.1);
      glutSolidDodecahedron();
    glPopMatrix();
    if(selected){
        glColor3f(0, 0, 0.8);
        glBegin(GL_QUADS);
          glVertex3d(x, y, 0.02);
          glVertex3d(x+1, y, 0.02);
          glVertex3d(x+1, y+1, 0.02);
          glVertex3d(x, y+1, 0.02);
        glEnd();
    }
  }

  void moving() override{
    selected = 1;
  }
};

class king: public piece{
public:
  king(GLfloat x1, GLfloat y1, GLfloat z1, GLfloat tam, GLfloat R1, GLfloat G1, GLfloat B1):piece(x1, y1, z1, tam, R1, G1, B1){};
  GLint selected = 0;
  void update() override{
    glPushMatrix();
      glColor3f(R, G, B);
      glTranslatef(x+height/2, y+height/2, z*1.4 + 0.01);
      glScalef(0.4, 0.4, 1.4);
      glutSolidCube(height);
    glPopMatrix();

    glPushMatrix();
      glColor3f(R, G, B);
      glTranslatef(x+height/2, y+height/2, z*3 + 0.01);
      glScalef(0.1, 0.1, 0.1);
      glutSolidTorus(height, height*2, 32, 32);
    glPopMatrix();

    if(selected){
      glColor3f(0, 0, 0.8);
      glBegin(GL_QUADS);
        glVertex3d(x, y, 0.02);
        glVertex3d(x+1, y, 0.02);
        glVertex3d(x+1, y+1, 0.02);
        glVertex3d(x, y+1, 0.02);
      glEnd();
    }
  }

  void moving() override{
    selected = 1;
  }
};
