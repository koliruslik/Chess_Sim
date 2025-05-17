#include <map>
#include <string>
#include <vector>


#include "Move.hpp"
#include "Position.hpp"
#include "LegalMoveGen.hpp"
#include "MoveList.hpp"
#include "raylib.h"
#include "AI.h"
#include "MainMenu.h"
#include "Init.h"
#include "BoardRenderer.h"
#pragma once

class Game
{
public:
	Game(Position position, SIDE aiSideToPlay);
	int processMove(Position& position, MoveList& moves, uint8_t from, uint8_t to, uint8_t side);
	int processMoveWithClick();
	int proccesAiMove();
	void proccessAiMoveAsync();
	void processGame();

	static bool isPromotionMove(Move& move);
	void proccesPromotionClick(Move& move);
	std::string handlePromotionSelection(int promotionIndex);
	void handlePromotion(Move& move);
	SIDE checkVictory() const
	{
		return checkVictory(position);
	}
	static SIDE checkVictory(const Position position);
	
	std::string generateAnnotation();
	

	
	void drawBoard();
	void drawPieces();
	void drawMask();
	void drawPromotionOptions();
	void drawCoordinates();
	Vector2 centerPiece(float pieceSize, float texWidth, float texHeight) const;

	void selectPiece();
	void update();
	uint8_t getCurrentSideToMove(float moveCtr) { return (moveCtr == static_cast<int>(moveCtr)) ? SIDE::Black : SIDE::White; }
	uint8_t getSelectedSquare() const { return selectedSquare; }
	SIDE getWonSide() const { return wonSide; }
	void setAiSideToPlay(SIDE side) { aiSide = side; }
	void setTheme(Theme newTheme);
	

	void resetPosition();
private:
	BoardRenderer bRenderer = BoardRenderer(80);
	Position position;
	MoveList selectedPieceMoves;
	MoveList movesHistory;
	Move currentMove;
	const std::string startingPosition = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR";
	const std::string savePath = "..\\..\\..\\..\\saves\\save.txt";
	const std::string loadPath = "..\\..\\..\\..\\saves\\load.txt";
	const std::string names[6] = { "King", "Queen", "Rook", "Bishop", "Knight", "Pawn" };
	std::vector<std::string> promotionNames = { "Queen", "Rook", "Bishop", "Knight" };
	int8_t selectedSquare;
	bool isSelected = false;
	const int boardSize = 8;
	const int squareSize = 80;
	int clickCount = 0;
	const int constOffset = 5;
	const int OffsetXForRows = squareSize - 15;
	const int OffsetYForRows = 5;
	const int OffsetXForFiles = 5;
	const int OffsetYForFiles = squareSize - 20;
	Color maskColor = { 160, 112, 112, 136 };
	SIDE wonSide = SIDE::None;

	bool promotionOption = false;
	bool promotionDone = false;
	uint8_t promotionSquare = -1;
	uint8_t promotionSide;
	std::string pieceToPromote;

	Vector2 mousePos;

	AI ai;
	bool aiThinking = false;
	uint8_t aiSide;
	std::thread aiMoveThread;
	int32_t minMS = 0;
	int32_t maxMs = 1500;

	const bool debugMode = false;
	const bool debugMoves = true;
	bool PrintMove();
	bool printMove = false;
	void clearFile(std::string filePath);

	Theme theme;
	std::map<Theme, const PieceTextures*> allPieceTextures;
	const Texture2D* squareWhiteTex = nullptr;
	const Texture2D* squareBlackTex = nullptr;
	
};