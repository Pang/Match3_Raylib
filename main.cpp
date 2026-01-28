#pragma once

#include "raylib.h"
#include <stdlib.h>
#include <time.h>

#include "types/types.h"
#include "board/config.h"
#include "board/board.h"
#include "animation/animation.h"
#include "render/render.h"

Rectangle resetButton = { screenWidth - 140, screenHeight - 60, 120, 40 };
FoundMatchesResponse found_matches_response = {};
int score = 0;

void matchFound(Board& board, Animation& animation) {
    for (int i = 0; i < found_matches_response.matchPositions.size(); i++) {
        Vec2Int pos = found_matches_response.matchPositions[i];
        animation.addScorePopup(pos.x, pos.y, 10, board.getGridOrigin());
    }
    board.resolveMatches();
}

bool isResetClicked(Rectangle button, Vector2 mouse) {
    return CheckCollisionPointRec(mouse, button) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
}

void findInitialMatches(Board& board, Animation& animation) {
    found_matches_response = board.findMatches(score);
    score = found_matches_response.updatedScore;

    if (found_matches_response.foundMatches) {
        matchFound(board, animation);
    }
    else {
        board.setTileState(STATE_IDLE);
    }
}

int main(void) {
    InitWindow(screenWidth, screenHeight, "Match 3 Game");
    SetTargetFPS(60);  
    srand(time(NULL));

    Board board;
    board.init();

    Animation animation;
    Render render;

    findInitialMatches(board, animation);
    Vector2 mouse = { 0, 0 };

    while (!WindowShouldClose()) 
    {
        mouse = GetMousePosition();

        if (isResetClicked(resetButton, mouse)) {
            board.init();
            score = 0;
			findInitialMatches(board, animation);
        }

        if (board.getTileState() == STATE_IDLE && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            int x = (mouse.x - board.getGridOrigin().x) / TILE_SIZE;
            int y = (mouse.y - board.getGridOrigin().y) / TILE_SIZE;

            if (x >= 0 && x < BOARD_SIZE && y >= 0 && y < BOARD_SIZE) {
                if (board.getSelectedTile().x < 0) {
                    board.setSelectedTile({ x, y });
                }
                else {
                    if (board.areTilesAdjacent(board.getSelectedTile().x, board.getSelectedTile().y, x, y)) {
                        board.swapTiles(board.getSelectedTile().x, board.getSelectedTile().y, x, y);

                        found_matches_response = board.findMatches(score);
                        score = found_matches_response.updatedScore;

                        if (found_matches_response.foundMatches) {
                            matchFound(board, animation);
                        }
                        else {
                            board.swapTiles(board.getSelectedTile().x, board.getSelectedTile().y, x, y);
                        }
                    }
                    board.setSelectedTile({ -1, -1 });
                }
            }
        }

        animation.animateFallingBlocks(board);

        if (board.getTileState() == STATE_MATCH_DELAY) {
            animation.match_delay_timer -= GetFrameTime();
            if (animation.match_delay_timer <= 0.0f) {
                found_matches_response = board.findMatches(score);
                score = found_matches_response.updatedScore;

                if (found_matches_response.foundMatches) {
                    matchFound(board, animation);
                }
                else {
                    board.setTileState(STATE_IDLE);
                }
            }
        }

        animation.updateScorePopups(GetFrameTime());

        BeginDrawing();
        ClearBackground(BLACK);
        render.drawEntireBoard(board, animation, score, resetButton);
        EndDrawing();

    }

    CloseWindow(); 
    return 0;
}