void coolCube(float size) {
    glutSolidCube(size);
    setColor(0);
    glutWireCube(size * 1.001);
}

void texCube(vertex begin, vertex end, textureHandler tex = textureHandler()) {
    tex.bind();
    /// sides
    glBegin(GL_TRIANGLE_FAN);
    glNormal3f(0, 0, -1);
    tex.texCoord(0, 0);
    glVertex3f(end.x, begin.y, begin.z);
    tex.texCoord(1, 0);
    glVertex3f(begin.x, begin.y, begin.z);
    tex.texCoord(1, 1);
    glVertex3f(begin.x, end.y, begin.z);
    tex.texCoord(0, 1);
    glVertex3f(end.x, end.y, begin.z);
    glEnd();

    glBegin(GL_TRIANGLE_FAN);
    glNormal3f(1, 0, 0);
    tex.texCoord(0, 0);
    glVertex3f(end.x, begin.y, end.z);
    tex.texCoord(1, 0);
    glVertex3f(end.x, begin.y, begin.z);
    tex.texCoord(1, 1);
    glVertex3f(end.x, end.y, begin.z);
    tex.texCoord(0, 1);
    glVertex3f(end.x, end.y, end.z);
    glEnd();

    glBegin(GL_TRIANGLE_FAN);
    glNormal3f(0, 0, 1);
    tex.texCoord(0, 0);
    glVertex3f(begin.x, begin.y, end.z);
    tex.texCoord(1, 0);
    glVertex3f(end.x, begin.y, end.z);
    tex.texCoord(1, 1);
    glVertex3f(end.x, end.y, end.z);
    tex.texCoord(0, 1);
    glVertex3f(begin.x, end.y, end.z);
    glEnd();

    glBegin(GL_TRIANGLE_FAN);
    glNormal3f(-1, 0, 0);
    tex.texCoord(0, 0);
    glVertex3f(begin.x, begin.y, begin.z);
    tex.texCoord(1, 0);
    glVertex3f(begin.x, begin.y, end.z);
    tex.texCoord(1, 1);
    glVertex3f(begin.x, end.y, end.z);
    tex.texCoord(0, 1);
    glVertex3f(begin.x, end.y, begin.z);
    glEnd();
    /// top
    glBegin(GL_TRIANGLE_FAN);
    glNormal3f(0, 1, 0);
    tex.texCoord(0, 0);
    glVertex3f(end.x, end.y, begin.z);
    tex.texCoord(1, 0);
    glVertex3f(begin.x, end.y, begin.z);
    tex.texCoord(1, 1);
    glVertex3f(begin.x, end.y, end.z);
    tex.texCoord(0, 1);
    glVertex3f(end.x, end.y, end.z);
    glEnd();
    /// bottom
    glBegin(GL_TRIANGLE_FAN);
    glNormal3f(0, 1, 0);
    tex.texCoord(0, 0);
    glVertex3f(begin.x, begin.y, end.z);
    tex.texCoord(1, 0);
    glVertex3f(end.x, begin.y, end.z);
    tex.texCoord(1, 1);
    glVertex3f(end.x, begin.y, begin.z);
    tex.texCoord(0, 1);
    glVertex3f(begin.x, begin.y, begin.z);
    glEnd();
    tex.disable();
}

void texCube(vertex sizes, textureHandler tex = textureHandler()) {
    glPushMatrix();
    glTranslatef(-sizes.x / 2, -sizes.y / 2, -sizes.z / 2);
    texCube(vertex(0, 0, 0), sizes, tex);
    glPopMatrix();
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
