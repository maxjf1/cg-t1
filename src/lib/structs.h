glcTexture textureManager;

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

    brick(float r, float g, float b, int health = 1) : health(health) { this->setColor(r, g, b); }

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
        float yPos = -STAGE_OFFSET + (BOARD_HEIGHT / 2) - (getBrickHeight() * y);
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

class textureHandler {
public:
    int TEX_ID = NULL;
    float multiplierS, multiplierT, offsetS, offsetT;

    textureHandler(int id = NULL, float multiplierS = 1, float multiplierT = 1, float offsetS = 0, float offsetT = 0) :
            TEX_ID(id), multiplierS(multiplierS), multiplierT(multiplierT), offsetS(offsetS), offsetT(offsetT) {}

    void bind() {
        if (TEX_ID != NULL)
            textureManager.Bind(TEX_ID);
    }

    void texCoord(float s, float t) {
        glTexCoord2f(offsetS + s * multiplierS,
                     offsetT + t * multiplierT);
    }

    void disable() {
        textureManager.Disable();
    }
};