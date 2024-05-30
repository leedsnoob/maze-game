#include "maze.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void initialize_maze(char **maze, int width, int height) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            maze[i][j] = WALL;
        }
    }
    maze[1][1] = START;
    maze[height-2][width-2] = EXIT;
}

void generate_maze(char **maze, int width, int height, int difficulty) {
    srand(time(NULL));

    typedef struct {
        int x, y;
    } Position;

    Position *walls = malloc((width * height) * sizeof(Position));
    int wallCount = 0;

    walls[wallCount++] = (Position) {1, 2};
    walls[wallCount++] = (Position) {2, 1};

    maze[1][1] = PATH;

    while (wallCount > 0) {
        int index = rand() % wallCount;
        Position wall = walls[index];
        walls[index] = walls[--wallCount];

        int dx[] = {0, 0, -1, 1};
        int dy[] = {-1, 1, 0, 0};

        int nx = wall.x + dx[rand() % 4];
        int ny = wall.y + dy[rand() % 4];

        if (nx > 0 && nx < width - 1 && ny > 0 && ny < height - 1 && maze[ny][nx] == WALL) {
            maze[wall.y][wall.x] = PATH;
            maze[ny][nx] = PATH;

            walls[wallCount++] = (Position) {nx, ny};

            for (int i = 0; i < 4; i++) {
                int ex = nx + dx[i];
                int ey = ny + dy[i];
                if (ex > 0 && ex < width - 1 && ey > 0 && ey < height - 1 && maze[ey][ex] == WALL) {
                    walls[wallCount++] = (Position) {ex, ey};
                }
            }
        }
    }

    free(walls);
}

void write_maze_to_file(char **maze, int width, int height, const char *filename) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        fprintf(stderr, "Error opening file: %s\n", filename);
        exit(1);
    }

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            fputc(maze[i][j], file);
        }
        fputc('\n', file);
    }

    fclose(file);
}
