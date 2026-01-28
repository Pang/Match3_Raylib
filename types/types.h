#pragma once
#include <vector>

typedef enum {
    STATE_IDLE,
    STATE_ANIMATING,
    STATE_MATCH_DELAY
} TileState;

enum TileType {
    TILE_RED,
    TILE_BLUE,
    TILE_GREEN,
    TILE_YELLOW,
    TILE_PURPLE,
    TILE_COUNT,
    TILE_NONE
};

typedef struct Vec2Int
{
    int x;
    int y;
};

typedef struct {
    Vec2Int position;
    int amount;
    float lifetime;
    float alpha;
    bool active;
} ScorePopup;

struct FoundMatchesResponse
{
    bool foundMatches = false;
    int updatedScore = 0;
    std::vector<Vec2Int> matchPositions;
};
