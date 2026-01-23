#pragma once
#include "../board/config.h"
#include "render.h"
#include "raylib.h"

void draw_board(int x1, int y1) {
    DrawRectangle(
        x1,
        y1,
        BOARD_SIZE * TILE_SIZE,
        BOARD_SIZE * TILE_SIZE,
        Fade(DARKGRAY, 0.60f)
    );
}