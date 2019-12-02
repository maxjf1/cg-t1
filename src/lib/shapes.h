void coolCube(float size) {
    glutSolidCube(size);
    setColor(0);
    glutWireCube(size * 1.001);
}


void maxSolidCurve(float width, float height, int slices, float depth = BOARD_DEPTH,
                   textureHandler frontTex = textureHandler(), textureHandler sideTex = textureHandler()) {
    float SLICE_RADIUS = 180.0 / slices;
    frontTex.bind();
    vertex rotation(-1, 0), aux;
    glPushMatrix();
    glTranslatef(0, 0, -BOARD_DEPTH * 0.5);
    glBegin(GL_QUAD_STRIP);
    for (int i = 0; i < slices + 1; ++i) {
        glNormalv(rotateVertexAngle(rotation, SLICE_RADIUS / 2));

        frontTex.texCoord(1.0 / slices * i, 0);
        glVertex3f(
                width / 2 * rotation.x,
                height * rotation.y,
                0);

        frontTex.texCoord(1.0 / slices * i, 1);
        glVertex3f(
                width / 2 * rotation.x,
                height * rotation.y,
                depth
        );
        rotation = rotateVertexAngle(rotation, -SLICE_RADIUS);
    }
    frontTex.texCoord(1, 1);
    glEnd();
    // backface
    glBegin(GL_TRIANGLE_STRIP);
    glNormal3f(0, -1, 0);

    frontTex.texCoord(1, 0);
    glVertex3f(width / 2, 0, 0);
    frontTex.texCoord(1, 1);
    glVertex3f(width / 2, 0, depth);

    frontTex.texCoord(0, 0);
    glVertex3f(-width / 2, 0, 0);

    frontTex.texCoord(0, 1);
    glVertex3f(-width / 2, 0, depth);
    glEnd();
    textureManager.Disable();
    // sides
    sideTex.bind();
    rotation = vertex(-1, 0);
    for (int i = 0; i < slices; ++i) {
        aux = rotateVertexAngle(rotation, -SLICE_RADIUS);
        for (int j = 0; j < 2; ++j) {
            glBegin(GL_QUAD_STRIP);
            glNormal3f(0, 0, -1);

            sideTex.texCoord(1.0 / slices * i, 0);
            glVertex3f(width / 2 * rotation.x, 0, j * depth);

            sideTex.texCoord(1.0 / slices * i, rotation.y);
            glVertex3f(
                    width / 2 * rotation.x,
                    height * rotation.y,
                    j * depth);

            sideTex.texCoord(1.0 / slices * (i + 1), 0);
            glVertex3f(width / 2 * aux.x, 0, j * depth);
            sideTex.texCoord(1.0 / slices * (i + 1), aux.y);
            glVertex3f(
                    width / 2 * aux.x,
                    height * aux.y,
                    j * depth);
            glEnd();
        }
        rotation = aux;
    }
    textureManager.Disable();
    glPopMatrix();
}

void drawTexture(int id, int size = 2) {
    float ratio = textureManager.GetAspectRatio(id);
    glPushMatrix();
    glScalef(size, size, 0);
    glTranslatef(-ratio / 2, -0.5, 0);

    setColor(1);
    textureManager.Bind(id);
    glDisable(GL_LIGHTING);
    glBegin(GL_TRIANGLE_FAN);
    glNormal3f(0, 0, 1);

    glTexCoord2f(0, 0);
    glVertex3f(0, 0, 0);

    glTexCoord2f(1, 0);
    glVertex3f(ratio, 0, 0);

    glTexCoord2f(1, 1);
    glVertex3f(ratio, 1, 0);

    glTexCoord2f(0, 1);
    glVertex3f(0, 1, 0);

    glEnd();
    glEnable(GL_LIGHTING);
    textureManager.Disable();
    glPopMatrix();
}
