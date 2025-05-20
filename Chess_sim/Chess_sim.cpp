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
#include "PosConstructor.h"
#include "ESCMenu.h"

using namespace std;


enum GameState
{
	MAIN_MENU,
	SETTINGS,
	GAMEPLAY,
    POSCONSTRUCTOR,
    ESCMENU
};

const int boardSize = 8;
const int squareSize = 80;
const int screenWidth = boardSize * squareSize;
const int screenHeight = boardSize * squareSize;
const char* title = "Chess_sim";
bool escPressedHandled = false;
int main()//f2 f3 e7 e5 g2 g4 d8 h4
{
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    //SetConfigFlags(FLAG_WINDOW_UNDECORATED);
    InitWindow(screenWidth, screenHeight, title);
    SetExitKey(0);
    SetTargetFPS(60);
    
    

    Timer timer;
    GameState gameState = MAIN_MENU;
    GameState tempState = gameState;
    MainMenu mainMenu(screenWidth, screenHeight);
    //SettingsMenu settingsMenu(screenWidth, screenHeight);
    Position GamePosition("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");

    Position ConstructPosition("8/8/8/8/8/8/8/8 w KQkq - 0 1");
    //Position position("1ppp4/Ppkp4/1ppp4/8/8/8/8/7K", Position::NONE, true, true, true, true, 0.0f);
    //Position position("k7/8/8/8/8/q7/8/K7", Position::NONE, true, true, true, true, 0.0f);
    
    

	SIDE wonSide = SIDE::None;
    Game game(GamePosition, SIDE::White);
    PosConstructor posConstructor(ConstructPosition);
    ESCMenu escMenu(screenWidth,screenHeight);
    MenuResult result;
    Theme theme;
    std::string path = escMenu.getFilePath();
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
            if (result == MenuResult::StartGame)
            {
                gameState = GAMEPLAY;
                game.resetPosition();
                game.setAiSideToPlay(mainMenu.getSideToPlay());
                theme = mainMenu.getTheme();
                game.setTheme(theme);
            }
            if (result == MenuResult::OpenSettings)
                gameState = SETTINGS;
            if (result == MenuResult::ConstructPosition)
            {
                gameState = POSCONSTRUCTOR;

                //SetWindowSize(840, 600);
            }
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
            posConstructor.update();
            result = posConstructor.getResult();
            break;
        case ESCMENU:
            escMenu.DrawMenu();
            escMenu.Update();
            switch (escMenu.getResult())
            {
            case ESCResult::Resume:
                gameState = tempState;
                escMenu.resetResult();
                break;
            case ESCResult::ToMainMenu:
                gameState = GameState::MAIN_MENU;
                tempState = gameState;
                escMenu.resetResult();
                break;
            
            case ESCResult::Save:
                if (tempState == POSCONSTRUCTOR)
                {
                    if (posConstructor.getKingCounter() < 2)
                    {
                        std::cout << "You need at least 2 kings on the board\n";
                        gameState = tempState;
                        escMenu.resetResult();
                        break;
                    }
                    path = escMenu.getFilePath();
                    if (!path.empty())
                    {
                        posConstructor.getPosition().save(path.c_str());
                        std::cout << "Game saved to " << path << std::endl;
                    }
                    escMenu.resetResult();
                }
                else if (tempState == GAMEPLAY)
                {
                    path = escMenu.getFilePath();
                    if (!path.empty())
                    {
                        game.getPosition().save(path.c_str());
                        std::cout << "Game saved to " << path << std::endl;
                    }
                    escMenu.resetResult();
                }
                
                break;

            case ESCResult::Load:
                path = escMenu.getFilePath();
                if (!path.empty())
                {
                    Position newPos;
                    newPos = Position::load(path.c_str());
                    game.setPosition(newPos);
                    std::cout << "Game loaded from " << path << std::endl;
                }
                escMenu.resetResult();
                break;

            }
        }
        if (!IsKeyPressed(KEY_ESCAPE)) {
            escPressedHandled = false; 
        }
        if (IsKeyPressed(KEY_ESCAPE) && gameState != GameState::MAIN_MENU
           && gameState != GameState::ESCMENU) {
            tempState = gameState;
            mainMenu.resetResult();
            gameState = ESCMENU;
            escPressedHandled = true;
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
