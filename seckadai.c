#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define INIT_DAT        0

struct Cell {
    struct Cell *front; // 前へのポインタ
    int data; // 整数値のデータ
    struct Cell *back; // 後ろへのポインタ
};

struct Cell *gen_cell(int d)
{
    struct Cell *cell = malloc(sizeof(struct Cell));
    cell->front = NULL;
    cell->data = d;
    cell->back = NULL;
    return cell;
}

struct Cell *gen_list_loop(int len)
{
    // リストの長さが1未満であったらエラー終了
    if (len < 1) {
        printf("Bad length\n");
        exit(1);
    }
    int i;
    struct Cell *list = malloc(sizeof(struct Cell) * len);
    for (i = 0; i < len; i++) {
        list[i].front = NULL;
        list[i].data = INIT_DAT;
        list[i].back = NULL;
    }
    for (i = 0; i < len - 1; i++) {
        list[i].back = &list[i + 1];
        list[i + 1].front = &list[i];
    }
    return list;
}

struct Cell *gen_list_rec(int len)
{
    // リストの長さが1未満だったらエラー終了
    if (len < 1) {
        printf("Bad length\n");
        exit(1);
    }
    struct Cell *cell = gen_cell(INIT_DAT);
    // 再帰的にリストを作成
    if (len != 1) {
        cell->back = gen_list_rec(len - 1);
        struct Cell *head = cell;
        struct Cell *next_cell = cell->back;
        while (next_cell->back != NULL) {
            cell->back = next_cell;
            cell = next_cell;
            next_cell = next_cell->back;
        }
        cell->back = next_cell;
        return head;
    } else {
        return cell;
    }
}

int get_list_len(struct Cell *list)
{
    int len = 1;
    struct Cell *cell = list;
    while (cell->back != NULL) {
        len++;
        cell = cell->back;
    }
    return len;
}

// リストlistのlocation番目の後ろにデータdatのセルを挿入する
struct Cell *insert_cell(struct Cell *list, int dat, int location)
{
    int len = get_list_len(list);
    // locationが範囲外ならfalseを返す
    if (location < 0 || len - 1 < location) {
        printf("This location is out of range.\n");
        return list;
    }

    struct Cell *ins_cell = gen_cell(dat);
    struct Cell *prev_cell_ptr = list;
    int i;
    for (i = 0; i < location; i++) {
        prev_cell_ptr = prev_cell_ptr->back;
    }
    struct Cell *next_cell_ptr = prev_cell_ptr->back;
    // 挿入するセルとその前のセルをつなぐ
    prev_cell_ptr->back = ins_cell;
    ins_cell->front = prev_cell_ptr;
    // 挿入するセルとその後のセルをつなぐ
    if (location != len - 1) {
        next_cell_ptr->front = ins_cell;
        ins_cell->back = next_cell_ptr;
    }

    printf("Insert data %3d into %3d.\n", dat, location);
    return list;
}

struct Cell *insert_cell_head(struct Cell *list, int dat)
{
    struct Cell *new_head = gen_cell(dat);
    new_head->back = list;
    list->front = new_head;
    printf("Insert data %3d into head.\n", dat);
    return new_head;
}

void display(struct Cell *list)
{
    struct Cell *cell = list;
    while (cell->back != NULL) {
        printf("%3d ", cell->data);
        cell = cell->back;
    }
    printf("%3d ", cell->data); // 最後のセルを表示
    printf("\n");
    return;
}

void free_list_rec(struct Cell *list)
{
    struct Cell *cell = list, *del_cell;
    while (cell->back != NULL) {
        cell = cell->back;
    }
    while (cell->front != NULL) {
        del_cell = cell;
        cell = cell->front;
        free(del_cell);
    }
    free(cell); // 先頭のセルを削除
    return;
}

void free_list_loop(struct Cell *list)
{
    free(list);
    return;
}

int main(void)
{
    clock_t start, end;

    start = clock();
    struct Cell *list_rec = gen_list_rec(8); // 5つのセルを持つリストを生成
    list_rec = insert_cell(list_rec, 15, 3); // 3番目の後ろに15を挿入
    list_rec = insert_cell(list_rec, -1, 0); // 0番目の後ろに-1を挿入
    list_rec = insert_cell(list_rec,  6, 9);
    list_rec = insert_cell_head(list_rec, 1); // 先頭に1を挿入
    display(list_rec);
    free_list_rec(list_rec);
    end = clock();
    printf("rec:  %f[sec]\n", (double)(end - start) / CLOCKS_PER_SEC);

    printf("\n");

    start = clock();
    struct Cell *list_loop = gen_list_loop(8);
    list_loop = insert_cell(list_loop, 15, 3);
    list_loop = insert_cell(list_loop, -1, 0);
    list_loop = insert_cell(list_loop,  6, 9);
    list_loop = insert_cell_head(list_loop, 1);
    display(list_loop);
    free_list_loop(list_loop);
    end = clock();
    printf("loop: %f[sec]\n", (double)(end - start) / CLOCKS_PER_SEC);

    return 0;
}
