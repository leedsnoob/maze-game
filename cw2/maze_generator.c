#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "maze_generator.h"
// 部分思路来自 https://blog.csdn.net/m0_75008697/article/details/132566835
// 算法思路来自 https://blog.csdn.net/weixin_45218342/article/details/134369330
//哈曼吨距离 https://blog.csdn.net/weixin_56154577/article/details/136388934

int main(int argc, char *argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <filename> <width> <height>\n", argv[0]);
        return 1;
    }

    const char *filename = argv[1];
    int width = atoi(argv[2]);
    int height = atoi(argv[3]);

    if (width < 5 || width > 100 || height < 5 || height > 100) {
        fprintf(stderr, "Width and height must be between 5 and 100.\n");
        return 1;
    }

    srand(time(NULL));
    prim_maze(width, height, filename);

    return 0;
}