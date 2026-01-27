#pragma once
#include "animation.h"
#include "../board/board.h"

void init_animation(Animation& animation) {
    animation.score_animating = false;
    animation.score_scale = 1.0f;
    animation.score_scale_velocity = 0.0f;
    animation.fall_speed = 5.0f;
    animation.match_delay_timer = 0.0f;
    animation.match_delay_duration = 0.5f;
}

void add_score_popup(Animation& animation, int x, int y, int amount, Vec2Int grid_origin) {
    animation.score_animating = true;
    animation.score_scale = 2.0f;
    animation.score_scale_velocity = -2.5f;

    for (int i = 0; i < MAX_SCORE_POPUPS; i++) {
        if (!animation.score_popups[i].active) {
            Vec2Int popupPos = {
                grid_origin.x + x * TILE_SIZE + TILE_SIZE / 2,
                grid_origin.y + y * TILE_SIZE + TILE_SIZE / 2
            };
            animation.score_popups[i].position = popupPos;
            animation.score_popups[i].amount = amount;
            animation.score_popups[i].lifetime = 1.0f;
            animation.score_popups[i].alpha = 1.0f;
            animation.score_popups[i].active = true;
            break;
        }
    }
}

void animate_falling_blocks(Board& board, Animation& animation) {
    if (board.getTileState() == STATE_ANIMATING) {
        bool still_animating = false;

        for (int y = 0; y < BOARD_SIZE; y++) {
            for (int x = 0; x < BOARD_SIZE; x++) {
                if (board.getFallOffset(x, y) > 0) {
                    float fallOffset = board.getFallOffset(x, y);
					float newOffset = fallOffset - animation.fall_speed;
					board.setFallOffset(x, y, newOffset);
                    if (board.getFallOffset(x, y) < 0) {
                        board.setFallOffset(x, y, 0);
                    }
                    else {
                        still_animating = true;
                    }
                }
            }
        }

        if (!still_animating) {
            board.setTileState(STATE_MATCH_DELAY);
            animation.match_delay_timer = animation.match_delay_duration;
        }
    }
}

void update_scores(Animation& animation, float frame_time) {
    // update our score popups array
    for (int i = 0; i < MAX_SCORE_POPUPS; i++) {
        if (animation.score_popups[i].active) {
            animation.score_popups[i].lifetime -= frame_time;
            animation.score_popups[i].position.y -= 30 * frame_time;
            animation.score_popups[i].alpha = animation.score_popups[i].lifetime;

            if (animation.score_popups[i].lifetime <= 0.0f) {
                animation.score_popups[i].active = false;
            }
        }
    }

    // update the score animation
    if (animation.score_animating) {
        animation.score_scale += animation.score_scale_velocity * frame_time;
        if (animation.score_scale <= 1.0f) {
            animation.score_scale = 1.0f;
            animation.score_animating = false;
        }
    }
}