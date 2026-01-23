#pragma once
#include "config.h"
#include "vector"

typedef struct {
    char tiles[BOARD_SIZE][BOARD_SIZE];
    bool matched[BOARD_SIZE][BOARD_SIZE];
    float fall_offset[BOARD_SIZE][BOARD_SIZE];
} Board;

std::vector<float> init_board(Board& board);
char random_tile();
void swap_tiles(Board& board, int x1, int y1, int x2, int y2);
bool are_tiles_adjacent(int x1, int y1, int x2, int y2);
