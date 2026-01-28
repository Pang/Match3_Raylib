#pragma once
#include "render.h"
#include "raylib.h"

void Render::drawEntireBoard(Board& board, Animation& animation, int score, Rectangle resetButton) {
    drawBoard(board.getGridOrigin().x, board.getGridOrigin().y);
    drawTiles(board);
    drawSelected(board);

    DrawText(TextFormat("Score: %d", score), 20, 20, 24, YELLOW);
	drawResetButton(resetButton);
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

            int tile = board.getTile(x, y);
            if (static_cast<TileType>(tile) != TILE_NONE) {
                Rectangle src = { 0, 0, (float)board.tileTextures[tile].width, (float)board.tileTextures[tile].height };
                Rectangle dst = { rect.x + 2, rect.y + 2 - board.getFallOffset(x, y), TILE_SIZE - 4, TILE_SIZE - 4 };
                DrawTexturePro(board.tileTextures[tile], src, dst, { 0, 0 }, 0.0f, WHITE);
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
            Color c = Fade(WHITE, animation.score_popups[i].alpha);
            DrawText(
                TextFormat("+%d", animation.score_popups[i].amount),
                animation.score_popups[i].position.x,
                animation.score_popups[i].position.y,
                20, c);
        }
    }
}

void Render::drawResetButton(Rectangle button)
{
    DrawRectangleRec(button, LIGHTGRAY);
    DrawRectangleLinesEx(button, 2, DARKGRAY);
    DrawText("RESET",
        button.x + 30,
        button.y + 10,
        20,
        BLACK);
}