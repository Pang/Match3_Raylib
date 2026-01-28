#pragma once
#include "render.h"
#include "raylib.h"

void Render::drawEntireBoard(Board& board, Animation& animation, int score) {
    drawBoard(board.getGridOrigin().x, board.getGridOrigin().y);
    drawTiles(board);
    drawSelected(board);
    DrawText(TextFormat("Score: %d", score), 20, 20, 24, YELLOW);
    drawScorePopups(animation);
}

void Render::drawBoard(int x1, int y1) {
    DrawRectangle(
        x1,
        y1,
        BOARD_SIZE * TILE_SIZE,
        BOARD_SIZE * TILE_SIZE,
        Fade(DARKGRAY, 0.60f)
    );
}

void Render::drawTiles(Board& board) {
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

void Render::drawSelected(Board& board) {
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

void Render::drawScorePopups(Animation& animation) {
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