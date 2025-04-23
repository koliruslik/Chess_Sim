#include "Move.hpp"
#include "Position.hpp"
#include "LegalMoveGen.hpp"
#include "MoveList.hpp"
#include "raylib.h"
#include <map>
#include <string>
#pragma once


class Game
{
public:
	Game(Position position);
	int processMove(Position& position, MoveList& moves, std::string& from, std::string& to, SIDE side);
	int processGame();

	static void handlePawnPromotion(Move& move);
	bool checkVictory(SIDE side);
	int8_t convertToIndex(const std::string& input);
	void loadPieceTextures();
	void drawBoard();
	void drawPieces();

	Vector2 centerPiece(float pieceSize, float texWidth, float texHeight) const;

private:
	Position position;
	MoveList allMoves;
	const std::string startingPosition = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR";
	const std::string whitePiecesPath = "..\\..\\..\\..\\recources\\ChessPieces\\White\\";
	const std::string blackPiecesPath = "..\\..\\..\\..\\recources\\ChessPieces\\Black\\";
	const std::string names[6] = { "King", "Queen", "Rook", "Bishop", "Knight", "Pawn" };
	std::map<std::string, Texture2D> whitePieces;
	std::map<std::string, Texture2D> blackPieces;

	const int boardSize = 8;
	const int squareSize = 80;
};