#pragma once
#include "board.h"
#include "config.h"
#include "raylib.h"
#include <stdlib.h>
#include <vector>

const char tile_chars[TILE_TYPES] = { '#', '@', '$', '%', '&' };

std::vector<float> init_board(Board& board) {
    for (int y = 0; y < BOARD_SIZE; y++) {
        for (int x = 0; x < BOARD_SIZE; x++) {
            board.tiles[y][x] = random_tile();
        }
    }

    int grid_width = BOARD_SIZE * TILE_SIZE;
    int grid_height = BOARD_SIZE * TILE_SIZE;

    return { (GetScreenWidth() - grid_width) / 2.0f, (GetScreenHeight() - grid_height) / 2.0f };
}

char random_tile() {
    return tile_chars[rand() % TILE_TYPES];
}

void swap_tiles(Board& board, int x1, int y1, int x2, int y2) {
    char temp = board.tiles[y1][x1];
    board.tiles[y1][x1] = board.tiles[y2][x2];
    board.tiles[y2][x2] = temp;
}

bool are_tiles_adjacent(int x1, int y1, int x2, int y2) {
    return (abs((int)x1 - (int)x2) + abs((int)y1 - (int)y2)) == 1;
}