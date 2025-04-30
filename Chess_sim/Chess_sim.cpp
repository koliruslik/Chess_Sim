// Chess_sim.cpp: определяет точку входа для приложения.
//
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
#include "StandartMovingNotation.hpp"
#include "PsLegalMoveMaskGen.hpp"
#include "Game.hpp"

using namespace std;


enum GameState
{
	MAIN_MENU,
	SETTINGS,
	GAMEPLAY
};

void printParentDirectory() {
    char cwd[1024];

    // Получаем текущую рабочую директорию
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        std::cout << "Current working directory: " << cwd << std::endl;

        // Добавляем "/.." для перехода в родительскую папку
        strcat(cwd, "/..");

        // Выводим путь родительской папки
        std::cout << "Parent directory: " << cwd << std::endl;

        // Теперь путь будет указывать на родительскую директорию
        // Конкатенируем с папкой resources
        strcat(cwd, "/resources");

        std::cout << "Resources directory: " << cwd << std::endl;
    }
    else {
        std::cerr << "Error getting current working directory." << std::endl;
    }
}

int main()//f2 f3 e7 e5 g2 g4 d8 h4
{
    printParentDirectory();
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
    Button startGameButton(screenWidth / 2, screenHeight / 2 - 25, 150, 50, "Start Game");
    Button exitGameButton(screenWidth / 2, screenHeight / 2 + 25, 150, 50, "Exit");

    Position position("rnbqkbnr/Pppppppp/8/8/8/8/pPPPPPPP/RNBQKBNR", Position::NONE, true, true, true, true, 0.0f);
    //Position position("k7/8/8/8/8/q7/8/K7", Position::NONE, true, true, true, true, 0.0f);
    Game game(position);
    
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

            if (mainMenu.shouldExitGame())
                CloseWindow();
            else if (mainMenu.shouldStartGame())
                gameState = GAMEPLAY;
            else if (mainMenu.isSettingsMenu())
                gameState = SETTINGS;

            break;

        case SETTINGS:
            DrawText("SETTINGS", screenWidth / 2 - MeasureText("SETTINGS", 20), 0, 20, WHITE);
            break;

        case GAMEPLAY:
            game.processGame();

            // Отрисовка доски
            
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
