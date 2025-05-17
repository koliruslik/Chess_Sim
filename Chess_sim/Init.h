#pragma once

#include <map>
#include <string>
#include <utility>

#include "raylib.h"
#include "MainMenu.h"
struct PieceTextures {
    std::map<std::string, Texture2D> white;
    std::map<std::string, Texture2D> black;
};

class Init {
public:
    static const PieceTextures& getPieceTextures(Theme theme);
    static const Texture2D& getPawnIcon(Theme theme, bool isWhite);
    static const Texture2D& getSquareWhite();
    static const Texture2D& getSquareBlack();
    static void unloadAllTextures();
private:
    Init() = delete; 
    static inline std::map<Theme, PieceTextures> cachedThemes;
    static inline Theme currentTheme = Theme::Theme1;
    static inline PieceTextures pieceTexturesT1;
    static inline PieceTextures pieceTexturesT2;
    static inline PieceTextures pieceTexturesT3;
    static inline Texture2D squareWhiteTex;
    static inline Texture2D squareBlackTex;
    static inline bool squaresLoaded = false;
    static inline bool texturesLoaded = false;
    static inline const std::string whitePiecesPath = "..\\..\\..\\..\\recources\\ChessPieces\\White\\";
    static inline const std::string blackPiecesPath = "..\\..\\..\\..\\recources\\ChessPieces\\Black\\";
    static inline const std::string squarePath = "..\\..\\..\\..\\recources\\boards\\";

    static inline const std::string names[6] = { "King", "Queen", "Rook", "Bishop", "Knight", "Pawn" };

    static PieceTextures loadPieceTextures(Theme theme);
    static void loadAllTextures();
    static void loadSquareTextures();
};