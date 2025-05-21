#pragma once
#include "raylib.h"
#include <string>
#include <map>
#include <cctype>
#include <chrono>
#include <thread>
#include <initializer_list>
#include "Position.hpp"
#include "Init.h"
#include "MoveList.hpp"
#include "LegalMoveGen.hpp"
#include "BTrans.h" 




class BoardRenderer {
public:
    BoardRenderer(int squareSize = 80);

    void setTheme(Theme theme);

    void drawBoard(const Position& position, uint8_t selectedSquare,
                  bool promotionOption, uint8_t promotionSquare,
                    uint8_t promotionSide) const;
    void drawBoard() const;
    void drawCoordinates() const;
    void drawMask(uint8_t selectedSquare, const Position& position) const;
    void drawBoardBlurredAndDarkened() const;
    void displayWarning(const std::string& text, float durationSeconds = 1.0f);
    void drawWarning() const;
    void drawInfo(int x, int y, int fontSize, std::initializer_list<std::string> lines);
    void drawPieceAtCursor(uint8_t type, uint8_t side, float scale);
    void drawPieceAtSquare(uint8_t type, uint8_t side, uint8_t square);
private:
    //BoardRenderer bRenderer(int squareSize = 80);
    void drawPieces(const Position& position) const;
    void drawPromotionOptions(uint8_t promotionSquare, uint8_t promotionSide) const;
    Vector2 centerPiece(float targetSize, float texWidth, float texHeight) const;

    static constexpr int boardSize = 8;
    int squareSize;

    std::vector<std::string> warningLines;
    std::atomic<bool> showWarning = false;
    float warningFontSize = 30.0f;
    std::chrono::steady_clock::time_point warningStart;
    float warningDuration = 1.0f;
    std::vector<std::string> wrapTextToLines(const std::string& text, int maxWidth, int fontSize) const;

    const Texture2D* squareWhiteTex = nullptr;
    const Texture2D* squareBlackTex = nullptr;

    std::map<Theme, const PieceTextures*> allPieceTextures;
    Theme theme;

    Color maskColor { 160, 112, 112, 136 };
    const Color promoteColor = GRAY;
    const Color whiteSquareColor = WHITE;
    const Color blackSquareColor = DARKGRAY;

    const std::string names[6] = { "King", "Queen", "Rook", "Bishop", "Knight", "Pawn" };
    std::vector<std::string> promotionNames = { "Queen", "Rook", "Bishop", "Knight" };

    const int constOffset = 5;
    int OffsetXForRows;
    int OffsetYForRows;
    int OffsetXForFiles;
    int OffsetYForFiles;
};