#pragma once

#include "raylib.h"
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>

#include "types/types.h"
#include "board/config.h"
#include "game/game.h"
#include "board/board.h"
#include "animation/animation.h"
#include "render/render.h"

Board board{};
Animation animation{};

FoundMatchesResponse found_matches_response = {};
int score = 0;

void match_found() {
    for (int i = 0; i < found_matches_response.matchPositions.size(); i++) {
        Vec2Int pos = found_matches_response.matchPositions[i];
        add_score_popup(animation, pos.x, pos.y, 10, board.grid_origin);
    }
    resolve_matches(board);
}

int main(void) {
    InitWindow(screenWidth, screenHeight, "Match 3 Game");
    SetTargetFPS(60);  
    srand(time(NULL));

    init_board(board);
    init_animation(animation);

	found_matches_response = find_matches(board, score);
	score = found_matches_response.updatedScore;

    if (found_matches_response.foundMatches) {
        match_found();
    }
    else {
        board.tile_state = STATE_IDLE;
    }

    Vector2 mouse = { 0, 0 };

    while (!WindowShouldClose()) 
    {
        mouse = GetMousePosition();
        if (board.tile_state == STATE_IDLE && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            int x = (mouse.x - board.grid_origin.x) / TILE_SIZE;
            int y = (mouse.y - board.grid_origin.y) / TILE_SIZE;

            if (x >= 0 && x < BOARD_SIZE && y >= 0 && y < BOARD_SIZE) {
                if (board.selected_tile.x < 0) {
                    board.selected_tile = { x, y };
                }
                else {
                    if (are_tiles_adjacent(board.selected_tile.x, board.selected_tile.y, x, y)) {
                        swap_tiles(board, board.selected_tile.x, board.selected_tile.y, x, y);

                        found_matches_response = find_matches(board, score);
                        score = found_matches_response.updatedScore;

                        if (found_matches_response.foundMatches) {
                            match_found();
                        }
                        else {
                            swap_tiles(board, board.selected_tile.x, board.selected_tile.y, x, y);
                        }
                    }
                    board.selected_tile = { -1, -1 };
                }
            }
        }

        animate_falling_blocks(board, animation);

        if (board.tile_state == STATE_MATCH_DELAY) {
            animation.match_delay_timer -= GetFrameTime();
            if (animation.match_delay_timer <= 0.0f) {
                found_matches_response = find_matches(board, score);
                score = found_matches_response.updatedScore;

                if (found_matches_response.foundMatches) {
                    match_found();
                }
                else {
                    board.tile_state = STATE_IDLE;
                }
            }
        }

        update_scores(animation, GetFrameTime());

        BeginDrawing();
        ClearBackground(BLACK);
        draw_entire_board(board, animation, score);
        EndDrawing();
    }

    CloseWindow(); 
    return 0;
}