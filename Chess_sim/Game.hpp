#include "Move.hpp"
#include "Position.hpp"
#include "LegalMoveGen.hpp"
#include "MoveList.hpp"
#include "raylib.h"
#include <map>
#include <string>
#include <vector>
#pragma once


class Game
{
public:
	Game(Position position);
	int processMove(Position& position, MoveList& moves, uint8_t from, uint8_t to, uint8_t side);
	int processMoveWithClick();
	int processGame();

	static bool isPromotionMove(Move& move);
	void proccesPromotionClick(Move& move);
	std::string handlePromotionSelection(int promotionIndex);
	void handlePromotion(Move& move);
	bool checkVictory(uint8_t side);
	uint8_t squareToIndex(const std::string& square) const;
	std::string indexToSquare(uint8_t index) const;
	std::pair<int, int> indexToRowCol(uint8_t index) const;
	uint8_t getSelectedSquare() const { return selectedSquare; }

	void loadPieceTextures();
	void drawBoard();
	void drawPieces();
	void drawMask();
	void drawPromotionOptions();
	Vector2 centerPiece(float pieceSize, float texWidth, float texHeight) const;

	void selectPiece();
	void update();
private:
	Position position;
	MoveList selectedPieceMoves;
	Move currentMove;
	const std::string startingPosition = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR";
	const std::string whitePiecesPath = "..\\..\\..\\..\\recources\\ChessPieces\\White\\";
	const std::string blackPiecesPath = "..\\..\\..\\..\\recources\\ChessPieces\\Black\\";
	const std::string names[6] = { "King", "Queen", "Rook", "Bishop", "Knight", "Pawn" };
	std::vector<std::string> promotionPieces = { "Queen", "Rook", "Bishop", "Knight" };
	std::map<std::string, Texture2D> whitePieces;
	std::map<std::string, Texture2D> blackPieces;

	int8_t selectedSquare;
	bool isSelected = false;
	const int boardSize = 8;
	const int squareSize = 80;
	int clickCount = 0;

	bool promotion = false;
	uint8_t promotionSquare = -1;
	uint8_t promotionSide;
	std::string pieceToPromote;

	Vector2 mousePos;
};