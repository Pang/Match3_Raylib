#pragma once
#include "render.h"
#include "raylib.h"

void draw_entire_board(Board& board, Animation& animation, int score) {
    draw_board(board.grid_origin.x, board.grid_origin.y);
    draw_tiles(board);
    draw_selected(board);
    DrawText(TextFormat("Score: %d", score), 20, 20, 24, YELLOW);
    draw_score_popups(animation);
}

void draw_board(int x1, int y1) {
    DrawRectangle(
        x1,
        y1,
        BOARD_SIZE * TILE_SIZE,
        BOARD_SIZE * TILE_SIZE,
        Fade(DARKGRAY, 0.60f)
    );
}

void draw_tiles(Board& board) {
    for (int y = 0; y < BOARD_SIZE; y++) {
        for (int x = 0; x < BOARD_SIZE; x++) {
            Rectangle rect = {
                board.grid_origin.x + (x * TILE_SIZE),
                board.grid_origin.y + (y * TILE_SIZE),
                TILE_SIZE,
                TILE_SIZE
            };

            DrawRectangleLinesEx(rect, 1, DARKGRAY);

            if (board.tiles[y][x] != ' ') {
                Vector2 position = { rect.x + 12, rect.y + 8 - board.fall_offset[y][x] };
                DrawTextEx(
                    GetFontDefault(),
                    TextFormat("%c", board.tiles[y][x]),
                    position,
                    20, 1,
                    board.matched[y][x] ? GREEN : YELLOW
                );
            }
        }
    }
}

void draw_selected(Board& board) {
    if (board.selected_tile.x >= 0) {
        Rectangle selectedRect = {
            board.grid_origin.x + (board.selected_tile.x * TILE_SIZE),
            board.grid_origin.y + (board.selected_tile.y * TILE_SIZE),
            TILE_SIZE,
            TILE_SIZE
        };
        DrawRectangleLinesEx(selectedRect, 2, YELLOW);
    }
}

void draw_score_popups(Animation& animation) {
    for (int i = 0; i < MAX_SCORE_POPUPS; i++) {
        if (animation.score_popups[i].active) {
            Color c = Fade(YELLOW, animation.score_popups[i].alpha);
            DrawText(
                TextFormat("+%d", animation.score_popups[i].amount),
                animation.score_popups[i].position.x,
                animation.score_popups[i].position.y,
                20, c);
        }
    }
}