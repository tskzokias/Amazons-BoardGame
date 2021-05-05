#pragma once
#include <utility>
class WholeBoard;

// piece's x and y position
typedef std::pair<int, int> piecepos;

// first is piece position stored then its move location
typedef std::pair<piecepos, piecepos> piecepos_moveloc;

// first is piece position and then the player scope due to that move
typedef std::pair<piecepos, int> piecepos_scope;

typedef std::pair<piecepos, std::pair<piecepos, piecepos>> piecepos_moveloc_placeX;

typedef std::pair<WholeBoard, int> board_scope;