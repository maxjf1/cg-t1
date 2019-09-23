/// CONSTS
const float FPS = 60;
const float ANGLE_3D = 30;
const float BOARD_WIDTH = 7;
const float BOARD_HEIGHT = 4;
const float BOARD_DEPTH = 0.2;
const float STAGE_HEIGHT = BOARD_HEIGHT / 2;
const float BAR_WIDTH = BOARD_WIDTH / 4;
float BALL_RADIUS = 0.1;
float VELOCITY = 0.7 / (BALL_RADIUS * 100);

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
int fullscreen[] = {0, 0, 0};

/// Globals
float rotationX = 0, rotationY = 0;
float barPosition = 0;
bool PAUSED = false;
bool STARTED = false;
bool FREECAM = false;
bool ORTHO = false;
vertex ballPosition;
vertex ballDirection;
brickGrid stage;
bool WIN = false;


// Renderiza tabuleiro
void drawBoard() {
    glPushMatrix();
    setColor(0.5);

    // sides
    for (int side = -1; side < 2; side+=2) {
        glPushMatrix();
        glTranslatef((BOARD_WIDTH / 2 + (BOARD_DEPTH / 2)) * side, -BOARD_HEIGHT / 2, 0);
        glScalef(BOARD_DEPTH, BOARD_HEIGHT * 2, BOARD_DEPTH);
        glutSolidCube(1);
        glPopMatrix();
    }

    // top
    glPushMatrix();
    glTranslatef(0, BOARD_HEIGHT / 2 + (BOARD_DEPTH / 2), 0);
    glScalef(BOARD_WIDTH + 2 * BOARD_DEPTH, BOARD_DEPTH, BOARD_DEPTH);
    glutSolidCube(1);
    glPopMatrix();
}

// renderiza tijolo
void drawBrick(brick b) {
    glPushMatrix();
    glTranslatev(b.getCenter());
    glScalef(b.getScale(0), b.getScale(1), b.getScale(2));
    glutSolidCube(1);
    // cube outline
    glScalef(1, 1, 1.01);
    setColor(0);
    glutWireCube(1);
    glPopMatrix();
}

// Renderiza a fase
void drawStage() {
    for (int i = 0; i < stage.getHeight(); ++i) {
        for (int j = 0; j < stage.getWidth(); ++j) {
            brick b = stage.grid[i][j];
            if (!b.health) continue;
            setColor(b.color[0], b.color[1], b.color[2]);
            drawBrick(b);
        }
    }
}

// renderiza o rebatedor
void drawBar() {
    static GLUquadric *qobj = gluNewQuadric();
    glPushMatrix();
    setColor(0.1, 0.8, 0.1);
    glTranslatef(barPosition, -BOARD_HEIGHT / 2 - BOARD_DEPTH / 2 + BOARD_DEPTH * 0.2, 0);
    glScalef(BAR_WIDTH, BOARD_DEPTH * 0.8, BOARD_DEPTH);
    glutSolidCube(1);
    glPopMatrix();
}

// Renderiza Bola
void drawBall() {
    glPushMatrix();
    setColor(1, 1, 0);
    glTranslatev(ballPosition);
    glutSolidSphere(BALL_RADIUS, 100, 100);
    glPopMatrix();
}

// renderiza indicador de direção
void drawArrow() {
    glPushMatrix();
    setColor(1, 0, 0);
    glTranslatev(ballPosition);
    glRotatef(vertexAngle(ballDirection) - 90, 0, 0, 1);
    glTranslatef(0, BALL_RADIUS * 1.2, 0);
    glRotatef(-90, 1, 0, 0);
    glutSolidCone(BALL_RADIUS / 2, BALL_RADIUS * 2, 100, 100);
    glPopMatrix();
}


void display(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(0, 0, 4.5, 0, 0, 0, 0, 1, 0);

    glRotatef(rotationY, 0.0, 1.0, 0.0);
    glRotatef(rotationX, 1.0, 0.0, 0.0);

    glPushMatrix();

    drawBoard();
    drawStage();
    drawBar();
    drawBall();
    if (!STARTED)
        drawArrow();

    glPopMatrix();

    glutSwapBuffers();
}

// Gerenciamento de estado

// Reseta bola
void resetBall() {
    ballPosition = vertex(barPosition, -BOARD_HEIGHT / 2 + (BALL_RADIUS * 1.1)); // arbitrary initial distance
    ballDirection = rotateVertexAngle(vertex(0, 1), -30);
    PAUSED = STARTED = FREECAM = false;
}

// reseta o jogo
void resetState() {
    barPosition = 0;
    resetBall();
    // cria fase padrão
    stage = brickGrid(10, 6);
    for (int i = 0; i < stage.getWidth(); ++i)
        for (int j = 0; j < stage.getHeight(); ++j)
            if (WIN ?
                gameRandom() < 0.4 : // RANDOM STAGE
                (j + i) % 2 == 0) // CHESS STAGE
                continue;
            else
                stage.setBrick(i, j, brick(
                        gameRandom(),
                        gameRandom(),
                        gameRandom()
                ));

}

/// Atualiza estado da aplicação
void updateState() {
    if (FREECAM || PAUSED || !STARTED) return;

    const float xLimit = BOARD_WIDTH / 2 - BALL_RADIUS,
            yLimit = BOARD_HEIGHT / 2 - BALL_RADIUS,
            BALL_HITBOX = BALL_RADIUS * 0.5;

    // movimento da bola
    ballPosition.x = fixRange(ballPosition.x + VELOCITY * ballDirection.x, -xLimit, xLimit);
    ballPosition.y = fixRange(ballPosition.y + VELOCITY * ballDirection.y, -yLimit, yLimit);

    bool hasColision = false;
    // loop dos tijolos
    for (int i = 0; i < stage.getHeight(); ++i) {
        if (hasColision) break;
        for (int j = 0; j < stage.getWidth(); ++j) {
            if (hasColision) break;

            brick *b = &stage.grid[i][j];
            if (!b->health) continue; // sem vida
            // pre-colisão
            if (!inSquare(
                    ballPosition,
                    moveVertex(b->position[0], -BALL_HITBOX),
                    moveVertex(b->position[1], BALL_HITBOX)))
                continue;

            // Verifica se a bola esta em algum dos lados do tijolo
            // bottom
            if (inSquare(
                    ballPosition,
                    moveVertex(b->position[0], vertex(-BALL_HITBOX, -BALL_HITBOX)),
                    moveVertex(b->position[1], vertex(BALL_HITBOX, -stage.getBrickHeight() + VELOCITY))
            )) {
                ballDirection.y = -fabs(ballDirection.y);
                hasColision = true;
            }

            // top
            if (inSquare(
                    ballPosition,
                    moveVertex(b->position[0], vertex(-BALL_HITBOX, stage.getBrickHeight() - VELOCITY)),
                    moveVertex(b->position[1], vertex(BALL_HITBOX, BALL_HITBOX))
            )) {
                ballDirection.y = fabs(ballDirection.y);
                hasColision = true;
            }

            // left
            if (inSquare(
                    ballPosition,
                    moveVertex(b->position[0], vertex(-BALL_HITBOX, -BALL_HITBOX)),
                    moveVertex(b->position[1], vertex(VELOCITY - stage.getBrickWidth(), BALL_HITBOX))
            )) {
                ballDirection.x = -fabs(ballDirection.x);
                hasColision = true;
            }

            // right
            if (inSquare(
                    ballPosition,
                    moveVertex(b->position[0], vertex(-VELOCITY + stage.getBrickWidth(), -BALL_HITBOX)),
                    moveVertex(b->position[1], vertex(BALL_HITBOX, BALL_HITBOX))
            )) {
                ballDirection.x = fabs(ballDirection.x);
                hasColision = true;
            }

            if(hasColision)
                b->hit();
        }
    }

    // verificação de vitória - fase 2
    if (hasColision && stage.isFinished()) {
        WIN = true;
        BALL_RADIUS = 0.3;
        VELOCITY = 0.7 / (BALL_RADIUS * 100);
        resetState();
    }

    // Colisão  com paredes
    if (fabs(ballPosition.x) == xLimit)
        ballDirection.x *= -1;
    if (ballPosition.y == yLimit)
        ballDirection.y *= -1;
    // Colisão com rebatedor
    if (ballPosition.y == -yLimit) {
        if (ballPosition.x >= barPosition - BAR_WIDTH / 2 && ballPosition.x < barPosition + BAR_WIDTH / 2)
            ballDirection.y *= -1;
        else
            resetBall();
    }

}

// fullscreen handle
void specialKeyboard(int key, int x, int y) {
    if (key != GLUT_KEY_F12) return;

    if (fullscreen[0])
        glutReshapeWindow(fullscreen[1], fullscreen[2]);
    else {
        fullscreen[1] = width;
        fullscreen[2] = height;
        glutFullScreen();
    }
    fullscreen[0] = (fullscreen[0] + 1) % 2;
}

// Corrige perspectiva
void fixPerspective() {
    int w = width, h = height;
    const float ortho = 2.54;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    if (ORTHO) {
        // glOrtho(-orho, orho, -orho, orho, -200, 200);
        if (width <= height)
            glOrtho(-ortho, ortho, -ortho * h / w, ortho * h / w, -100, 100);
        else
            glOrtho(-ortho * w / h, ortho * w / h, -ortho, ortho, -100, 100);
    } else
        gluPerspective(60, (GLfloat) w / (GLfloat) h, 0.01, 200);
}

// atualiza perspectiva
void setPerspective(bool ortho = ORTHO) {
    ORTHO = ortho;
    rotationY = 0;
    rotationX = ORTHO ? 0 : -ANGLE_3D;
    fixPerspective();
}

/// Functions
// proporção
void reshape(int w, int h) {
    width = w;
    height = h;

    glViewport(0, 0, (GLsizei) w, (GLsizei) h);
    fixPerspective();
}

void init(void) {
    glEnable(GL_NORMALIZE);
    initLight(width, height); // Função extra para tratar iluminação.
    setPerspective();
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

    tLast = t;

    updateState();
    glutPostRedisplay();
}

// move mouse para centro
void resetMouse() {
    glutWarpPointer(width / 2, height / 2);
    glutSetCursor(GLUT_CURSOR_NONE);
}

// movimenta rebatedor
void mouseMotion(int x, int _) {
    if (PAUSED) return;
    float motion = (x - width / 2) * 0.005;
    if (!motion) return;

    barPosition = fixRange(barPosition + motion, -BOARD_WIDTH / 2 + BAR_WIDTH / 2, BOARD_WIDTH / 2 - BAR_WIDTH / 2);
    if (!STARTED)
        ballPosition.x = barPosition;
    resetMouse();
}

// movimenta câmera
void camMotion(int x, int y) {
    if (ORTHO) return;
    x = x - width / 2;
    y = y - height / 2;
    if (!x && !y) return;

    rotationX += y;
    rotationY += x;

    resetMouse();
}

// movimenta direção
void mouseScroll(int button, int _, int __, int ___) {
    if (button == GLUT_LEFT_BUTTON) {
        STARTED = true;
    } else if (!STARTED && (button == 3 || button == 4)) {
        int direction = -1 * (((button - 3) * 2) - 1);

        ballDirection = rotateVertexAngle(ballDirection, 1 * direction);
    }
}

/// Entradas do teclado
void keyboard(unsigned char key, int x = 0, int y = 0) {
    switch (tolower(key)) {
        case 'p':
            if (!FREECAM)
                setPerspective(!ORTHO);
            break;
        case 'c':
            if (ORTHO) break;
            FREECAM = !FREECAM;
            glutMotionFunc(FREECAM ? camMotion : mouseMotion);
            glutPassiveMotionFunc(FREECAM ? camMotion : mouseMotion);
            setPerspective();
            break;
        case ' ':
            PAUSED = !FREECAM && STARTED && !PAUSED;
            break;
        case 'r':
            resetState();
            break;
        case 27:
            exit(0);
    }
}

int main(int argc, char **argv) {
    resetState();
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(1000, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow(argv[0]);
    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMouseFunc(mouseScroll);
    glutMotionFunc(mouseMotion);
    glutPassiveMotionFunc(mouseMotion);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeyboard);
    glutIdleFunc(idle);
    glutMainLoop();
    fixPerspective();
    return 0;
}