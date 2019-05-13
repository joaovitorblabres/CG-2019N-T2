#include <GL/freeglut.h>
#include <iostream>
using namespace std;

// 0 = NONE = VOID
// 1X = white pieces
// 2X = black pieces
// X = 1 PAWN; X = 2 ROOK; X = 3 KNIGHT; X = 4 BISHOP; X = 5 QUEEN; X = 6 KING
const int tab[8][8] = {
                        {22, 23, 24, 25, 26, 24, 23, 22},
                        {21, 21, 21, 21, 21, 21, 21, 21},
                        {0, 0, 0, 0, 0, 0, 0, 0},
                        {0, 0, 0, 16, 0, 0, 0, 0},
                        {0, 0, 0, 0, 0, 0, 0, 0},
                        {0, 0, 0, 0, 0, 0, 0, 0},
                        {11, 11, 11, 11, 11, 11, 11, 11},
                        {12, 13, 14, 16, 15, 14, 13, 12}
                      };

class piece{
public:
  GLint isMoved = 0;
  GLfloat RChange = 0.4;
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
    if(R1 == 1 && this->y < 6){
      this->isMoved = 1;
    }
    if(R1 == 0 && this->y > 1){
      this->isMoved = 1;
    }
  }

  virtual void update() = 0;
  virtual void select() = 0;
  virtual void moves(GLfloat R, GLfloat G, GLfloat B) = 0;
};

static int checkBorder(GLfloat xF, GLfloat yF){
	if(xF >= 0 && xF <= 7 && yF >= 0 && yF <= 7){
		return 1;
	}
	return 0;
}

static void drawVertex(GLfloat x, GLfloat y, GLfloat z, GLfloat R, GLfloat G, GLfloat B){
	glPushMatrix();
		glColor3f(R, G, B);
	  glBegin(GL_QUADS);
	    glVertex3d(x, y, 0.02);
	    glVertex3d(x+1, y, 0.02);
	    glVertex3d(x+1, y+1, 0.02);
	    glVertex3d(x, y+1, 0.02);
	  glEnd();
	glPopMatrix();
}

static void drawSolidCube(GLfloat x, GLfloat y, GLfloat z, GLfloat R, GLfloat G, GLfloat B, GLfloat sX, GLfloat sY, GLfloat sZ, GLfloat height){
	glPushMatrix();
	  glColor3f(R, G, B);
	  glTranslatef(x, y, z);
	  glScalef(sX, sY, sZ);
	  glutSolidCube(height);
  glPopMatrix();
}

static void itera(GLfloat x, GLfloat y, GLint movement[2], GLfloat G, GLfloat R){
	int yF, xF;
	for(yF = y, xF = x; yF <= 7 && xF <= 7 && yF >= 0 && xF >= 0; yF += movement[1], xF += movement[0]){
		if(tab[yF][xF] == 0){
			drawVertex(xF, yF, 0, R, 0.2, 0.2);
		}else if ((G == 1.0 ? tab[yF][xF] > 20 : tab[yF][xF] < 20)){
			drawVertex(xF, yF, 0, R, 0.2, 0.2);
			break;
		}else{
			break;
		}
	}
}

class pawn: public piece{
public:
  pawn(GLfloat x1, GLfloat y1, GLfloat z1, GLfloat tam, GLfloat R1, GLfloat G1, GLfloat B1):piece(x1, y1, z1, tam, R1, G1, B1){
    if(R1 == 1 && this->y != 6){
      this->isMoved = 1;
    }
    if(R1 == 0 && this->y != 1){
      this->isMoved = 1;
    }
  }
  GLint selected = 0;
  void update() override{
  	drawSolidCube(this->x+this->height/2, this->y+this->height/2, this->z*0.8 + 0.01, R, G, B, 0.5, 0.5, 0.8, this->height);

    glPushMatrix();
      glColor3f(R, G, B);
      glTranslatef(x+height/2, y+height/2, this->z*1.6 + 0.01);
      glutSolidSphere(0.3, 32, 32);
    glPopMatrix();

    this->RChange = (this->RChange > 1 ? 0.4 : this->RChange + 0.0005);
    if(this->selected == 1){
      drawVertex(x, y, 0, 0, 0, 0.8);
      moves(this->RChange, 0.2, 0.2);
    }
  }

  void select() override{
    this->selected = 1;
  }

  void moves(GLfloat R, GLfloat G, GLfloat B) override{
    if(this->R == 1.0){
      GLint xF = this->x;
      GLint yF = this->y-1;
      if(tab[yF][xF] == 0){
        drawVertex(x, y-1, 0, R, G, B);
      }
      if(this->isMoved == 0 && tab[yF-1][xF] == 0){
        drawVertex(x, y-2, 0, R, G, B);
      }
      if(tab[yF][xF-1] != 0 && tab[yF][xF-1] > 20 && xF-1 >= 0){
        drawVertex(x-1, y-1, 0, R, G, B);
      }
      if(tab[yF][xF+1] != 0 && tab[yF][xF-1] > 20 && xF-1 <= 7){
        drawVertex(x+1, y-1, 0, R, G, B);
      }
    }
    if(this->R == 0.0){
      GLint xF = this->x;
      GLint yF = this->y+1;
      if(tab[yF][xF] == 0){
        drawVertex(x, y+1, 0, R, G, B);
      }
      if(this->isMoved == 0 && tab[yF+1][xF] == 0){
        drawVertex(x, y+2, 0, R, G, B);
      }
      if(tab[yF][xF+1] != 0 && tab[yF][xF+1] < 20 && xF+1 <= 7){
        drawVertex(x+1, y+1, 0, R, G, B);
      }
      if(tab[yF][xF-1] != 0 && tab[yF][xF-1] < 20 && xF-1 >= 0){
        drawVertex(x-1, y+1, 0, R, G, B);
      }
    }
  }
};

class rook: public piece{
public:
  rook(GLfloat x1, GLfloat y1, GLfloat z1, GLfloat tam, GLfloat R1, GLfloat G1, GLfloat B1):piece(x1, y1, z1, tam, R1, G1, B1){};
  GLint selected = 0;
	GLint movements[4][2] = {{0, 1}, {1, 0}, {-1, 0}, {0, -1}};
  void update() override{
  	drawSolidCube(this->x+this->height/2, this->y+this->height/2, this->z*1.4 + 0.01, R, G, B, 0.8, 0.8, 0.2, this->height);

		drawSolidCube(this->x+this->height/2, this->y+this->height/2, z + 0.01, R, G, B, 0.5, 0.5, 1, this->height);

    this->RChange = (this->RChange > 1 ? 0.4 : this->RChange + 0.0005);
    if(this->selected == 1){
      drawVertex(x, y, 0, 0, 0, 0.8);
      moves(this->RChange, 0.2, 0.2);
    }
  }

  void select() override{
    selected = 1;
  }

  void moves(GLfloat R, GLfloat G, GLfloat B) override{
		for(int i = 0; i < 4; i++){
			itera(this->x + movements[i][0], this->y + movements[i][1], movements[i], this->G, R);
	  }
  }
};

class knight: public piece{
public:
  knight(GLfloat x1, GLfloat y1, GLfloat z1, GLfloat tam, GLfloat R1, GLfloat G1, GLfloat B1):piece(x1, y1, z1, tam, R1, G1, B1){};
  GLint selected = 0;
  void update() override{
		drawSolidCube(this->x+this->height/2, this->y+this->height/2, this->z*1.2 + 0.01, R, G, B, 0.5, 0.5, 1.2, this->height);

		drawSolidCube(this->x+this->height/2 , this->y+this->height/2 + (this->R == 0 ? 0.1 : 1*-0.1), this->z*1.8, R, G, B, 0.5, 0.9, 0.5, this->height);

		drawSolidCube(this->x+this->height/2 + (this->R == 0 ? 1*-0.2 : 1*0.2), this->y+this->height/2, this->z*2.2, R, G, B, 0.1, 0.2, 0.6, this->height);

		drawSolidCube(this->x+this->height/2 + (this->R == 0 ? 1*0.2 : 1*-0.2), this->y+this->height/2, this->z*2.2, R, G, B, 0.1, 0.2, 0.6, this->height);

		this->RChange = (this->RChange > 1 ? 0.4 : this->RChange + 0.0005);
    if(this->selected == 1){
      drawVertex(x, y, 0, 0, 0, 0.8);
      moves(this->RChange, 0.2, 0.2);
    }
  }

  void select() override{
    selected = 1;
  }

  void moves(GLfloat R, GLfloat G, GLfloat B) override{
	  GLint xF = this->x+1;
	  GLint yF = this->y+2;
	  if(((this->R == 1.0 ? tab[yF][xF] > 20 : tab[yF][xF] < 20) || tab[yF][xF] == 0) && checkBorder(xF, yF))	drawVertex(xF, yF, 0, R, G, B);
	  xF = this->x-1; yF = this->y+2;
	  if(((this->R == 1.0 ? tab[yF][xF] > 20 : tab[yF][xF] < 20) || tab[yF][xF] == 0) && checkBorder(xF, yF)) drawVertex(xF, yF, 0, R, G, B);
	  xF = this->x+2; yF = this->y+1;
	  if(((this->R == 1.0 ? tab[yF][xF] > 20 : tab[yF][xF] < 20) || tab[yF][xF] == 0) && checkBorder(xF, yF)) drawVertex(xF, yF, 0, R, G, B);
	  xF = this->x+2; yF = this->y-1;
	  if(((this->R == 1.0 ? tab[yF][xF] > 20 : tab[yF][xF] < 20) || tab[yF][xF] == 0) && checkBorder(xF, yF)) drawVertex(xF, yF, 0, R, G, B);

	  xF = this->x-1; yF = this->y-2;
	  if(((this->R == 1.0 ? tab[yF][xF] > 20 : tab[yF][xF] < 20) || tab[yF][xF] == 0) && checkBorder(xF, yF)) drawVertex(xF, yF, 0, R, G, B);
	  xF = this->x+1; yF = this->y-2;
	  if(((this->R == 1.0 ? tab[yF][xF] > 20 : tab[yF][xF] < 20) || tab[yF][xF] == 0) && checkBorder(xF, yF)) drawVertex(xF, yF, 0, R, G, B);
	  xF = this->x-2; yF = this->y+1;
	  if(((this->R == 1.0 ? tab[yF][xF] > 20 : tab[yF][xF] < 20) || tab[yF][xF] == 0) && checkBorder(xF, yF)) drawVertex(xF, yF, 0, R, G, B);
	  xF = this->x-2; yF = this->y-1;
	  if(((this->R == 1.0 ? tab[yF][xF] > 20 : tab[yF][xF] < 20) || tab[yF][xF] == 0) && checkBorder(xF, yF)) drawVertex(xF, yF, 0, R, G, B);
  }
};

class bishop: public piece{
public:
  bishop(GLfloat x1, GLfloat y1, GLfloat z1, GLfloat tam, GLfloat R1, GLfloat G1, GLfloat B1):piece(x1, y1, z1, tam, R1, G1, B1){};
  GLint selected = 0;
	GLint movements[4][2] = {{1, 1}, {1, -1}, {-1, 1}, {-1, -1}};
  void update() override{
  	drawSolidCube(this->x+this->height/2, this->y+this->height/2, this->z*1.2 + 0.01, R, G, B, 0.4, 0.4, 1.2, this->height);

		drawSolidCube(this->x+this->height/2, this->y+this->height/2, this->z*2.2 + 0.01, R, G, B, 0.6, 0.6, 0.3, this->height);

		drawSolidCube(this->x+this->height/2, this->y+this->height/2, this->z*2.9 + 0.01, R, G, B, 0.1, 0.1, 0.4, this->height);

		drawSolidCube(this->x+this->height/2, this->y+this->height/2, this->z*3 + 0.01, R, G, B, 0.4, 0.1, 0.1, this->height);

		this->RChange = (this->RChange > 1 ? 0.4 : this->RChange + 0.0005);
    if(this->selected == 1){
      drawVertex(x, y, 0, 0, 0, 0.8);
      moves(this->RChange, 0.2, 0.2);
    }
  }

  void select() override{
    selected = 1;
  }

  void moves(GLfloat R, GLfloat G, GLfloat B) override{
		for(int i = 0; i < 4; i++){
			itera(this->x + movements[i][0], this->y + movements[i][1], movements[i], this->G, R);
	  }
  }
};

class queen: public piece{
public:
  queen(GLfloat x1, GLfloat y1, GLfloat z1, GLfloat tam, GLfloat R1, GLfloat G1, GLfloat B1):piece(x1, y1, z1, tam, R1, G1, B1){};
  GLint selected = 0;
	GLint movements[8][2] = {{1, 1}, {1, -1}, {-1, 1}, {-1, -1}, {0, 1}, {1, 0}, {-1, 0}, {0, -1}};
  void update() override{
  	drawSolidCube(this->x+this->height/2, this->y+this->height/2, this->z*1.4 + 0.01, R, G, B, 0.4, 0.4, 1.4, this->height);

    glPushMatrix();
      glColor3f(R, G, B);
      glTranslatef(x+height/2, y+height/2, z*3.1 + 0.01);
      glScalef(0.1, 0.1, 0.1);
      glutSolidDodecahedron();
    glPopMatrix();

	this->RChange = (this->RChange > 1 ? 0.4 : this->RChange + 0.0005);
    if(this->selected == 1){
      drawVertex(x, y, 0, 0, 0, 0.8);
      moves(this->RChange, 0.2, 0.2);
    }
  }

  void select() override{
    selected = 1;
  }

	void moves(GLfloat R, GLfloat G, GLfloat B) override{
		for(int i = 0; i < 8; i++){
			itera(this->x + movements[i][0], this->y + movements[i][1], movements[i], this->G, R);
	  }
  }
};

class king: public piece{
public:
  king(GLfloat x1, GLfloat y1, GLfloat z1, GLfloat tam, GLfloat R1, GLfloat G1, GLfloat B1):piece(x1, y1, z1, tam, R1, G1, B1){};
  GLint selected = 0;
  void update() override{
  	drawSolidCube(this->x+this->height/2, this->y+this->height/2, this->z*1.4 + 0.01, R, G, B, 0.4, 0.4, 1.4, this->height);

    glPushMatrix();
      glColor3f(R, G, B);
      glTranslatef(x+height/2, y+height/2, z*3 + 0.01);
      glScalef(0.1, 0.1, 0.1);
      glutSolidTorus(height, height*2, 32, 32);
    glPopMatrix();

	this->RChange = (this->RChange > 1 ? 0.4 : this->RChange + 0.0005);
    if(this->selected == 1){
      drawVertex(x, y, 0, 0, 0, 0.8);
      moves(this->RChange, 0.2, 0.2);
    }
  }

  void select() override{
    selected = 1;
  }

  void moves(GLfloat R, GLfloat G, GLfloat B) override{
		int xF = this->x-1, yF;
		for(yF = this->y-1; yF <= this->y+1 || !checkBorder(xF, yF); yF++){
			for(xF = this->y-1; xF <= this->y+1 || !checkBorder(xF, yF); xF++){
				if(tab[yF][xF] == 0){
					drawVertex(xF, yF, 0, R, 0.2, 0.2);
				}else if ((G == 1.0 ? tab[yF][xF] > 20 : tab[yF][xF] < 20)){
					drawVertex(xF, yF, 0, R, 0.2, 0.2);
				}else{
					break;
				}
			}
		}
  }
};
