#ifndef MAZE_G
#define MAZE_G

#define WALL '#'
#define PATH ' '
#define START 'S'
#define END 'E'

void prim_maze(int width, int height, const char *filename);
char** initialize_maze(int width, int height);
void print_maze(char **maze, int width, int height);
void save_maze(char **maze, int width, int height, const char *filename);

#endif //
