#include <GL/freeglut.h>
class peca{
public:
  GLfloat x, y, z = 0.5, tamanho, R, G, B;
  // x, y, z = posicao no mundo
  // tamnho...
  // R, G, B...
  peca(){}
  peca(GLfloat x1, GLfloat y1, GLfloat z1, GLfloat tam, GLfloat R1, GLfloat G1, GLfloat B1){
    this->x = x1;
    this->y = y1;
    this->z = z1;
    this->tamanho = tam;
    this->R = R1;
    this->G = G1;
    this->B = B1;
  }
};

class peao: public peca{
public:
  peao():peca(){};
  peao(GLfloat x1, GLfloat y1, GLfloat z1, GLfloat tam, GLfloat R1, GLfloat G1, GLfloat B1):peca(x1, y1, z1, tam, R1, G1, B1){

  };
  void update(){
    glPushMatrix();
      glColor3f(R, G, B);
      glTranslatef(x+tamanho/2, y+tamanho/2, z*1.5);
      glScalef(0.9, 0.9, 1.5);
      glutSolidCube(tamanho);
    glPopMatrix();
  }
};
