#include <raylib.h>

enum LifeType {
	STRAIGHT_LINE = 0,
	CHAOS = 1,
	GLIDER = 2,
};

static int straight_line[3][3] = {{0, 1, 0}, {0, 1, 0}, {0, 1, 0}};
static int glider[3][3] = {{1, 0, 1}, {0, 1, 1}, {0, 1, 0}};
static int chaos[3][3] = {{0, 1, 0}, {1, 1, 0}, {0, 1, 1}};

static Color GRID_COLOR = {.r = 236, .g = 228, .b = 203, .a = 255};
static Color BAR_COLOR = {.r = 30, .g = 25, .b = 32, .a = 255};
static Color LIVE_CELL_COLOR = {.r = 153, .g = 168, .b = 131, .a = 255};

