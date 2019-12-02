/// Menu methods

vertex menuShake(30, 0);

void menuDisplay() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(0, 0, 4.5, 0, -0.3, 0, 0, 1, 0);

    glPushMatrix();

    glPushMatrix();
    glTranslatef(0, 1, 0);
    drawTexture(TEX_LOGO);
    glTranslatef(0, -2.7, 0);
    drawTexture(TEX_MENU);
    glPopMatrix();

    setColor(1);

    glRotatef(menuShake.x, 1.0, 0.0, 0.0);
    glRotatef(menuShake.y, 0.0, 1.0, 0.0);
    drawSkybox();
    glPopMatrix();
    glutSwapBuffers();
}

void menuIdle() {
    float frameTime = shouldRenderFrame();
    if (!frameTime) return;
    menuShake = rotateVertexAngle(menuShake, frameTime * 11.8);

    glutPostRedisplay();
}


/// Entradas do teclado
void menuKeyboard(unsigned char key, int x = 0, int y = 0) {
    switch (tolower(key)) {
        case 27:
            exit(0);
    }
}

// movimenta direção
void menuMouse(int button, int out, int __, int ___) {
    if (button == GLUT_LEFT_BUTTON && out){
        setup();
        STARTED = false;
    }

}

void menuSetup() {
    glutDisplayFunc(menuDisplay);
    glutMouseFunc(menuMouse);
    glutMotionFunc(NULL);
    glutPassiveMotionFunc(NULL);
    glutKeyboardFunc(menuKeyboard);
    glutSpecialFunc(specialKeyboard);
    glutIdleFunc(menuIdle);
}