// Renderiza tabuleiro
void drawBoard() {
    glPushMatrix();
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
    setColor(0.1, 0.8, 0.1);
    maxSolidCurve(BAR_WIDTH, BAR_HEIGHT, BAR_SLICES, BOARD_DEPTH);
    glPopMatrix();
}

/** @deprecated */
void drawBar(int x) {
    glPushMatrix();
    setColor(0.1, 0.8, 0.1);
    glTranslatef(barPosition, -BOARD_HEIGHT / 2 - BOARD_DEPTH / 2 + BOARD_DEPTH * 0.2, 0);
    glScalef(BAR_WIDTH, BAR_HEIGHT * 0.8, BOARD_DEPTH);
    coolCube(1);
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
