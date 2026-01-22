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

int score = 0;
Vector2 grid_origin;

char random_title() {
	return tile_chars[rand() % TILE_TYPES];
}

void init_board() {
    for (int y = 0; y < BOARD_SIZE; y++) {
        for (int x = 0; x < BOARD_SIZE; x++) {
            board[y][x] = random_title();
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

    while (!WindowShouldClose()) 
    {
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

				Vector2 position = { rect.x + 12, rect.y + 8 };
                DrawTextEx(
                    GetFontDefault(),
                    TextFormat("%c", board[y][x]),
                    position,
                    20, 1, WHITE
                );
            }
		}
        DrawText(TextFormat("Score: %d", score), 20, 20, 24, YELLOW);
        EndDrawing();
    }

    CloseWindow(); 
    return 0;
}