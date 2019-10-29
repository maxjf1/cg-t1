
// Reseta bola
void resetBall() {
    ballPosition = vertex(barPosition, -BOARD_HEIGHT / 2 + (BALL_RADIUS * 1.1)); // arbitrary initial distance
    ballDirection = rotateVertexAngle(vertex(0, 1), -30);
    PAUSED = STARTED = FREECAM = false;
}

void generateStage() {
    const int level2[6][8] = {
            {1, 1, 1, 1, 1, 1, 1, 1},
            {1, 0, 0, 0, 0, 0, 0, 1},
            {1, 0, 0, 0, 0, 0, 0, 1},
            {1, 0, 0, 0, 0, 0, 0, 1},
            {1, 0, 0, 0, 0, 0, 0, 1},
            {1, 1, 1, 1, 1, 1, 1, 1},
    };

    const int level3[6][9] = {
            {0, 0, 1, 1, 0, 1, 1, 0, 0},
            {0, 1, 1, 1, 1, 1, 1, 1, 0},
            {0, 1, 1, 1, 1, 1, 1, 1, 0},
            {0, 0, 1, 1, 1, 1, 1, 0, 0},
            {0, 0, 0, 1, 1, 1, 0, 0, 0},
            {0, 0, 0, 0, 1, 0, 0, 0, 0},
    };
    stage = brickGrid(8, 6);
    switch (STAGE) {
        case 1: // CHESS STAGE
            for (int i = 0; i < stage.getWidth(); ++i)
                for (int j = 0; j < stage.getHeight(); ++j)
                    if ((j + i) % 2 == 0)
                        continue;
                    else
                        stage.setBrick(i, j, brick(
                                gameRandom(),
                                gameRandom(),
                                gameRandom()
                        ));
            break;
        case 2:
            for (int i = 0; i < stage.getWidth(); ++i)
                for (int j = 0; j < stage.getHeight(); ++j)
                    stage.setBrick(i, j,
                                   level2[j][i] ?
                                   brick(0.3, 0.3, 0.3, 2) :
                                   brick(0, 0.9, 0));
            break;

        case 3:
            stage = brickGrid(9, 6);
            for (int i = 0; i < stage.getWidth(); ++i)
                for (int j = 0; j < stage.getHeight(); ++j)
                    if (level3[j][i])
                        stage.setBrick(i, j, brick(1, 0, 0, 2));
            break;
        default: // RANDOM STAGE
            for (int i = 0; i < stage.getWidth(); ++i)
                for (int j = 0; j < stage.getHeight(); ++j)
                    if (gameRandom() >= 0.4)
                        stage.setBrick(i, j, brick(
                                gameRandom(),
                                gameRandom(),
                                gameRandom()
                        ));
    }
}


// reseta o jogo
void resetState() {
    LIVES = 5;
    STAGE = 1;
    barPosition = 0;
    resetBall();
    // cria fase padrão
    generateStage();

}


void fail() {
    LIVES--;
    if (LIVES > 0)
        resetBall();
    else
        resetState();
}

// Colisão com rebatedor
void handleBarColision(float limit) {
    if (ballPosition.y != limit) return;

    if (ballPosition.x >= barPosition - BAR_WIDTH / 2 && ballPosition.x < barPosition + BAR_WIDTH / 2) {
        // colision happened, check angle

        ballDirection.y *= -1;
        ballDirection.x *= -1;
        float cSlice;
        float barBegin = barPosition - BAR_WIDTH / 2;
        const float barSlice = BAR_WIDTH / BAR_SLICES;

        for (cSlice = 0; cSlice < BAR_SLICES; ++cSlice)
            if (barBegin + barSlice * cSlice <= ballPosition.x &&
                barBegin + barSlice * (cSlice + 1) >= ballPosition.x)
                break;

        vertex colisionAngle = rotateVertexAngle(vertex(-1, 0), -20);
        colisionAngle = rotateVertexAngle(colisionAngle, -((180.0 - 40) / BAR_SLICES) * cSlice);

        /// NOTA: na versão inicial, era realizado o cálculo pela lei da reflexão,
        /// mas isso fazia com que a bola fosse para fora dependendo do angulo,
        /// logo do ponto de vista da jogabilidade, o novo calculo e melhor
        // float difference = vertexAngle(ballDirection) - vertexAngle(colisionAngle);
        // ballDirection = rotateVertexAngle(ballDirection, -difference * 2);

        ballDirection = colisionAngle;
    } else
        /// colision fail
        fail();

}

// Colisão  com paredes
void handleBoardColision(float xLimit, float yLimit) {
    const float notchP = BOARD_NOTCH_WIDTH / 2,
            notchSlices = 16,
            notchSliceSize = BOARD_NOTCH_WIDTH / notchSlices;

    if (fabs(ballPosition.x) == xLimit) {
        ballDirection.x *= -1;
        /// HIT notch
        if (-notchP <= ballPosition.y && ballPosition.y <= notchP) {
            ballDirection.y *= -1;
            float cSlice = 0;
            vertex cAngle(0, 1);
            for (cSlice = 0; cSlice < notchSlices; ++cSlice) {
                if (cAngle.y * notchP <= ballPosition.y) {
                    break;
                }
                cAngle = rotateVertexAngle(cAngle, 180.0 / notchSlices * signal(ballPosition.x));
            }
            cAngle = rotateVertexAngle(cAngle, -(180.0 / (notchSlices)) / 2 * signal((ballPosition.x)));
            ballDirection = rotateVertexAngle(ballDirection, vertexAngle(cAngle) - vertexAngle(ballDirection));
        }
    }

    if (ballPosition.y == yLimit)
        ballDirection.y *= -1;


}