#include "raylib.h"
#include "Chess_sim.h"
#include <bitset> 
#include <iostream>

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



const int boardSize = 8;
const int squareSize = 80;
const int screenWidth = boardSize * squareSize;
const int screenHeight = boardSize * squareSize;
const char* title = "Chess_sim";
bool escPressedHandled = false;
const std::string savePath = "saves/save.txt";
SIDE aiSide = SIDE::None;
void insertTextFromFile(const std::string& targetPath, const std::string& sourcePath)
{
    std::ifstream targetFile(targetPath);
    std::ifstream sourceFile(sourcePath);

    if (!targetFile.is_open() || !sourceFile.is_open()) {
        std::cerr << "Failed to open file(s)." << std::endl;
        return;
    }

    std::vector<std::string> targetLines;
    std::string line;
    while (std::getline(targetFile, line)) {
        targetLines.push_back(line);
    }
    targetFile.close();

    std::vector<std::string> sourceLines;
    while (std::getline(sourceFile, line)) {
        sourceLines.push_back(line);
    }
    sourceFile.close();

    size_t insertIndex = (targetLines.size() >= 2) ? 2 : targetLines.size();
    targetLines.insert(targetLines.begin() + insertIndex, sourceLines.begin(), sourceLines.end());

    std::ofstream outFile(targetPath);
    for (const auto& l : targetLines) {
        outFile << l << "\n";
    }
    outFile.close();
}

int main()//f2 f3 e7 e5 g2 g4 d8 h4
{

    //SetConfigFlags(FLAG_WINDOW_UNDECORATED);
    InitWindow(screenWidth, screenHeight, title);
    SetExitKey(0);
    SetTargetFPS(60);
    
    

    Timer timer;
    std::shared_ptr<BoardRenderer> renderer = std::make_shared<BoardRenderer>(80); 
    MainMenu mainMenu(screenWidth, screenHeight, renderer);
    ESCMenu escMenu(screenWidth, screenHeight, renderer);
    MenuResult result;
    GameState gameState = MAIN_MENU;
    GameState tempState = gameState;
    //SettingsMenu settingsMenu(screenWidth, screenHeight);

    Position GamePosition("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");

    Position ConstructPosition("8/8/8/8/8/8/8/8 w KQkq - 0 1");
    //Position position("1ppp4/Ppkp4/1ppp4/8/8/8/8/7K", Position::NONE, true, true, true, true, 0.0f);
    //Position position("k7/8/8/8/8/q7/8/K7", Position::NONE, true, true, true, true, 0.0f);
    
    

	SIDE wonSide = SIDE::None;
    Game game(GamePosition, SIDE::White, renderer);
    PosConstructor posConstructor(ConstructPosition,renderer);
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
            
            switch (result)
            {
            case MenuResult::Exit:
                CloseWindow();
                break;

            case MenuResult::StartGame:
                gameState = GAMEPLAY;
                game.resetPosition();
                aiSide = mainMenu.getSideToPlay();
                game.setAiSideToPlay(aiSide);
                theme = mainMenu.getTheme();
                game.setTheme(theme);
                break;

            case MenuResult::OpenSettings:
                gameState = SETTINGS;
                break;

            case MenuResult::ConstructPosition:
                gameState = POSCONSTRUCTOR;
                theme = mainMenu.getTheme();
                posConstructor.setTheme(theme);
                break;

            default:
                break;
            }
            break;
        case GAMEPLAY:

            game.processGame();
            wonSide = game.getWonSide();
            if (wonSide == SIDE::Incorrect)
            {
                DrawText("Invalid position:", screenWidth / 2 - MeasureText("Invalid position:", 50)/2, screenHeight / 2, 50, BLACK);
                DrawText("both players are in checkmate", screenWidth / 2 - MeasureText("both players are in checkmate", 30)/2, screenHeight / 2 + 50, 30, BLACK);
            }
            if(wonSide == SIDE::White) DrawText("White wins!", screenWidth / 2 - MeasureText("White wins!", 50)/2, screenHeight / 2, 50, BLACK);
			else if (wonSide == SIDE::Black) DrawText("Black wins!", screenWidth / 2 - MeasureText("Black wins!", 50) / 2, screenHeight / 2, 50, BLACK);
			else if (wonSide == SIDE::Stalemate) DrawText("Stalemate!", screenWidth / 2 - MeasureText("Stalemate!", 50) / 2, screenHeight / 2, 50, BLACK);
			else if (wonSide == SIDE::Draw) DrawText("Draw!", screenWidth / 2 - MeasureText("Draw!", 50)/2, screenHeight / 2, 50, BLACK);

            break;
        case POSCONSTRUCTOR:
            renderer->resetHighLight();
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
                        renderer->displayWarning("You need at least 2 kings on the board",2.0f);
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
                        insertTextFromFile(path, savePath);
                        std::cout << "Game saved to " << path << std::endl;
						renderer->displayWarning("Game saved to " + path, 2.0f);
                    }
                    else
                    {
                        renderer->displayWarning("Select saving file");
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
                    int wTime = newPos.getWhiteTime();
					int bTime = newPos.getBlackTime();
                    std::cout << wTime << " " << bTime << "LOADING \n";
                    game.setPosition(newPos);
                    game.setAiSideToPlay(aiSide);
                    if (wTime > 0 && bTime > 0)
                    {
                        game.setTime(wTime, bTime);
                    }

                    std::cout << "Game loaded from " << path << std::endl;
					renderer->displayWarning("Game loaded from " + path, 2.0f);
                }
                else
                {
                    renderer->displayWarning("Select loading file");
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
        renderer->drawWarning();
        EndDrawing();
    }
    Init::unloadAllTextures();
    CloseWindow();
    return 0;
}



