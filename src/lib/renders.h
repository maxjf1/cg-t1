#include "shapes.h"

void drawSkybox() {
    const float bSize = 100.1;

    glPushMatrix();
    glDisable(GL_LIGHTING);
    setColor(1);
    textureManager.Bind(TEX_SKYBOX);

    if (ORTHO) {
        float slice = 1.0 / 3;
        float w = (float) width / 100, h = (float) height / 200;
        glBegin(GL_TRIANGLE_FAN);

        glNormal3f(0, 0, 1);

        glTexCoord2f(0.1, slice);
        glVertex3f(-w, -h, 0);

        glTexCoord2f(0.9, slice);
        glVertex3f(w, -h, 0);

        glTexCoord2f(0.9, slice * 2);
        glVertex3f(w, h, 0);

        glTexCoord2f(0.1, slice * 2);
        glVertex3f(-w, h, 0);

        glEnd();

        textureManager.Disable();
        glPopMatrix();
        return;
    }

    glRotatef(90, 1, 0, 0);
    glTranslatef(0, 0, -bSize);

    // front and back
    for (int j = 0; j < 2; ++j) {
        glBegin(GL_TRIANGLE_FAN);

        glNormal3f(0, 0, 1);

        glTexCoord2f(0.25, 2.0 / 3 - j * 2.0 / 3);
        glVertex3f(-bSize, -bSize, 0);

        glTexCoord2f(0.50, 2.0 / 3 - j * 2.0 / 3);
        glVertex3f(bSize, -bSize, 0);

        glTexCoord2f(0.50, 1.0 - j * 2.0 / 3);
        glVertex3f(bSize, bSize, 0);

        glTexCoord2f(0.25, 1.0 - j * 2.0 / 3);
        glVertex3f(-bSize, bSize, 0);

        glEnd();

        glTranslatef(0, 0, bSize);
        glRotatef(180 - 90 * j, 1, 0, 0);
        glTranslatef(0, 0, -bSize);
    }

    // 4 lateral faces
    for (int i = 0; i < 4; ++i) {
        float ti = ((i + 3 - i * 2) + 1) % 4;
        glTranslatef(-bSize, 0, bSize);
        glRotatef(90, 0, 1, 0);
        glBegin(GL_TRIANGLE_FAN);

        glNormal3f(0, 0, 1);

        glTexCoord2f(1.0 / 4 * ti, 1.0 / 3);
        glVertex3f(-bSize, -bSize, 0);

        glTexCoord2f(1.0 / 4 * (ti + 1), 1.0 / 3);
        glVertex3f(bSize, -bSize, 0);

        glTexCoord2f(1.0 / 4 * (ti + 1), 2.0 / 3);
        glVertex3f(bSize, bSize, 0);

        glTexCoord2f(1.0 / 4 * ti, 2.0 / 3);
        glVertex3f(-bSize, bSize, 0);

        glEnd();
    }
    glEnable(GL_LIGHTING);
    textureManager.Disable();
    glPopMatrix();
}

// Renderiza tabuleiro
void drawBoard() {
    setColor(0.5);

    // sides
    for (int side = -1; side < 2; side += 2) {
        glPushMatrix();
        glTranslatef((BOARD_WIDTH / 2 + (BOARD_DEPTH / 2)) * side, -BOARD_HEIGHT / 2, 0);
        glScalef(BOARD_DEPTH, BOARD_HEIGHT * 2, BOARD_DEPTH);
        glutSolidCube(1);
        glPopMatrix();

        // Board Curves
        glPushMatrix();
        glTranslatef(side * BOARD_WIDTH / 2, 0, 0);
        glRotatef(side * 90, 0, 0, 1);
        maxSolidCurve(BOARD_NOTCH_WIDTH, BOARD_NOTCH_HEIGHT, 15, BOARD_DEPTH);
        glPopMatrix();
    }

    // top
    glPushMatrix();
    glTranslatef(0, BOARD_HEIGHT / 2 + (BOARD_DEPTH / 2), 0);
    glScalef(BOARD_WIDTH + 2 * BOARD_DEPTH, BOARD_DEPTH, BOARD_DEPTH);
    glutSolidCube(1);
    glPopMatrix();

    // top holes
    setColor(0.01);
    for (int side = -1; side < 2; side += 2) {
        glPushMatrix();
        glTranslatef(BOARD_WIDTH / 3 * side, BOARD_HEIGHT / 2, 0);
        glRotatef(180, 0, 0, 1);
        maxSolidCurve(BOARD_WIDTH / 5, 0.1, 15, BOARD_DEPTH);
        glPopMatrix();
    }
}

// renderiza tijolo
void drawBrick(brick b) {
    glPushMatrix();
    glTranslatev(b.getCenter());
    glScalef(b.getScale(0), b.getScale(1), b.getScale(2) * 1.1);
    coolCube(1);
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
    glPushMatrix();

    glTranslatef(barPosition, -BOARD_HEIGHT / 2 + BOARD_DEPTH * 0.5 - BAR_HEIGHT - BALL_RADIUS, 0);
    setColor(1);
    maxSolidCurve(BAR_WIDTH, BAR_HEIGHT, BAR_SLICES, BOARD_DEPTH, textureHandler(TEX_BARFRONT), textureHandler(TEX_BARSIDE, 1.5));
    textureManager.Disable();
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

/// Renderiza vidas
void drawLives() {
    for (int i = 0; i < LIVES; ++i) {
        glPushMatrix();
        setColor(1, 1, 0);
        glTranslatef(BOARD_WIDTH / 2 + BOARD_DEPTH * 2, -(i - 2) * BALL_RADIUS * 2.5, 0);
        glutSolidSphere(BALL_RADIUS, 100, 100);
        glPopMatrix();
    }
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
