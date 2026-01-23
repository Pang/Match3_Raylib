#pragma once
#include "config.h"
#include "../types/types.h"
#include <vector>

typedef struct {
    Vec2Int grid_origin;
    Vec2Int selected_tile;
    char tiles[BOARD_SIZE][BOARD_SIZE];
    bool matched[BOARD_SIZE][BOARD_SIZE];
    float fall_offset[BOARD_SIZE][BOARD_SIZE];
} Board;

typedef struct {
    bool foundMatches;
    int updatedScore;
    std::vector<Vec2Int> matchPositions;
} FoundMatchesResponse;

void init_board(Board& board);
char random_tile();
void swap_tiles(Board& board, int x1, int y1, int x2, int y2);
bool are_tiles_adjacent(int x1, int y1, int x2, int y2);
FoundMatchesResponse find_matches(Board& board, int score);