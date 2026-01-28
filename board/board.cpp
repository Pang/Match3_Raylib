#pragma once
#include "../animation/animation.h"
#include "board.h"
#include "config.h"
#include "raylib.h"
#include <stdlib.h>
#include <vector>

const char tile_chars[TILE_TYPES] = { '#', '@', '$', '%', '&' };
const float MATCH_DELAY_DURATION = 0.5f;

TileState Board::getTileState() const {
    return tile_state;
}
void Board::setTileState(TileState state) {
	tile_state = state;
}

const Vec2Int& Board::getSelectedTile() const {
    return selected_tile;
}
void Board::setSelectedTile(const Vec2Int& tile) {
    selected_tile = tile;
}

const Vec2Int& Board::getGridOrigin() const {
    return grid_origin;
}

float Board::getFallOffset(int x, int y) const {
    return fall_offset[y][x];
}
void Board::setFallOffset(int x, int y, float newValue) {
    fall_offset[y][x] = newValue;
}

int Board::getTile(int x, int y) const {
    return tiles[y][x];
}
bool Board::isMatched(int x, int y) const {
    return matched[y][x];
}


Board::Board()
{
    tile_state = TileState::STATE_IDLE;
    int grid_width = BOARD_SIZE * TILE_SIZE;
    int grid_height = BOARD_SIZE * TILE_SIZE;
    grid_origin = { (GetScreenWidth() - grid_width) / 2, (GetScreenHeight() - grid_height) / 2 };
    selected_tile = { -1, -1 };

    loadTextures();
}

void Board::init() {
    for (int y = 0; y < BOARD_SIZE; y++) {
        for (int x = 0; x < BOARD_SIZE; x++) {
            tiles[y][x] = randomTile();
            matched[y][x] = false;
            fall_offset[y][x] = 0.0f;
        }
    }
}

char Board::randomTile() {
    return static_cast<TileType>(rand() % TILE_COUNT);
}

void Board::swapTiles(int x1, int y1, int x2, int y2) {
    char temp = tiles[y1][x1];
    tiles[y1][x1] = tiles[y2][x2];
    tiles[y2][x2] = temp;
}

FoundMatchesResponse Board::findMatches(int score) {
	FoundMatchesResponse found_matches_response = {};
    found_matches_response.foundMatches = false;
    found_matches_response.updatedScore = score;
    found_matches_response.matchPositions = {};

    for (int y = 0; y < BOARD_SIZE; y++) {
        for (int x = 0; x < BOARD_SIZE; x++) {
            matched[y][x] = false;
        }
    }

    for (int y = 0; y < BOARD_SIZE; y++) {
        for (int x = 0; x < BOARD_SIZE - 2; x++) {
            char t = tiles[y][x];
            if (t == tiles[y][x + 1] && t == tiles[y][x + 2]) {
                matched[y][x] = matched[y][x + 1] = matched[y][x + 2] = true;
                found_matches_response.updatedScore += 10;
                found_matches_response.foundMatches = true;
				found_matches_response.matchPositions.push_back({ x, y });
            }
        }
    }

    for (int x = 0; x < BOARD_SIZE; x++) {
        for (int y = 0; y < BOARD_SIZE - 2; y++) {
            char t = tiles[y][x];
            if (t == tiles[y + 1][x] && t == tiles[y + 2][x]) {
                matched[y][x] = matched[y + 1][x] = matched[y + 2][x] = true;
                found_matches_response.updatedScore += 10;
                found_matches_response.foundMatches = true;
                found_matches_response.matchPositions.push_back({ x, y });
            }
        }
    }

    return found_matches_response;
}

void Board::resolveMatches() {
    for (int x = 0; x < BOARD_SIZE; x++) {
        int write_y = BOARD_SIZE - 1;
        for (int y = BOARD_SIZE - 1; y >= 0; y--) {
            if (!matched[y][x]) {
                if (y != write_y) {
                    tiles[write_y][x] = tiles[y][x];
                    fall_offset[write_y][x] = (write_y - y) * TILE_SIZE;
                    tiles[y][x] = ' ';
                }
                write_y--;
            }
        }

        // fill empty spots with new random tiles
        while (write_y >= 0) {
            tiles[write_y][x] = randomTile();
            fall_offset[write_y][x] = (write_y + 1) * TILE_SIZE;
            write_y--;
        }
    }

	setTileState(STATE_ANIMATING);
}

bool Board::areTilesAdjacent(int x1, int y1, int x2, int y2) const {
    return (abs((int)x1 - (int)x2) + abs((int)y1 - (int)y2)) == 1;
}

void Board::loadTextures() {
    tileTextures[TILE_RED] = LoadTexture("assets/heart.png");
    tileTextures[TILE_BLUE] = LoadTexture("assets/ghost.png");
    tileTextures[TILE_GREEN] = LoadTexture("assets/invader.png");
    tileTextures[TILE_YELLOW] = LoadTexture("assets/star.png");
    tileTextures[TILE_PURPLE] = LoadTexture("assets/gamepad.png");
}