#include <stdlib.h>
#include "list.h"
//新的墙壁
void add_to_list(Node **head, int x, int y) {
    Node *new_node = (Node *)malloc(sizeof(Node));
    new_node->cell.x = x;
    new_node->cell.y = y;
    new_node->next = *head;
    *head = new_node;
}
//移除已经挖开的
void remove_from_list(Node **head, int x, int y) {
    Node *temp = *head, *prev = NULL;
    while (temp != NULL && (temp->cell.x != x || temp->cell.y != y)) {
        prev = temp;
        temp = temp->next;
    }
    if (temp != NULL) {
        if (prev == NULL) {
            *head = temp->next;
        } else {
            prev->next = temp->next;
        }
        free(temp);
    }
}
//还要多少
int list_length(Node *head) {
    int length = 0;
    while (head != NULL) {
        length++;
        head = head->next;
    }
    return length;
}
//随机选择一个待处理的墙壁单元格
Cell get_random_cell(Node *head) {
    int length = list_length(head);
    int index = rand() % length;
    for (int i = 0; i < index; i++) {
        head = head->next;
    }
    return head->cell;
}
//检查其单元格
Cell* get_neighbors(int x, int y, int width, int height, int *count) {
    Cell *neighbors = (Cell *)malloc(4 * sizeof(Cell));
    *count = 0;

    if (x > 1) {
        neighbors[*count].x = x - 2;
        neighbors[*count].y = y;
        (*count)++;
    }
    if (x < width - 2) {
        neighbors[*count].x = x + 2;
        neighbors[*count].y = y;
        (*count)++;
    }
    if (y > 1) {
        neighbors[*count].x = x;
        neighbors[*count].y = y - 2;
        (*count)++;
    }
    if (y < height - 2) {
        neighbors[*count].x = x;
        neighbors[*count].y = y + 2;
        (*count)++;
    }

    return neighbors;
}
