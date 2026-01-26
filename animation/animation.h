#pragma once
#include "../board/config.h"
#include "../board/board.h"
#include "../types/types.h"

typedef struct {
    ScorePopup score_popups[MAX_SCORE_POPUPS];
    float score_scale;
    float score_scale_velocity;
    bool score_animating;
    float fall_speed;
    float match_delay_timer;
    float match_delay_duration;
} Animation;

void init_animation(Animation& animation);
void add_score_popup(Animation& animation, int x, int y, int amount, Vec2Int grid_origin);
void animate_falling_blocks(Board& board, Animation& animation);
void update_scores(Animation& animation, float frame_time);