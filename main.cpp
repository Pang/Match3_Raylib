#include "raylib.h"
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>

#define BOARD_SIZE 8
#define TILE_SIZE 40
#define TILE_TYPES 5

const char tile_chars[TILE_TYPES] = { '#', '@', '$', '%', '&' };    
char board[BOARD_SIZE][BOARD_SIZE];
bool matched[BOARD_SIZE][BOARD_SIZE] = { 0 };
float fall_offset[BOARD_SIZE][BOARD_SIZE] = { 0 };

int score = 0;
Vector2 grid_origin;
Vector2 selected_tile = { -1, -1};
float fall_speed = 5.0f;

char random_tile() {
	return tile_chars[rand() % TILE_TYPES];
}

bool find_matches() {
    bool found = false;
    for (int y = 0; y < BOARD_SIZE; y++) {
        for (int x = 0; x < BOARD_SIZE; x++) {
            matched[y][x] = false;
        }
    }

    for (int y = 0; y < BOARD_SIZE; y++) {
        for (int x = 0; x < BOARD_SIZE - 2; x++) {
            char t = board[y][x];
            if (t == board[y][x + 1] &&
                t == board[y][x + 2]) {
                matched[y][x] = matched[y][x + 1] = matched[y][x + 2] = true;
                // update score
                score += 10;
                found = true;
            }
        }
    }

    for (int x = 0; x < BOARD_SIZE; x++) {
        for (int y = 0; y < BOARD_SIZE - 2; y++) {
            char t = board[y][x];
            if (t == board[y + 1][x] &&
                t == board[y + 2][x]) {
                matched[y][x] = matched[y + 1][x] = matched[y + 2][x] = true;
                score += 10;
                found = true;
            }
        }
    }

    return found;
}

void resolve_matches() {
    for (int x = 0; x < BOARD_SIZE; x++) {
        int write_y = BOARD_SIZE - 1;
        for (int y = BOARD_SIZE - 1; y >= 0; y--) {
            if (!matched[y][x]) {
                if (y != write_y) {
                    board[write_y][x] = board[y][x];
                    fall_offset[write_y][x] = (write_y - y) * TILE_SIZE;
                    board[y][x] = ' ';
                }
                write_y--;
            }
        }

        // fill empty spots with new random tiles
        while (write_y >= 0) {
            board[write_y][x] = random_tile();
            fall_offset[write_y][x] = (write_y + 1) * TILE_SIZE;
            write_y--;
        }
    }
}

void init_board() {
    for (int y = 0; y < BOARD_SIZE; y++) {
        for (int x = 0; x < BOARD_SIZE; x++) {
            board[y][x] = random_tile();
        }
    }

	int grid_width = BOARD_SIZE * TILE_SIZE;
	int grid_height = BOARD_SIZE * TILE_SIZE;

	grid_origin = Vector2 { (GetScreenWidth() - grid_width) / 2.0f, (GetScreenHeight() - grid_height) / 2.0f };
}

int main(void)
{
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "Match 3 Game");
    SetTargetFPS(60);  

    srand(time(NULL));
	init_board();

    Vector2 mouse = { 0, 0 };

    while (!WindowShouldClose()) 
    {
        mouse = GetMousePosition();
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            int x = (mouse.x - grid_origin.x) / TILE_SIZE;
            int y = (mouse.y - grid_origin.y) / TILE_SIZE;

            if (x >= 0 && x < BOARD_SIZE && y >= 0 && y < BOARD_SIZE) {
                selected_tile = { static_cast<float>(x), static_cast<float>(y) };
            }
        }

		for (int y = 0; y < BOARD_SIZE; y++) {
            for (int x = 0; x < BOARD_SIZE; x++) {
                if (fall_offset[y][x] > 0) {
                    fall_offset[y][x] -= fall_speed;
                    if (fall_offset[y][x] < 0) {
                        fall_offset[y][x] = 0;
                    }
                }
            }
        }

        if (find_matches()) {
			resolve_matches();
        }

        BeginDrawing();
        ClearBackground(BLACK);

        for (int y = 0; y < BOARD_SIZE; y++) {
            for (int x = 0; x < BOARD_SIZE; x++) {
				Rectangle rect = { 
                    grid_origin.x + (x * TILE_SIZE),
                    grid_origin.y + (y * TILE_SIZE),
                    TILE_SIZE, 
                    TILE_SIZE 
                };

				DrawRectangleLinesEx(rect, 1, DARKGRAY);

                if (board[y][x] != ' ') {
                    Vector2 position = { rect.x + 12, rect.y + 8 - fall_offset[y][x]};
                    DrawTextEx(
                        GetFontDefault(),
                        TextFormat("%c", board[y][x]),
                        position,
                        20, 1,
                        matched[y][x] ? GREEN : YELLOW
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
        EndDrawing();
    }

    CloseWindow(); 
    return 0;
}