#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "cgol.h"

#undef ENABLE_GRID
#undef ENABLE_CELL_COORDINATE
#define max(a, b)                                                              \
  ({                                                                           \
    __typeof__(a) _a = (a);                                                    \
    __typeof__(b) _b = (b);                                                    \
    _a > _b ? _a : _b;                                                         \
  })

int WIDTH = 1550;
int HEIGHT = 950;
int GRID_SIZE = 50;
int CELL_SIZE = 40;
int MAX_CELLS = 1000;

struct Cell {
  int x;
  int y;
  int alive;
} typedef Cell;

int *get_random_coordinate(int x_lower, int x_upper, int y_lower, int y_upper);
int get_neighbor_count(int **game_map, int x, int y);

int **init_simulation(int **game_map, int *n_cols, int *n_rows, int *population,
                      int type) {
  printf("Rows: %d, Cols: %d\n", *n_cols, *n_cols);
  game_map = (int **)malloc(*n_cols * sizeof(int *));

  for (int i = 0; i <= *n_cols; i++) {
    game_map[i] = (int *)malloc(*n_rows * sizeof(int));
    memset(game_map[i], 0, *n_rows * sizeof(int));
  }

  for (int i = 0; i < *n_rows; i = i + 4) {
    for (int j = 0; j < *n_cols; j = j + 4) {
      game_map[j][i] = 0;
      switch (type) {
      case 0:
        for (int x = 0; x < 3; x++) {
          for (int y = 0; y < 3; y++) {
            if (glider[x][y] == 1) {
              game_map[0 + y][0 + x] = 1;
            }
          }
        }
        break;
      default:
        for (int x = 0; x < 3; x++) {
          for (int y = 0; y < 3; y++) {
            if (straight_line[x][y] == 1) {
              game_map[j + y][i + x] = 1;
            }
          }
        }
      }
    }
  }

  return game_map;
}

// x = 1, y = 2
// x = 0-2
// y = 1-3
int get_neighbor_count(int **game_map, int x, int y) {
  int count = 0;
  for (int i = x - 1; i <= x + 1; i++) {
    for (int j = y - 1; j <= y + 1; j++) {
      if (i >= 0 && i < HEIGHT / GRID_SIZE && j >= 0 && j < WIDTH / GRID_SIZE) {
        if (game_map[j][i] == 1)
          count++;
      }
    }
  }

  if (game_map[y][x] == 1) {
    return count - 1;
  }

  return count;
}

int *get_random_coordinate(int x_lower, int x_upper, int y_lower, int y_upper) {
  int *coordinate = malloc(2 * sizeof(int));
  printf("Getting random coord\n");

  coordinate[0] = max(0, rand() % (x_upper - x_lower + 1) + x_lower);
  coordinate[1] = max(0, rand() % (y_upper - y_lower + 1) + y_lower);

  printf("Got random coord\n");
  return coordinate;
}

int **simulate_universe(int *population, int **game_map, int *n_rows,
                        int *n_cols) {
  int neighbor = 0;
  int updated_cell_count = 0;
  Cell current_gen_updates[MAX_CELLS];

  for (int x = 0; x < *n_rows; x++) {
    for (int y = 0; y < *n_cols; y++) {
      Cell cell;
      neighbor = get_neighbor_count(game_map, x, y);

      if (neighbor == 3 && game_map[y][x] == 0) {
        cell = (Cell){.x = x, .y = y, .alive = 1};
      } else if ((neighbor < 2 || neighbor > 3) && game_map[y][x] == 1) {
        cell = (Cell){.x = x, .y = y, .alive = 0};
      } else if ((neighbor == 2 || neighbor == 3) && game_map[y][x] == 1) {
        cell = (Cell){x = x, .y = y, .alive = 1};
      }

      neighbor = 0;
      current_gen_updates[updated_cell_count] = cell;
      updated_cell_count++;
    }
  }

  for (int i = 0; i < updated_cell_count; i++) {
    if (current_gen_updates[i].alive == 1) {
      game_map[current_gen_updates[i].y][current_gen_updates[i].x] = 1;
    } else if (current_gen_updates[i].alive == 0) {
      game_map[current_gen_updates[i].y][current_gen_updates[i].x] = 0;
    }
  }

  return game_map;
}

int main() {
  int n_cols = WIDTH / GRID_SIZE, n_rows = HEIGHT / GRID_SIZE, population = 0;
  int **game_map;
  Cell cells[MAX_CELLS];
  srand(time(NULL));
  game_map = init_simulation(game_map, &n_cols, &n_rows, &population, 0);
  printf("Population: %d\n", population);

  InitWindow(WIDTH, HEIGHT, "Conway's Game of Life");

  while (!WindowShouldClose()) {
    PollInputEvents();

    BeginDrawing();

#ifdef ENABLE_GRID
    for (int i = 0; i < n_rows; i++) {
      for (int j = 0; j < n_cols; j++) {
        DrawLine(j * GRID_SIZE, 0, j * GRID_SIZE, HEIGHT * GRID_SIZE, BEIGE);
      }
      DrawLine(0, i * GRID_SIZE, WIDTH * GRID_SIZE, i * GRID_SIZE, BEIGE);
    }
#endif

    for (int i = 0; i < n_rows; i++) {
      for (int j = 0; j < n_cols; j++) {
        if (game_map[j][i] == 1) {
          DrawRectangle(j * GRID_SIZE + (GRID_SIZE - CELL_SIZE) / 2,
                        i * GRID_SIZE + (GRID_SIZE - CELL_SIZE) / 2, CELL_SIZE,
                        CELL_SIZE, WHITE);
        } else if (game_map[j][i] == 0) {
          DrawRectangle(j * GRID_SIZE + (GRID_SIZE - CELL_SIZE) / 2,
                        i * GRID_SIZE + (GRID_SIZE - CELL_SIZE) / 2, CELL_SIZE,
                        CELL_SIZE, RED);
        }

#ifdef ENABLE_CELL_COORDINATE
        char snum[10];
        sprintf(snum, "%d", i);
        DrawText(snum, j * GRID_SIZE, i * GRID_SIZE, 10, GREEN);
        DrawText("-", j * GRID_SIZE + 12, i * GRID_SIZE, 10, GREEN);
        sprintf(snum, "%d", j);
        DrawText(snum, j * GRID_SIZE + 22, i * GRID_SIZE, 10, GREEN);
#endif
      }
    }

    game_map = simulate_universe(&population, game_map, &n_rows, &n_cols);
    WaitTime(0.5);
    EndDrawing();
  }
  return 0;
}
