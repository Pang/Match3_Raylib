#pragma once
#include "../board/config.h"
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

