#include "raylib.h"
#include "Chess_sim.h"
#include <bitset> 
#include <iostream>
#include <direct.h>

#include "MainMenu.h"
#include "Button.h"
#include "Timer.h"
#include "Pieces.hpp"
#include "LegalMoveGenTester.hpp"
#include "PsLegalMoveMaskGen.hpp"
#include "Game.hpp"
#include "GameEnums.h"


using namespace std;


enum GameState
{
	MAIN_MENU,
	SETTINGS,
	GAMEPLAY,
    POSCONSTRUCTOR
};


int main()//f2 f3 e7 e5 g2 g4 d8 h4
{
    const int boardSize = 8;
    const int squareSize = 80;
    const int screenWidth = boardSize * squareSize;
    const int screenHeight = boardSize * squareSize;
    const char* title = "Chess_sim";
    InitWindow(screenWidth, screenHeight, title);
    SetTargetFPS(60);

    Timer timer;
    GameState gameState = MAIN_MENU;
    MainMenu mainMenu(screenWidth, screenHeight);
    //SettingsMenu settingsMenu(screenWidth, screenHeight);
    Position position("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR", Position::NONE, true, true, true, true, 0.0f);
    //Position position("1ppp4/Ppkp4/1ppp4/8/8/8/8/7K", Position::NONE, true, true, true, true, 0.0f);
    //Position position("k7/8/8/8/8/q7/8/K7", Position::NONE, true, true, true, true, 0.0f);
    
    

	SIDE wonSide = SIDE::None;
    Game game(position, SIDE::White);
    MenuResult result;
    Theme theme;
    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(BLACK);

        switch (gameState)
        {
        case MAIN_MENU:
            mainMenu.DrawMenu();
            mainMenu.Update();
            DrawText(timer.GetCurrentTime().c_str(), 10, 10, 20, WHITE);
            result = mainMenu.getResult();
            if (result == MenuResult::Exit)
                CloseWindow();
            else if (result == MenuResult::StartGame)
            {
                gameState = GAMEPLAY;

                game.setAiSideToPlay(mainMenu.getSideToPlay());
                theme = mainMenu.getTheme();
                game.setTheme(theme);
            }
            else if (result == MenuResult::OpenSettings)
                gameState = SETTINGS;
            else if (result == MenuResult::ConstructPosition)
                gameState = POSCONSTRUCTOR;
            break;

        case SETTINGS:
            DrawText("SETTINGS", screenWidth / 2 - MeasureText("SETTINGS", 20), 0, 20, WHITE);
            /*settingsMenu.DrawMenu();
            settingsMenu.Update();*/
            break;

        case GAMEPLAY:
            game.processGame();
            wonSide = game.getWonSide();
            if(wonSide == SIDE::White) DrawText("White wins!", screenWidth / 2 - MeasureText("White wins!", 20), screenHeight / 2, 50, BLACK);
			else if (wonSide == SIDE::Black) DrawText("Black wins!", screenWidth / 2 - MeasureText("Black wins!", 20), screenHeight / 2, 50, BLACK);
			else if (wonSide == SIDE::Draw) DrawText("Draw!", screenWidth / 2 - MeasureText("Draw!", 20), screenHeight / 2, 50, BLACK);

            // Отрисовка доски
            break;
        case POSCONSTRUCTOR:
            break;
        }
        
            

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
