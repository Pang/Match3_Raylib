#pragma once
#include "../animation/animation.h"
#include "../board/board.h"
#include "../board/config.h"

typedef struct {
} Render;

void draw_entire_board(Board& board, Animation& animation, int score);
void draw_board(int x, int y);
void draw_tiles(Board& board);
void draw_selected(Board& board);
void draw_score_popups(Animation& animation);