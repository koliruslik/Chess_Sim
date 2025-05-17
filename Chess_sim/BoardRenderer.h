#pragma once
#include "raylib.h"
#include <string>
#include <map>
#include <cctype>
#include "Position.hpp"
#include "Init.h"
#include "MoveList.hpp"
#include "LegalMoveGen.hpp"
#include "Btrans.h" 




class BoardRenderer {
public:
    BoardRenderer(int squareSize);

    void setTheme(Theme theme);

    void drawBoard(const Position& position, uint8_t selectedSquare,
                  bool promotionOption, uint8_t promotionSquare,
                    uint8_t promotionSide) const;
    void drawCoordinates() const;
    void drawMask(uint8_t selectedSquare, const Position& position) const;

private:
    void drawPieces(const Position& position) const;
    void drawPromotionOptions(uint8_t promotionSquare, uint8_t promotionSide) const;
    Vector2 centerPiece(float targetSize, float texWidth, float texHeight) const;

    int boardSize = 8;
    int squareSize;
    const Texture2D* squareWhiteTex = nullptr;
    const Texture2D* squareBlackTex = nullptr;
    std::map<Theme, const PieceTextures*> allPieceTextures;
    Theme theme;

    Color maskColor = { 160, 112, 112, 136 };
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