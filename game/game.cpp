#pragma once
#include "raylib.h"
#include "../board/board.h"
#include "../animation/animation.h"
#include "../render/render.h"

//Board board{};
//Animation animation{};
//
//void match_found(FoundMatchesResponse& found_matches_response) {
//    for (int i = 0; i < found_matches_response.matchPositions.size(); i++) {
//        Vec2Int pos = found_matches_response.matchPositions[i];
//        add_score_popup(animation, pos.x, pos.y, 10, board.grid_origin);
//    }
//    resolve_matches(board);
//}
//
//void board_interaction(FoundMatchesResponse& found_matches_response, int score) {
//    Vector2 mouse = GetMousePosition();
//    if (board.tile_state == STATE_IDLE && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
//        int x = (mouse.x - board.grid_origin.x) / TILE_SIZE;
//        int y = (mouse.y - board.grid_origin.y) / TILE_SIZE;
//
//        if (x >= 0 && x < BOARD_SIZE && y >= 0 && y < BOARD_SIZE) {
//            if (board.selected_tile.x < 0) {
//                board.selected_tile = { x, y };
//            }
//            else {
//                if (are_tiles_adjacent(board.selected_tile.x, board.selected_tile.y, x, y)) {
//                    swap_tiles(board, board.selected_tile.x, board.selected_tile.y, x, y);
//
//                    found_matches_response = find_matches(board, score);
//                    score = found_matches_response.updatedScore;
//
//                    if (found_matches_response.foundMatches) {
//                        match_found(found_matches_response);
//                    }
//                    else {
//                        swap_tiles(board, board.selected_tile.x, board.selected_tile.y, x, y);
//                    }
//                }
//                board.selected_tile = { -1, -1 };
//            }
//        }
//    }
// }