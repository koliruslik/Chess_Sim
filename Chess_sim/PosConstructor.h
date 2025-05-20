#pragma once

#include "Position.hpp"
#include "BoardRenderer.h"
#include "raylib.h"
#include "BTrans.h"

#include <cstdlib>
class PosConstructor
{
public:
	PosConstructor(Position position);
	void update();
	MenuResult getResult() { return result; }
	Position getPosition() { return position; }
	int getKingCounter() { return wKingCounter + bKingCounter; }
private:
	Position position;
	BoardRenderer bRenderer = BoardRenderer(80);
	MenuResult result;
	Theme theme = Theme::Theme1;
	int squareSize = 80;
	int boardSize = 8;
	int wKingCounter = 0;
	int bKingCounter = 0;
	Vector2 mousePos;

	struct SelectedPiece {
		int8_t type = PIECE::NONE;
		int8_t side = SIDE::White;
		bool isWhite = true;
	};

	SelectedPiece selected;
	

	void SelectSquare(bool isPlacing);
	void SelectPiece();
	bool isKingAdjacent(uint8_t squareIndex, uint8_t side);
};