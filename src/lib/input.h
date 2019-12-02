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