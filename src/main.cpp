/// CONSTS
const float FPS = 60;
const float BOARD_WIDTH = 5;
const float BOARD_HEIGHT = 4;
const float BOARD_DEPTH = 0.2;
const float BRICK_WIDTH = BOARD_WIDTH / 7;
const float BRICK_HEIGHT = BOARD_DEPTH;
const float STAGE_HEIGHT = BOARD_HEIGHT / 2;

#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <math.h>
#include <float.h>
#include <iostream>

#include "lib/extras.h"
#include "lib/etc.h"

using namespace std;


/// SETTINGS
int width, height;
float viewAngle = 0;

/// Globals
float zdist = 5.0;
float rotationX = 0.0, rotationY = 0.0;
int last_x, last_y;

brickGrid stage(5, 4);


void drawBoard() {
    glPushMatrix();

//    setColorBase();

    setColor(0.5);

    // left
    glPushMatrix();
    glTranslatef(-BOARD_WIDTH / 2 - (BOARD_DEPTH / 2), -BOARD_HEIGHT / 2, 0);
    glScalef(BOARD_DEPTH, BOARD_HEIGHT * 2, BOARD_DEPTH);
    glutSolidCube(1);
    glPopMatrix();

    // right
    glPushMatrix();
    glTranslatef(BOARD_WIDTH / 2 + (BOARD_DEPTH / 2), -BOARD_HEIGHT / 2, 0);
    glScalef(BOARD_DEPTH, BOARD_HEIGHT * 2, BOARD_DEPTH);
    glutSolidCube(1);
    glPopMatrix();

    // top
    glPushMatrix();
    glTranslatef(0, BOARD_HEIGHT / 2 + (BOARD_DEPTH / 2), 0);
    glScalef(BOARD_WIDTH + 2 * BOARD_DEPTH, BOARD_DEPTH, BOARD_DEPTH);
    glutSolidCube(1);
    glPopMatrix();
}


void drawBrick(brick b) {
    glPushMatrix();

    setColor(b.color[0], b.color[1], b.color[2]);

    glNormal3f(0, 0, -1);
    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(b.position[0].x, b.position[0].y, b.position[0].z);
    glVertex3f(b.position[1].x, b.position[0].y, b.position[0].z);
    glVertex3f(b.position[1].x, b.position[1].y, b.position[0].z);
    glVertex3f(b.position[0].x, b.position[1].y, b.position[0].z);
    glEnd();

    glNormal3f(0, 0, 1);
    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(b.position[0].x, b.position[1].y, b.position[1].z);
    glVertex3f(b.position[1].x, b.position[1].y, b.position[1].z);
    glVertex3f(b.position[1].x, b.position[0].y, b.position[1].z);
    glVertex3f(b.position[0].x, b.position[0].y, b.position[1].z);
    glEnd();

    glPopMatrix();
}

void drawStage() {
    for (int i = 0; i < stage.height; ++i) {
        for (int j = 0; j < stage.width; ++j) {
            drawBrick(stage.grid[i][j]);
        }
    }
}

void display(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(0, viewAngle, 4, 0, 0, 0, 0, 1, 0);

    glRotatef(rotationY, 0.0, 1.0, 0.0);
    glRotatef(rotationX, 1.0, 0.0, 0.0);

    glPushMatrix();

    // TODO: renders
    drawBoard();

    drawStage();


    glPopMatrix();

    glutSwapBuffers();
}

/// Atualiza estado da aplicação
void updateState() {
    // TODO: update states
}

/// Entradas do teclado
void keyboard(unsigned char key, int x, int y) {
    switch (tolower(key)) {
        case 'p':
//            angulo = (angulo == 0) ? -30 : 0;
            break;
        case 27:
            exit(0);
    }
}

/// Functions
void reshape(int w, int h) {
    width = w;
    height = h;

    glViewport(0, 0, (GLsizei) w, (GLsizei) h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (GLfloat) w / (GLfloat) h, 0.01, 200.0);
}

void init(void) {
    initLight(width, height); // Função extra para tratar iluminação.
}

void idle() {
    float t, frameTime;
    static float tLast = 0.0;
    // Get elapsed time
    t = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
    // Calculate frame time
    frameTime = t - tLast;

    // Check if the desired frame time was achieved. If not, skip animation.
    if (frameTime <= 1.0 / FPS)
        return;

    updateState();
    glutPostRedisplay();
}

// Motion callback TODO: remove for debugging
void motion(int x, int y) {
    rotationX += (float) (y - last_y);
    rotationY += (float) (x - last_x);

    last_x = x;
    last_y = y;
}

// Mouse callback
void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        last_x = x;
        last_y = y;
    }
    if (button == 3) // Scroll up
    {
        zdist += 1.0f;
    }
    if (button == 4) // Scroll Down
    {
        zdist -= 1.0f;
    }
}

int main(int argc, char **argv) {
    for (int i = 0; i < stage.getWidth(); ++i)
        for (int j = 0; j < stage.getHeight(); ++j)
            stage.setBrick(i, j, brick(
                    1.0/stage.getWidth()*i,
                    1.0/(stage.getWidth() + stage.getHeight())*(i+j),
                    1.0/stage.getHeight()*j
            ));

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(1000, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow(argv[0]);
    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutKeyboardFunc(keyboard);
    glutIdleFunc(idle);
    glutMainLoop();
    return 0;
}