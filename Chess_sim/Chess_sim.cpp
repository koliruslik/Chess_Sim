// Chess_sim.cpp: определяет точку входа для приложения.
//
#include "raylib.h"
#include "Chess_sim.h"
#include <bitset> 

#include "MainMenu.h"
#include "Button.h"
#include "Timer.h"
#include "Pieces.hpp"
#include "LegalMoveGenTester.hpp"
#include "StandartMovingNotation.hpp"
#include "PsLegalMoveMaskGen.hpp"
using namespace std;


enum GameState
{
	MAIN_MENU,
	SETTINGS,
	GAMEPLAY
};

void handlePawnPromotion(Move& move) 
{
	// Проверка на превращение пешки
	uint8_t to = move.getTo();
	if (move.getAttackerType() == PIECE::PAWN) 
	{
		if ((move.getAttackerSide() == SIDE::White && to >= 56) ||
			(move.getAttackerSide() == SIDE::Black && to <= 7))
		{
			std::string pieceChoice;
			std::cout << "Pawn reached the last row! Choose a promotion piece (queen, rook, knight, bishop): ";
			std::cin >> pieceChoice;

			if (pieceChoice == "queen") {
				std::cout << "PROMOTE TO QUEEN\n";
				move.setFlag(Move::FLAG::PROMOTE_TO_QUEEN);
			}
			else if (pieceChoice == "rook") {
				std::cout << "PROMOTE TO ROOK\n";
				move.setFlag(Move::FLAG::PROMOTE_TO_ROOK);
			}
			else if (pieceChoice == "knight") {
				std::cout << "PROMOTE TO KNIGHT\n";
				move.setFlag(Move::FLAG::PROMOTE_TO_KNIGHT);
			}
			else if (pieceChoice == "bishop") {
				std::cout << "PROMOTE TO BISHOP\n";
				move.setFlag(Move::FLAG::PROMOTE_TO_BISHOP);
			}
			else {
				std::cout << "Invalid choice, defaulting to queen.\n";
				move.setFlag(Move::FLAG::PROMOTE_TO_QUEEN);
			}
		}
	}
}

static int8_t convertToIndex(const std::string& input) {
	if (input.length() != 2) return -1;
	char file = input[0];
	char rank = input[1];

	if (file < 'a' || file > 'h' || rank < '1' || rank > '8') return -1;

	return (rank - '1') * 8 + (file - 'a');
}

static std::string sideToMove(float moveCtr)
{
	return (moveCtr == static_cast<int>(moveCtr)) ? "White" : "Black";
}

bool checkVictory(MoveList& moves, SIDE side) {
	if (!moves.hasMoves()) {
		std::cout << (side == SIDE::White ? "BLACK WON" : "WHITE WON") << "\n";
		return true;
	}
	return false;
}
bool processMove(Position& position, MoveList& moves, std::string& from, std::string& to, SIDE side)
{
	
	int fromIndex = convertToIndex(from);
	int toIndex = convertToIndex(to);

	if (fromIndex == -1 || toIndex == -1) {
		std::cout << "INVALID.\n";
		return false;
	}

	moves = LegalMoveGen::generate(position, side);
	bool validMove = false;

	if (!moves.hasMoves()) {
		std::cout << (side == SIDE::White ? "BLACK WON" : "WHITE WON") << "\n";
		return false;
	}

	for (int i = 0; i < moves.getSize(); ++i) {
		if (moves[i].getFrom() == fromIndex && moves[i].getTo() == toIndex) {
			handlePawnPromotion(moves[i]);
			position.move(moves[i]);
			validMove = true;
			break;
		}
	}
	std::cout << position;
	if (!validMove) {
		std::cout << "Invalid move.\n";
		return false;
	}
	
	return true;
}

int main()
{
	//LegalMoveGenTester::runTests();
	const int screenWidth = 1920;
	const int screenHeight = 1080;
	const char* title = "Chess_sim";
	//InitWindow(screenWidth, screenHeight, title);
	Timer timer;
	GameState gameState = MAIN_MENU;
	MainMenu mainMenu(screenWidth, screenHeight);
	Button startGameButton(screenWidth / 2, screenHeight / 2 - 25, 150, 50, "Start Game");
	Button exitGameButton(screenWidth / 2, screenHeight / 2 + 25, 150, 50, "Exit");
	Pieces pieces("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR");
	Position position("rnbqkbnr/Pppppppp/8/8/8/8/pPPPPPPP/RNBQKBNR", Position::NONE, true, true, true, true, 0.0f);


	MoveList moves = LegalMoveGen::generate(position, SIDE::Black);
	while (true) {
		std::cout << position;
		if (sideToMove(position.moveCtr) == "White") {
			std::string from, to;
			std::cout << "Enter move (White): ";
			std::cin >> from >> to;

			if (!processMove(position, moves, from, to, SIDE::White)) {
				continue;
			}
			else {
				std::string from, to;
				std::cout << "Enter move (Black): ";
				std::cin >> from >> to;

				if (!processMove(position, moves, from, to, SIDE::Black)) {
					continue;
				}
			}

		}

		//SetTargetFPS(60);
		//int i = 0;
		//while (!WindowShouldClose())
		//{
		//	BeginDrawing();
		//	ClearBackground(BLACK);
		//	switch (gameState)
		//	{
		//	case MAIN_MENU:
		//		
		//		mainMenu.DrawMenu();
		//		mainMenu.Update();
		//		
		//		DrawText(timer.GetCurrentTime().c_str(), 10, 10, 20, WHITE);
		//		if (mainMenu.shouldExitGame())
		//		{
		//			CloseWindow();
		//		}
		//		else if (mainMenu.shouldStartGame())
		//		{
		//			gameState = GAMEPLAY;
		//		}
		//		else if (mainMenu.isSettingsMenu())
		//		{
		//			gameState = SETTINGS;
		//		}
		//		
		//		break;
		//	case SETTINGS:
		//		DrawText("SETTINGS", screenWidth / 2 - MeasureText("SETTINGS", 20), 0, 20, WHITE);//temporary
		//		break;
		//	case GAMEPLAY:
		//		DrawText("CHESS", screenWidth / 2 - MeasureText("CHESS", 20), 0, 20, WHITE);//temporary
		//		break;
		//	default:
		//		break;
		//	}
		//	EndDrawing();
		//}
		//cout << GetScreenWidth() << endl;
		
	}
	return 0;
}
