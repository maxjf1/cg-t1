
void initLight(int width, int height)
{
    glEnable(GL_LIGHTING);                 // Habilita luz
    glEnable(GL_LIGHT0);                   // habilita luz 0
    glEnable(GL_DEPTH_TEST);

    // Cor da fonte de luz (RGBA)
    GLfloat cor_luz[]     = { 1.0, 1.0, 1.0, 1.0};
    // Posicao da fonte de luz. Ultimo parametro define se a luz sera direcional (0.0) ou tera uma posicional (1.0)
    GLfloat posicao_luz[] = { (float) width, (float)height, 1000.0, 1.0};

    // Define parametros da luz
    glLightfv(GL_LIGHT0, GL_AMBIENT, cor_luz);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, cor_luz);
    glLightfv(GL_LIGHT0, GL_SPECULAR, cor_luz);
    glLightfv(GL_LIGHT0, GL_POSITION, posicao_luz);

    // Quando a opção "two_side lighting" está ativada, o opengl inverte a orientação das normais
    // permitindo que tanto as faces externas quanto internas sejam iluminadas.
    // Essa opção é mais lenta se comparada a renderização de apenas um dos lados
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
}


void setColorBase(void)
{
    // Parametros comuns para os dois lados da superfície
    GLfloat objeto_especular[] = { 0.626, 0.626, 0.626, 1.0 };
    GLfloat objeto_brilho[]    = { 90.0f };
    GLfloat objeto_ambient[]   = { 0.1, 0.1, 0.1, 1.0 };

    // Material da faces 'frente' (amarelo)
    GLfloat objeto_difusa[]    = { 0.6, 0.6, 0.0, 1.0 };

    glMaterialfv(GL_FRONT, GL_AMBIENT, objeto_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, objeto_difusa);
    glMaterialfv(GL_FRONT, GL_SPECULAR, objeto_especular);
    glMaterialfv(GL_FRONT, GL_SHININESS, objeto_brilho);

    // Material das faces 'back' (vermelho)
    GLfloat objeto_difusa_back[]    = { 0.6, 0.0, 0.0, 1.0 };

    glMaterialfv(GL_BACK, GL_AMBIENT, objeto_ambient);
    glMaterialfv(GL_BACK, GL_DIFFUSE, objeto_difusa_back);
    glMaterialfv(GL_BACK, GL_SPECULAR, objeto_especular);
    glMaterialfv(GL_BACK, GL_SHININESS, objeto_brilho);
}


void setColor(float r, float g, float b)
{
    // Parametros comuns para os dois lados da superfície
    GLfloat objeto_especular[] = { 0.626, 0.626, 0.626, 1.0 };
    GLfloat objeto_brilho[]    = { 90.0f };
    GLfloat objeto_ambient[]   = { 0.1, 0.1, 0.1, 1.0 };

    // Material da faces 'frente' (amarelo)
    GLfloat objeto_difusa[]    = { r, g, b, 1.0 };

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, objeto_ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, objeto_difusa);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, objeto_especular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, objeto_brilho);
}