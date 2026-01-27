#pragma once
#include "render.h"
#include "raylib.h"

void draw_entire_board(Board& board, Animation& animation, int score) {
    draw_board(board.getGridOrigin().x, board.getGridOrigin().y);
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
                board.getGridOrigin().x + (x * TILE_SIZE),
                board.getGridOrigin().y + (y * TILE_SIZE),
                TILE_SIZE,
                TILE_SIZE
            };

            DrawRectangleLinesEx(rect, 1, DARKGRAY);

            if (board.getTile(x, y) != ' ') {
                Vector2 position = { rect.x + 12, rect.y + 8 - board.getFallOffset(x, y) };
                DrawTextEx(
                    GetFontDefault(),
                    TextFormat("%c", board.getTile(x, y)),
                    position,
                    20, 1,
                    board.isMatched(x, y) ? GREEN : YELLOW
                );
            }
        }
    }
}

void draw_selected(Board& board) {
    if (board.getSelectedTile().x >= 0) {
        Rectangle selectedRect = {
            board.getGridOrigin().x + (board.getSelectedTile().x * TILE_SIZE),
            board.getGridOrigin().y + (board.getSelectedTile().y * TILE_SIZE),
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