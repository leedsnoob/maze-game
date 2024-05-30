#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define WALL '#'
#define PATH ' '
#define START 'S'
#define END 'E'

typedef struct {
    int x, y;
} Cell;

void initializeMaze(char **maze, int width, int height);
void generateMaze(char **maze, int width, int height);
void carvePassagesFrom(int x, int y, char **maze, int width, int height);
void saveMazeToFile(char **maze, int width, int height, const char *filename);
int isSolvable(char **maze, int width, int height, int startX, int startY);

int main(int argc, char *argv[]) {
    if (argc != 4) {
        printf("Usage: %s <filename> <width> <height>\n", argv[0]);
        return 1;
    }

    const char *filename = argv[1];
    int width = atoi(argv[2]);
    int height = atoi(argv[3]);

    if (width < 5 || width > 100 || height < 5 || height > 100) {
        printf("Maze dimensions must be between 5 and 100.\n");
        return 1;
    }

    char **maze = malloc(height * sizeof(char *));
    for (int i = 0; i < height; i++) {
        maze[i] = malloc(width * sizeof(char));
    }

    do {
        initializeMaze(maze, width, height);
        generateMaze(maze, width, height);
    } while (!isSolvable(maze, width, height, 1, 1)); // assuming start is at (1, 1)

    saveMazeToFile(maze, width, height, filename);

    for (int i = 0; i < height; i++) {
        free(maze[i]);
    }
    free(maze);

    return 0;
}

void initializeMaze(char **maze, int width, int height) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            maze[i][j] = WALL;
        }
    }
}

void generateMaze(char **maze, int width, int height) {
    srand(time(NULL));

    int startX = 1;
    int startY = 1;
    int endX = width - 2;
    int endY = height - 2;

    maze[startY][startX] = START;

    carvePassagesFrom(startX, startY, maze, width, height);

    // Ensure there is an exit point
    for (int i = height - 2; i >= 0; i--) {
        for (int j = width - 2; j >= 0; j--) {
            if (maze[i][j] == PATH) {
                maze[i][j] = END;
                return;
            }
        }
    }
}

void carvePassagesFrom(int x, int y, char **maze, int width, int height) {
    int directions[] = { 0, 1, 2, 3 };
    for (int i = 0; i < 4; i++) {
        int r = rand() % 4;
        int temp = directions[r];
        directions[r] = directions[i];
        directions[i] = temp;
    }

    for (int i = 0; i < 4; i++) {
        int dx = 0, dy = 0;
        switch (directions[i]) {
            case 0: dx = 1; break;
            case 1: dy = 1; break;
            case 2: dx = -1; break;
            case 3: dy = -1; break;
        }
        int nx = x + dx * 2;
        int ny = y + dy * 2;

        if (nx > 0 && nx < width && ny > 0 && ny < height && maze[ny][nx] == WALL) {
            maze[ny][nx] = PATH;
            maze[y + dy][x + dx] = PATH;
            carvePassagesFrom(nx, ny, maze, width, height);
        }
    }
}

int isSolvable(char **maze, int width, int height, int startX, int startY) {
    int **visited = malloc(height * sizeof(int *));
    for (int i = 0; i < height; i++) {
        visited[i] = malloc(width * sizeof(int));
        for (int j = 0; j < width; j++) {
            visited[i][j] = 0;
        }
    }

    int stackSize = width * height;
    Cell *stack = malloc(stackSize * sizeof(Cell));
    int stackTop = 0;

    stack[stackTop++] = (Cell){startX, startY};
    visited[startY][startX] = 1;

    while (stackTop > 0) {
        Cell cell = stack[--stackTop];
        int x = cell.x;
        int y = cell.y;

        if (maze[y][x] == END) {
            for (int i = 0; i < height; i++) {
                free(visited[i]);
            }
            free(visited);
            free(stack);
            return 1;
        }

        Cell neighbors[] = {
            {x + 1, y}, {x - 1, y}, {x, y + 1}, {x, y - 1}
        };

        for (int i = 0; i < 4; i++) {
            int nx = neighbors[i].x;
            int ny = neighbors[i].y;
            if (nx >= 0 && nx < width && ny >= 0 && ny < height &&
                maze[ny][nx] != WALL && !visited[ny][nx]) {
                stack[stackTop++] = (Cell){nx, ny};
                visited[ny][nx] = 1;
            }
        }
    }

    for (int i = 0; i < height; i++) {
        free(visited[i]);
    }
    free(visited);
    free(stack);
    return 0;
}

void saveMazeToFile(char **maze, int width, int height, const char *filename) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        printf("Error: could not open file %s for writing.\n", filename);
        exit(2);
    }

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            fputc(maze[i][j], file);
        }
        fputc('\n', file);
    }

    fclose(file);
}
