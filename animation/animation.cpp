#pragma once
#include "animation.h"
#include "../board/board.h"

Animation::Animation() {
    score_animating = false;
    score_scale = 1.0f;
    score_scale_velocity = 0.0f;
    fall_speed = 5.0f;
    match_delay_timer = 0.0f;
    match_delay_duration = 0.5f;
}

void Animation::addScorePopup(int x, int y, int amount, Vec2Int grid_origin) {
    score_animating = true;
    score_scale = 2.0f;
    score_scale_velocity = -2.5f;

    for (int i = 0; i < MAX_SCORE_POPUPS; i++) {
        if (!score_popups[i].active) {
            Vec2Int popupPos = {
                grid_origin.x + x * TILE_SIZE + TILE_SIZE / 2,
                grid_origin.y + y * TILE_SIZE + TILE_SIZE / 2
            };
            score_popups[i].position = popupPos;
            score_popups[i].amount = amount;
            score_popups[i].lifetime = 1.0f;
            score_popups[i].alpha = 1.0f;
            score_popups[i].active = true;
            break;
        }
    }
}

void Animation::animateFallingBlocks(Board& board) {
    if (board.getTileState() == STATE_ANIMATING) {
        bool still_animating = false;

        for (int y = 0; y < BOARD_SIZE; y++) {
            for (int x = 0; x < BOARD_SIZE; x++) {
                if (board.getFallOffset(x, y) > 0) {
                    float fallOffset = board.getFallOffset(x, y);
					float newOffset = fallOffset - fall_speed;
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
            match_delay_timer = match_delay_duration;
        }
    }
}

void Animation::updateScorePopups(float frame_time) {
    for (int i = 0; i < MAX_SCORE_POPUPS; i++) {
        if (score_popups[i].active) {
            score_popups[i].lifetime -= frame_time;
            score_popups[i].position.y -= 30 * frame_time;
            score_popups[i].alpha = score_popups[i].lifetime;

            if (score_popups[i].lifetime <= 0.0f) {
                score_popups[i].active = false;
            }
        }
    }

    // update the score animation
    if (score_animating) {
        score_scale += score_scale_velocity * frame_time;
        if (score_scale <= 1.0f) {
            score_scale = 1.0f;
            score_animating = false;
        }
    }
}
