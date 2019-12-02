// Corrige perspectiva
void fixPerspective() {
    int w = width, h = height;
    const float ortho = 2.54;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    if (ORTHO) {
        glDisable(GL_LIGHTING);
        // glOrtho(-orho, orho, -orho, orho, -200, 200);
        if (width <= height)
            glOrtho(-ortho, ortho, -ortho * h / w, ortho * h / w, -100, 100);
        else
            glOrtho(-ortho * w / h, ortho * w / h, -ortho, ortho, -100, 100);
    } else {
        glEnable(GL_LIGHTING);
        gluPerspective(60, (GLfloat) w / (GLfloat) h, 0.01, 200);
    }
}

// atualiza perspectiva
void setPerspective(bool ortho = ORTHO) {
    ORTHO = ortho;
    rotationY = 0;
    rotationX = ORTHO ? 0 : -ANGLE_3D;
    fixPerspective();
}
