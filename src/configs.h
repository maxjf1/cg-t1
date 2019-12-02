/// CONSTS / CONFIGS
const float FPS = 60;
const float ANGLE_3D = 30;
const float BOARD_WIDTH = 6;
const float BOARD_HEIGHT = 4;
const float BOARD_DEPTH = 0.2;
const float BOARD_NOTCH_WIDTH = BOARD_HEIGHT / 2;
const float BOARD_NOTCH_HEIGHT = BOARD_DEPTH / 2;

const float STAGE_HEIGHT = BOARD_HEIGHT / 2;
const float STAGE_OFFSET = BOARD_HEIGHT / 8;
const float BAR_WIDTH = BOARD_WIDTH / 3;
const float BAR_HEIGHT = BOARD_DEPTH * 0.8;
const float BAR_SLICES = 20;
int STAGE = 1;
float BALL_RADIUS = 0.1;
float VELOCITY = 0.7 / (BALL_RADIUS * 100);

// TEXTURES IDS
const int
        TEX_SKYBOX = 0,
        TEX_BARSIDE = 1,
        TEX_MENU = 2,
        TEX_LOGO = 3,
        TEX_STAGE = 4,
        TEX_BARFRONT = 5;