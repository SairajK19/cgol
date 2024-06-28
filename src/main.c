#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#undef ENABLE_GRID
#undef ENABLE_CELL_COORDINATE
#define max(a, b)                                                              \
  ({                                                                           \
    __typeof__(a) _a = (a);                                                    \
    __typeof__(b) _b = (b);                                                    \
    _a > _b ? _a : _b;                                                         \
  })

int WIDTH = 1500;
int HEIGHT = 800;
int GRID_SIZE = 50;
int CELL_SIZE = 40;
int MAX_CELLS = 1000;

struct Cell {
  int x;
  int y;
  int alive;
} typedef Cell;

int *get_random_coordinate(int x_lower, int x_upper, int y_lower, int y_upper);
int get_neighbor_count(int **GameMap, Cell cell);

int **init_simulation(int **GameMap, int *n_cols, int *n_rows, Cell *cells,
                      int *population) {
  int *coord, generation_size = 5;
  printf("Rows: %d, Cols: %d\n", *n_cols, *n_cols);
  GameMap = (int **)malloc(*n_cols * sizeof(int *));

  for (int i = 0; i <= *n_cols; i++) {
    GameMap[i] = (int *)malloc(*n_rows * sizeof(int));
    memset(GameMap[i], 0, *n_rows * sizeof(int));
  }

  for (int j = 0; j < 8; j++) {
    coord = get_random_coordinate(0, *n_rows - 1, 0, *n_cols - 1);
    printf("Initial Coords %d-%d\n", coord[0], coord[1]);
    for (int i = 0; i < generation_size; i++) {
      Cell cell;

      if (i != 0) {
        do {
          printf("Getting cell at %d\n", j * generation_size);
          cell = cells[j * generation_size];
          coord = get_random_coordinate(cell.x - 1, cell.x + 1, cell.y - 1,
                                        cell.y + 1);
        } while (GameMap[coord[0]][coord[1]] == 1 &&
                 get_neighbor_count(GameMap, cells[j * generation_size]) <
                     generation_size - 1);
      }

      cell.x = coord[0];
      cell.y = coord[1];
      cell.alive = 1;

      printf("Population: %d. x:%d y:%d\n", *population, cell.x, cell.y);
      cells[*population] = cell;
      printf("Adding to map: %d. x:%d y:%d. Added cell at: %d\n", *population,
             cell.x, cell.y, *population);
      GameMap[cell.y][cell.x] = 1;
      (*population)++;
    }
  }

  for (int i = 0; i < *n_rows; i++) {
    for (int j = 0; j < *n_cols; j++) {
      printf("GameMap[%d][%d] = %d n_rows=%d n_cols=%d\n", j, i, GameMap[j][i],
             *n_rows, *n_cols);
      if (GameMap[j][i] != 1) {
        Cell cell;
        cell.x = i * GRID_SIZE;
        cell.y = j * GRID_SIZE;
        cell.alive = 0;

        cells[*population] = cell;
        (*population)++;
      }
    }
  }

  return GameMap;
}

int get_neighbor_count(int **GameMap, Cell cell) {
  int count = -1;
  printf("Getting neigh at \n");
  for (int i = cell.x - 1; i <= cell.x + 1; i++) {
    for (int j = cell.y - 1; j <= cell.y + 1; j++) {
      if (i >= 0 && i < HEIGHT / GRID_SIZE && j >= 0 && j < WIDTH / GRID_SIZE) {
        if (GameMap[j][i] == 1)
          count++;
      }
    }
  }

  return count++;
}

int *get_random_coordinate(int x_lower, int x_upper, int y_lower, int y_upper) {
  int *coordinate = malloc(2 * sizeof(int));
  printf("Getting random coord\n");

  coordinate[0] = max(0, rand() % (x_upper - x_lower + 1) + x_lower);
  coordinate[1] = max(0, rand() % (y_upper - y_lower + 1) + y_lower);

  printf("Got random coord\n");
  return coordinate;
}

int **simulate_universe(Cell *cells, int *population, int **GameMap) {
  int neighbor = -1;
  for (int i = 0; i < *population; i++) {
    neighbor = get_neighbor_count(GameMap, cells[i]);

    if (neighbor == 3) {
      GameMap[cells[i].y][cells[i].x] = 1;
      cells[i].alive = 1;

      printf("Reseructed x:%d, y:%d\n", cells[i].x, cells[i].y);
    } else if ((neighbor < 2 || neighbor > 3) && neighbor != -1) {
      GameMap[cells[i].y][cells[i].x] = 0;
      cells[i].alive = 0;

      printf("Killed x:%d, y:%d\n", cells[i].x, cells[i].y);
    } else if (neighbor == 2) {
      printf("Surviving x:%d, y:%d\n", cells[i].x, cells[i].y);
    }
    neighbor = -1;
  }

  return GameMap;
}

int main() {
  int n_cols = WIDTH / GRID_SIZE, n_rows = HEIGHT / GRID_SIZE, population = 0;
  int **GameMap;
  Cell cells[MAX_CELLS];
  srand(time(NULL));
  GameMap = init_simulation(GameMap, &n_cols, &n_rows, cells, &population);
  printf("Population: %d\n", population);

  InitWindow(WIDTH, HEIGHT, "Conway's Game of Life");

  while (!WindowShouldClose()) {
    PollInputEvents();

    BeginDrawing();
    WaitTime(0.05);

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
        if (GameMap[j][i] == 1) {
          DrawRectangle(j * GRID_SIZE + (GRID_SIZE - CELL_SIZE) / 2,
                        i * GRID_SIZE + (GRID_SIZE - CELL_SIZE) / 2, CELL_SIZE,
                        CELL_SIZE, WHITE);
        } else if (GameMap[j][i] == 0) {
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

    GameMap = simulate_universe(cells, &population, GameMap);
    EndDrawing();
  }
  return 0;
}
