#pragma once

#include "config.h"
#include "../types/types.h"
#include <raylib.h>

class Board
{
    public:
        Board();

        void init();
        void resolveMatches();

        void swapTiles(int x1, int y1, int x2, int y2);
        bool areTilesAdjacent(int x1, int y1, int x2, int y2) const;

        FoundMatchesResponse findMatches(int currentScore);

        TileState getTileState() const;
        void setTileState(TileState state);

        const Vec2Int& getGridOrigin() const;

        const Vec2Int& getSelectedTile() const;
        void setSelectedTile(const Vec2Int& tile);

        float getFallOffset(int x, int y) const;
        void setFallOffset(int x, int y, float newValue);


        int getTile(int x, int y) const;
        bool isMatched(int x, int y) const;

    private:
        char randomTile();
        void loadTextures();

    public:
        Texture2D tileTextures[TILE_COUNT];

    private:
        TileState tile_state;
        Vec2Int grid_origin;
        Vec2Int selected_tile;

        int tiles[BOARD_SIZE][BOARD_SIZE];
        bool matched[BOARD_SIZE][BOARD_SIZE];
        float fall_offset[BOARD_SIZE][BOARD_SIZE];
};