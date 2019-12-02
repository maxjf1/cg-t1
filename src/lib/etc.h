void initLight(int width, int height) {
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_DEPTH_TEST);

    GLfloat cor_luz[] = {1.0, 1.0, 1.0, 1.0};
    GLfloat posicao_luz[] = {(float) width, (float) height, 1000.0, 1.0};

    glLightfv(GL_LIGHT0, GL_AMBIENT, cor_luz);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, cor_luz);
    glLightfv(GL_LIGHT0, GL_SPECULAR, cor_luz);
    glLightfv(GL_LIGHT0, GL_POSITION, posicao_luz);
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
}

void setColor(float r, float g, float b) {
    glColor3f(r, g, b);

    GLfloat objeto_especular[] = {0.626, 0.626, 0.626, 1.0};
    GLfloat objeto_brilho[] = {90.0f};
    GLfloat objeto_ambient[] = {0.1, 0.1, 0.1, 1.0};
    GLfloat objeto_difusa[] = {r, g, b, 1.0};

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, objeto_ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, objeto_difusa);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, objeto_especular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, objeto_brilho);
}

/**
 * Corrige o alcance de um valor
 * @param circular se true, quando o valor ultrapassa o limite,ele é rotacionado para o inverso (minimo/maximo)
 * @param overflow Corrige valor de sobra ao realizar circular
 */
float fixRange(float value, float min, float max, bool circular = false, bool overflow = false) {
    if (value > max)
        return circular ? min + (overflow ? value - max : 0) : max;
    else if (value < min)
        return circular ? max - (overflow ? value - min : 0) : min;
    return value;
}

int signal(float value) {
    return value / fabs(value);
}

/// converte angulo em radiandos
float rad(float angle) {
    return angle * M_PI / 180;
}

/// converte radiando em angulo
float angle(float rad) {
    return rad * 180.0 / M_PI;
}

/// Cacula distancia ntre dois pontos
float calcDistance(float aX, float aY, float bX, float bY) {
    return sqrt(pow(aX - bX, 2) + pow(aY - bY, 2));
}

float vertexMod(vertex v) {
    return sqrt(
            pow(v.x, 2) + pow(v.y, 2)
    );
}

float vertexProduto(vertex a, vertex b) {
    return a.x * b.x + a.y * b.y;
}

float vertexAngleDifference(vertex a, vertex b) {
    float ang = angle(acos(
            vertexProduto(a, b) /
            vertexMod(a) * vertexMod(b)
    ));

    return ang;
}

/// converte vetor em angulo
float vertexAngle(vertex v) {
    float a = angle(acos(v.x / sqrt(pow(v.x, 2) + pow(v.y, 2))));
//    float a = angle(vertexAngleDifference(v, vertex(1, 0)));

    if (v.y < 0)
        a = 360 - a;
    return a;
}

/// Move vetor
vertex moveVertex(vertex v, vertex movement) {
    return vertex(v.x + movement.x, v.y + movement.y, v.z + movement.z);
}

vertex moveVertex(vertex v, float movement) {
    return moveVertex(v, vertex(movement, movement, movement));
}

/// Verifica se ponto está em um quadrado
bool inSquare(vertex point, vertex begin, vertex end) {
    for (int i = 0; i < 2; ++i)
        if (point.at(i) < begin.at(i) || point.at(i) > end.at(i))
            return false;
    return true;
}

/// rotaciona vetor
vertex rotateVertexAngle(vertex v, float rotation) {
    rotation = rad(rotation);
    vertex result;
    result.x = v.x * cos(rotation) - v.y * sin(rotation);
    result.y = v.x * sin(rotation) + v.y * cos(rotation);
    return result;
}

float setColor(float w) {
    setColor(w, w, w);
}

/// glTranslate para vetores
void glTranslatev(vertex v) {
    glTranslatef(v.x, v.y, v.z);
}

void glNormalv(vertex v) {
    glNormal3f(v.x, v.y, v.z);
}

void glVertexv(vertex v) {
    glVertex3f(v.x, v.y, v.z);
}

/// gera numero aleatorio entre 0 e 1
float gameRandom() {
    return 1.0 / (float) RAND_MAX * (float) rand();
}

float shouldRenderFrame() {
    float t, frameTime;
    static float tLast = 0.0;
    // Get elapsed time
    t = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
    // Calculate frame time
    frameTime = t - tLast;

    // Check if the desired frame time was achieved. If not, skip animation.
    if (frameTime <= 1.0 / FPS)
        return 0;

    tLast = t;

    return frameTime;
}

