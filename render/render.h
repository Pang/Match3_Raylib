#pragma once
#include "../animation/animation.h"
#include "../board/board.h"
#include "../board/config.h"

class Render
{

public:
	void drawEntireBoard(Board& board, Animation& animation, int score, Rectangle resetButton);
	void drawResetButton(Rectangle button);

private:
	void drawBoard(int x, int y);
	void drawTiles(Board& board);
	void drawSelected(Board& board);
	void drawScorePopups(Animation& animation);

};

