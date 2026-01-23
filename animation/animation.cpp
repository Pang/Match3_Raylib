#pragma once
#include "animation.h"


void init_animation(Animation& animation) {
    animation.score_animating = false;
    animation.score_scale = 1.0f;
    animation.score_scale_velocity = 0.0f;
    animation.fall_speed = 5.0f;
    animation.match_delay_timer = 0.0f;
    animation.match_delay_duration = 0.5f;
}

