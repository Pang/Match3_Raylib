#pragma once

#include "raylib.h"
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>

#include "types/types.h"
#include "board/config.h"
#include "board/board.h"
#include "animation/animation.h"
#include "render/render.h"

Board board{};
Animation animation{};

FoundMatchesResponse found_matches_response = {};
int score = 0;

void add_score_popup(Animation& animation, int x, int y, int amount, Vec2Int grid_origin) {
    animation.score_animating = true;
    animation.score_scale = 2.0f;
    animation.score_scale_velocity = -2.5f;

    for (int i = 0; i < MAX_SCORE_POPUPS; i++) {
        if (!animation.score_popups[i].active) {
            Vec2Int popupPos = {
                grid_origin.x + x * TILE_SIZE + TILE_SIZE / 2,
                grid_origin.y + y * TILE_SIZE + TILE_SIZE / 2
            };
            animation.score_popups[i].position = popupPos;
            animation.score_popups[i].amount = amount;
            animation.score_popups[i].lifetime = 1.0f;
            animation.score_popups[i].alpha = 1.0f;
            animation.score_popups[i].active = true;
            break;
        }
    }
}

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
                } else {
                    if (are_tiles_adjacent(board.selected_tile.x, board.selected_tile.y, x, y)) {
                        swap_tiles(board, board.selected_tile.x, board.selected_tile.y, x, y);
                        
                        found_matches_response = find_matches(board, score);
                        score = found_matches_response.updatedScore;

                        if (found_matches_response.foundMatches) {
                            match_found();
                        } else {
                            swap_tiles(board, board.selected_tile.x, board.selected_tile.y, x, y);
                        }
                    }
                    board.selected_tile = { -1, -1 };
                }
            }
        }

        if (board.tile_state == STATE_ANIMATING) {
            bool still_animating = false;

            for (int y = 0; y < BOARD_SIZE; y++) {
                for (int x = 0; x < BOARD_SIZE; x++) {
                    if (board.fall_offset[y][x] > 0) {
                        board.fall_offset[y][x] -= animation.fall_speed;
                        if (board.fall_offset[y][x] < 0) {
                            board.fall_offset[y][x] = 0;
                        }
                        else {
                            still_animating = true;
                        }
                    }
                }
            }

            if (!still_animating) {
                board.tile_state = STATE_MATCH_DELAY;
                animation.match_delay_timer = animation.match_delay_duration;
            }
        }

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

        // update our score popups array
        for (int i = 0; i < MAX_SCORE_POPUPS; i++) {
            if (animation.score_popups[i].active) {
                animation.score_popups[i].lifetime -= GetFrameTime();
                animation.score_popups[i].position.y -= 30 * GetFrameTime();
                animation.score_popups[i].alpha = animation.score_popups[i].lifetime;

                if (animation.score_popups[i].lifetime <= 0.0f) {
                    animation.score_popups[i].active = false;
                }
            }
        }

        // update the score animation
        if (animation.score_animating) {
            animation.score_scale += animation.score_scale_velocity * GetFrameTime();
            if (animation.score_scale <= 1.0f) {
                animation.score_scale = 1.0f;
                animation.score_animating = false;
            }
        }

        BeginDrawing();
        ClearBackground(BLACK);

        draw_board(board.grid_origin.x, board.grid_origin.y);

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
                    Vector2 position = { rect.x + 12, rect.y + 8 - board.fall_offset[y][x]};
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

        if (board.selected_tile.x >= 0) {
            Rectangle selectedRect = {
                board.grid_origin.x + (board.selected_tile.x * TILE_SIZE),
                board.grid_origin.y + (board.selected_tile.y * TILE_SIZE),
                TILE_SIZE,
                TILE_SIZE
            };
            DrawRectangleLinesEx(selectedRect, 2, YELLOW);
        }
        DrawText(TextFormat("Score: %d", score), 20, 20, 24, YELLOW);

        // draw score popups
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

        EndDrawing();
    }

    CloseWindow(); 
    return 0;
}