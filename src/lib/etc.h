// Vertex class
class vertex {
public:
    float x = 0, y = 0, z = 0;

    vertex(float x = 0, float y = 0, float z = 0) : x(x), y(y), z(z) {}
    // Coordenadas com numeros
    float at(int i) {
        switch (i) {
            case 0:
                return x;
            case 1:
                return y;
            case 2:
                return z;
        }
    }
    float setAt(int i, float val) {
        switch (i) {
            case 0:
                return x = val;
            case 1:
                return y = val;
            case 2:
                return z = val;
        }
    }
};

class brick {
public:
    float color[3] = {1, 1, 1}; // RGB
    int health = 1; // VIDA: 0 esta morto
    vertex position[2];

    brick(float c[3], vertex p[2], int health = 1) : health(health) {
        this->setColor(c[0], c[1], c[2]);
        position[0] = p[0];
        position[1] = p[1];
    }

    brick(float r, float g, float b) { this->setColor(r, g, b); }

    void setColor(float r, float g, float b) {
        color[0] = r;
        color[1] = g;
        color[2] = b;
    }

    // reduz vida tijolo
    void hit(int damage = 1) {
        health = health -= damage;
        if (health < 0) health = 0;
    }

    // obtém centro do tijolo
    vertex getCenter() {
        return vertex(
                (position[0].x + position[1].x) / 2,
                (position[0].y + position[1].y) / 2,
                (position[0].z + position[1].z) / 2
        );
    }

    float getScale(int i) {
        return position[1].at(i) - position[0].at(i);
    }

    brick(int health) : health(health) {}

    brick() {};
};

class brickGrid {
    int width = 0, height = 0;

public:
    brick **grid = NULL;

    brickGrid(int w, int h) {
        width = w;
        height = h;

        grid = new brick *[height];
        for (int i = 0; i < height; ++i) {
            grid[i] = new brick[width];
            for (int j = 0; j < width; ++j)
                grid[i][j] = brick(0);
        }

    }

    brickGrid() {}

    float getBrickWidth() {
        return BOARD_WIDTH / width;
    }

    float getBrickHeight() {
        return STAGE_HEIGHT / height;
    }

    brick setBrick(int x, int y, brick b) {
        float xPos = -(BOARD_WIDTH / 2) + (getBrickWidth() * x);
        float yPos = (BOARD_HEIGHT / 2) - (getBrickHeight() * y);
        b.position[0] = vertex(xPos, yPos - getBrickHeight(), -BOARD_DEPTH / 2);

        b.position[1] = vertex(
                xPos + getBrickWidth(),
                yPos,
                BOARD_DEPTH / 2
        );
        return grid[y][x] = b;
    }

    int getWidth() const {
        return width;
    }

    int getHeight() const {
        return height;
    }

    // Estagio concluido - tijolos quebrados
    bool isFinished() {
        for (int i = 0; i < height; ++i)
            for (int j = 0; j < width; ++j)
                if (grid[i][j].health) return false;
        return true;
    }

};


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

/// converte vetor em angulo
float vertexAngle(vertex v) {
    float a = angle(acos(v.x / sqrt(pow(v.x, 2) + pow(v.y, 2))));

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

/// gera numero aleatorio entre 0 e 1
float gameRandom() {
    return 1.0 / (float) RAND_MAX * (float) rand();
}