#pragma once
#include "../animation/animation.h"
#include "board.h"
#include "config.h"
#include "raylib.h"
#include <stdlib.h>
#include <vector>

const char tile_chars[TILE_TYPES] = { '#', '@', '$', '%', '&' };
const float MATCH_DELAY_DURATION = 0.5f;

void init_board(Board& board) {
    board.tile_state = STATE_IDLE;

    for (int y = 0; y < BOARD_SIZE; y++) {
        for (int x = 0; x < BOARD_SIZE; x++) {
            board.tiles[y][x] = random_tile();
        }
    }

    int grid_width = BOARD_SIZE * TILE_SIZE;
    int grid_height = BOARD_SIZE * TILE_SIZE;
	board.grid_origin = { (GetScreenWidth() - grid_width) / 2, (GetScreenHeight() - grid_height) / 2 };
	board.selected_tile = { -1, -1 };
}

char random_tile() {
    return tile_chars[rand() % TILE_TYPES];
}

void swap_tiles(Board& board, int x1, int y1, int x2, int y2) {
    char temp = board.tiles[y1][x1];
    board.tiles[y1][x1] = board.tiles[y2][x2];
    board.tiles[y2][x2] = temp;
}

bool are_tiles_adjacent(int x1, int y1, int x2, int y2) {
    return (abs((int)x1 - (int)x2) + abs((int)y1 - (int)y2)) == 1;
}

FoundMatchesResponse find_matches(Board& board, int score) {
	FoundMatchesResponse found_matches_response = {};
    found_matches_response.foundMatches = false;
    found_matches_response.updatedScore = score;
    found_matches_response.matchPositions = {};

    for (int y = 0; y < BOARD_SIZE; y++) {
        for (int x = 0; x < BOARD_SIZE; x++) {
            board.matched[y][x] = false;
        }
    }

    for (int y = 0; y < BOARD_SIZE; y++) {
        for (int x = 0; x < BOARD_SIZE - 2; x++) {
            char t = board.tiles[y][x];
            if (t == board.tiles[y][x + 1] && t == board.tiles[y][x + 2]) {
                board.matched[y][x] = board.matched[y][x + 1] = board.matched[y][x + 2] = true;
                found_matches_response.updatedScore += 10;
                found_matches_response.foundMatches = true;
				found_matches_response.matchPositions.push_back({ x, y });
            }
        }
    }

    for (int x = 0; x < BOARD_SIZE; x++) {
        for (int y = 0; y < BOARD_SIZE - 2; y++) {
            char t = board.tiles[y][x];
            if (t == board.tiles[y + 1][x] && t == board.tiles[y + 2][x]) {
                board.matched[y][x] = board.matched[y + 1][x] = board.matched[y + 2][x] = true;
                found_matches_response.updatedScore += 10;
                found_matches_response.foundMatches = true;
                found_matches_response.matchPositions.push_back({ x, y });
            }
        }
    }

    return found_matches_response;
}

void resolve_matches(Board& board) {
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

    board.tile_state = STATE_ANIMATING;
}