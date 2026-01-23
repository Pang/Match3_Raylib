#pragma once

#include "raylib.h"
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>

#include "board/config.h"
#include "board/board.h"

Board board{};

int score = 0;
Vector2 grid_origin;
Vector2 selected_tile = { -1, -1};

float fall_speed = 5.0f;
float match_delay_timer = 0.0f;
const float MATCH_DELAY_DURATION = 0.5f;

float score_scale = 1.0f;
float score_scale_velocity = 0.0f;
bool score_animating = false;

typedef enum {
    STATE_IDLE,
    STATE_ANIMATING,
    STATE_MATCH_DELAY
} TileState;

TileState tile_state = STATE_IDLE;

typedef struct {
    Vector2 position;
    int amount;
    float lifetime;
    float alpha;
    bool active;
} ScorePopup;

ScorePopup score_popups[MAX_SCORE_POPUPS] = { 0 };

void add_score_popup(int x, int y, int amount, Vector2 grid_origin) {
    for (int i = 0; i < MAX_SCORE_POPUPS; i++) {
        if (!score_popups[i].active) {
            Vector2 popupPos = {
                grid_origin.x + x * TILE_SIZE + TILE_SIZE / 2,
                grid_origin.y + y * TILE_SIZE + TILE_SIZE / 2
			};
            score_popups[i].position = popupPos;
            score_popups[i].amount = amount;
            score_popups[i].lifetime = 1.0f;
            score_popups[i].alpha = 1.0f;
            score_popups[i].active = true;
            break;
        }
    }
}

bool find_matches() {
    bool found = false;
    for (int y = 0; y < BOARD_SIZE; y++) {
        for (int x = 0; x < BOARD_SIZE; x++) {
            board.matched[y][x] = false;
        }
    }

    for (int y = 0; y < BOARD_SIZE; y++) {
        for (int x = 0; x < BOARD_SIZE - 2; x++) {
            char t = board.tiles[y][x];
            if (t == board.tiles[y][x + 1] &&
                t == board.tiles[y][x + 2]) {
                board.matched[y][x] = board.matched[y][x + 1] = board.matched[y][x + 2] = true;
                // update score
                score += 10;
                found = true;

                score_animating = true;
                score_scale = 2.0f;
                score_scale_velocity = -2.5f;

                add_score_popup(x, y, 10, grid_origin);
            }
        }
    }

    for (int x = 0; x < BOARD_SIZE; x++) {
        for (int y = 0; y < BOARD_SIZE - 2; y++) {
            char t = board.tiles[y][x];
            if (t == board.tiles[y + 1][x] &&
                t == board.tiles[y + 2][x]) {
                board.matched[y][x] = board.matched[y + 1][x] = board.matched[y + 2][x] = true;
                score += 10;
                found = true;

                score_animating = true;
                score_scale = 2.0f;
                score_scale_velocity = -2.5f;

                add_score_popup(x, y, 10, grid_origin);
            }
        }
    }

    return found;
}

void resolve_matches() {
    for (int x = 0; x < BOARD_SIZE; x++) {
        int write_y = BOARD_SIZE - 1;
        for (int y = BOARD_SIZE - 1; y >= 0; y--) {
            if (!board.matched[y][x]) {
                if (y != write_y) {
                    board.tiles[write_y][x] = board.tiles[y][x];
                    board.fall_offset[write_y][x] = (write_y - y) * TILE_SIZE;
                    board.tiles[y][x] = ' ';
                }
                write_y--;
            }
        }

        // fill empty spots with new random tiles
        while (write_y >= 0) {
            board.tiles[write_y][x] = random_tile();
            board.fall_offset[write_y][x] = (write_y + 1) * TILE_SIZE;
            write_y--;
        }
    }

    tile_state = STATE_ANIMATING;
}

int main(void) {
    InitWindow(screenWidth, screenHeight, "Match 3 Game");
    SetTargetFPS(60);  
    srand(time(NULL));

    std::vector<float> boardVector = init_board(board);
	grid_origin = Vector2{ boardVector[0], boardVector[1] };

    if (find_matches()) {
        resolve_matches();
    }
    else {
        tile_state = STATE_IDLE;
    }

    Vector2 mouse = { 0, 0 };

    while (!WindowShouldClose()) 
    {
        mouse = GetMousePosition();
        if (tile_state == STATE_IDLE && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            int x = (mouse.x - grid_origin.x) / TILE_SIZE;
            int y = (mouse.y - grid_origin.y) / TILE_SIZE;

            if (x >= 0 && x < BOARD_SIZE && y >= 0 && y < BOARD_SIZE) {
				Vector2 current_tile = { static_cast<float>(x), static_cast<float>(y) };
                if (selected_tile.x < 0) {
                    selected_tile = current_tile;
                } else {
                    if (are_tiles_adjacent(selected_tile.x, selected_tile.y, current_tile.x, current_tile.y)) {
                        swap_tiles(board, (int)selected_tile.x, (int)selected_tile.y, current_tile.x, current_tile.y);
                        if (!find_matches()) {
                            // swap back if no match
                            swap_tiles(board, (int)selected_tile.x, (int)selected_tile.y, current_tile.x, current_tile.y);
                        } else {
                            resolve_matches();
                        }
                    }
                    selected_tile = { -1, -1 };
                }
            }
        }

        if (tile_state == STATE_ANIMATING) {
            bool still_animating = false;

            for (int y = 0; y < BOARD_SIZE; y++) {
                for (int x = 0; x < BOARD_SIZE; x++) {
                    if (board.fall_offset[y][x] > 0) {
                        board.fall_offset[y][x] -= fall_speed;
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
                tile_state = STATE_MATCH_DELAY;
                match_delay_timer = MATCH_DELAY_DURATION;
            }
        }

        if (tile_state == STATE_MATCH_DELAY) {
            match_delay_timer -= GetFrameTime();
            if (match_delay_timer <= 0.0f) {
                if (find_matches()) {
                    resolve_matches();
                }
                else {
                    tile_state = STATE_IDLE;
                }
            }
        }

        // update our score popups array
        for (int i = 0; i < MAX_SCORE_POPUPS; i++) {
            if (score_popups[i].active) {
                score_popups[i].lifetime -= GetFrameTime();
                score_popups[i].position.y -= 30 * GetFrameTime();
                score_popups[i].alpha = score_popups[i].lifetime;

                if (score_popups[i].lifetime <= 0.0f) {
                    score_popups[i].active = false;
                }
            }
        }

        // update the score animation
        if (score_animating) {
            score_scale += score_scale_velocity * GetFrameTime();
            if (score_scale <= 1.0f) {
                score_scale = 1.0f;
                score_animating = false;
            }
        }

        BeginDrawing();
        ClearBackground(BLACK);

        DrawRectangle(
            grid_origin.x,
            grid_origin.y,
            BOARD_SIZE * TILE_SIZE,
            BOARD_SIZE * TILE_SIZE,
            Fade(DARKGRAY, 0.60f)
        );

        for (int y = 0; y < BOARD_SIZE; y++) {
            for (int x = 0; x < BOARD_SIZE; x++) {
				Rectangle rect = { 
                    grid_origin.x + (x * TILE_SIZE),
                    grid_origin.y + (y * TILE_SIZE),
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

        if (selected_tile.x >= 0) {
            Rectangle selectedRect = {
                grid_origin.x + (selected_tile.x * TILE_SIZE),
                grid_origin.y + (selected_tile.y * TILE_SIZE),
                TILE_SIZE,
                TILE_SIZE
            };
            DrawRectangleLinesEx(selectedRect, 2, YELLOW);
        }
        DrawText(TextFormat("Score: %d", score), 20, 20, 24, YELLOW);

        // draw score popups
        for (int i = 0; i < MAX_SCORE_POPUPS; i++) {
            if (score_popups[i].active) {
                Color c = Fade(YELLOW, score_popups[i].alpha);
                DrawText(
                    TextFormat("+%d", score_popups[i].amount),
                    score_popups[i].position.x,
                    score_popups[i].position.y,
                    20, c);
            }
        }

        EndDrawing();
    }

    CloseWindow(); 
    return 0;
}