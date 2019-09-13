// Vertex class
class vertex {
public:
    float x = 0, y = 0, z = 0;

    vertex(float x, float y, float z = 0) : x(x), y(y), z(z) {}

    vertex() {};
};

class brick {
public:
    float color[3] = {1, 1, 1};
    int health = 1;
    vertex position[2];

    brick(float r, float g, float b) {
        color[0] = r;
        color[1] = 1;
        color[2] = b;
    }

    brick() {};
};

class brickGrid {
public:
    int width, height;
    brick **grid = NULL;

    brickGrid(int w, int h) {
        width = w;
        height = h;

        grid = new brick *[height];
        for (int i = 0; i < height; ++i) {
            grid[i] = new brick[width];
            for (int j = 0; j < width; ++j)
                grid[i][j] = brick();

        }

    }

    brick setBrick(int x, int y, brick b) const {
        float xPos = -(BOARD_WIDTH / 2) + (BOARD_WIDTH / width * x);
        float yPos = (BOARD_HEIGHT / 2) - (STAGE_HEIGHT / height * y);
        b.position[0] = vertex(xPos, yPos, -BOARD_DEPTH / 2);

        b.position[1] = vertex(
                xPos + BOARD_WIDTH / width,
                yPos - STAGE_HEIGHT / height,
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

};


/**
 * Corrige o alcance de um valor
 * @param circular se true, quando o valor ultrapassa o limite,ele é rotacionado para o inverso (minimo/maximo)
 */
float fixRange(float value, float min, float max, bool circular = false) {
    if (value > max)
        return circular ? min : max;
    else if (value < min)
        return circular ? max : min;
    return value;
}

/// converte angulo em radiandos
float rad(float angle) {
    return angle * M_PI / 180;
}

/// Cacula distancia ntre dois pontos
float calcDistance(float aX, float aY, float bX, float bY) {
    return sqrt(pow(aX - bX, 2) + pow(aY - bY, 2));
}

/// converte vetor em angulo
float vectorAngle(float ax, float ay) {
    return acos(ax * 1 + ay * 0) * (180.0 / M_PI);
}

float setColor(float w) {
    setColor(w, w, w);
}