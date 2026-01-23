#pragma once

typedef enum {
    STATE_IDLE,
    STATE_ANIMATING,
    STATE_MATCH_DELAY
} TileState;

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
