#include "configs.h"

#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <math.h>
#include <float.h>
#include <iostream>
using namespace std;

#include "lib/glctexture.cpp"
#include "lib/structs.h"
#include "lib/etc.h"

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
int LIVES = 5;
vertex ballPosition;
vertex ballDirection;
brickGrid stage;
bool WIN = false;


void setup();
void menuSetup();

#include "lib/state.h"
#include "lib/cam.h"
#include "lib/input.h"
#include "lib/renders.h"
#include "lib/menu.h"


void display(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(0, 0, 4.5, 0, -0.3, 0, 0, 1, 0);

    glRotatef(rotationY, 0.0, 1.0, 0.0);
    glRotatef(rotationX, 1.0, 0.0, 0.0);

    glPushMatrix();

    drawSkybox();

    drawBoard();
    setColor(1);
    drawStage();
    drawBar();
    drawBall();
    drawLives();
    if (!STARTED)
        drawArrow();

    glPopMatrix();

    glutSwapBuffers();
}

// Gerenciamento de estado


/// Atualiza estado da aplicação
void updateState(float frameTime) {
    if (FREECAM || PAUSED || !STARTED) return;

    const float xLimit = BOARD_WIDTH / 2 - BALL_RADIUS,
            yLimit = BOARD_HEIGHT / 2 - BALL_RADIUS,
            BALL_HITBOX = BALL_RADIUS * 0.5;

    // movimento da bola
    ballPosition.x = fixRange(ballPosition.x + VELOCITY * (frameTime * 1000 / 17) * ballDirection.x, -xLimit, xLimit);
    ballPosition.y = fixRange(ballPosition.y + VELOCITY * (frameTime * 1000 / 17) * ballDirection.y, -yLimit, yLimit);

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

            if (hasColision)
                b->hit();
        }
    }

    // verificação de vitória - fase 2
    if (hasColision && stage.isFinished()) {
        STAGE++;
        resetBall();
        generateStage();
    }

    handleBoardColision(xLimit, yLimit);

    handleBarColision(-yLimit);

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

    textureManager.SetNumberOfTextures(7);
    textureManager.SetWrappingMode(GL_REPEAT);
    textureManager.CreateTexture("./assets/textures/skybox.png", TEX_SKYBOX);
    textureManager.CreateTexture("./assets/textures/iron.png", TEX_BARSIDE);
    textureManager.CreateTexture("./assets/textures/title.png", TEX_MENU);
    textureManager.CreateTexture("./assets/textures/logo.png", TEX_LOGO);
    textureManager.CreateTexture("./assets/textures/stage.png", TEX_STAGE);
    textureManager.CreateTexture("./assets/textures/bar.png", TEX_BARFRONT);
    textureManager.CreateTexture("./assets/textures/brick.png", TEX_BRICK);
    textureManager.Disable();
}

void idle() {
    float frameTime = shouldRenderFrame();
    if (!frameTime) return;

    updateState(frameTime);
    glutPostRedisplay();
}

void setup() {
    glutDisplayFunc(display);
    glutMouseFunc(mouseScroll);
    glutMotionFunc(mouseMotion);
    glutPassiveMotionFunc(mouseMotion);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeyboard);
    glutIdleFunc(idle);
}


int main(int argc, char **argv) {
    resetState();
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(1000, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow(argv[0]);
    init();
    glutReshapeFunc(reshape);

    menuSetup();
    glutMainLoop();
    fixPerspective();
    return 0;
}