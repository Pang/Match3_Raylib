#pragma once
#include "../board/config.h"
#include "../board/board.h"
#include "../types/types.h"

class Animation
{
    public:
		Animation();

        void animateFallingBlocks(Board& board);
        void addScorePopup(int x, int y, int amount, Vec2Int grid_origin);
        void updateScorePopups(float frame_time);

    private:
        float score_scale;
        float score_scale_velocity;
        bool score_animating;

    public:
        ScorePopup score_popups[MAX_SCORE_POPUPS];
        float fall_speed;
        float match_delay_timer;
	    float match_delay_duration;
};
