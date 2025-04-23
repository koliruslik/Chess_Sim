
#include "MoveList.hpp"
#include "PsLegalMoveMaskGen.hpp"


#pragma once


class LegalMoveGen {
public:
    static MoveList generate(const Position &position, uint8_t side, bool onlyCaptures = false);
    static bool isLegal(Pieces pieces, Move move);

    static void pieceMaskToMoves(Pieces pieces, Bitboard mask, uint8_t attackerP, uint8_t attackerType, uint8_t attackerSide, MoveList &moves);
    static void pawnsMaskToMoves(Pieces pieces, Bitboard mask, uint8_t attackerSide, int8_t attackerIndex, bool checkDefender, uint8_t flag, MoveList &moves);
    
    static void addEnPassantCaptures(Pieces pieces, uint8_t side, uint8_t enPassant, MoveList &moves);
    static void addCastlingMoves(Pieces pieces, uint8_t side, bool lCastling, bool sCastling, MoveList &moves);

    static MoveList generateForSquare(const Position& position, uint8_t side, uint8_t square, bool onlyCaptures = false);
private:
    static void generatePawnMoves(const Position& position, uint8_t side, uint8_t squareIndex, bool onlyCaptures, MoveList& moves);
    static void generateKnightMoves(const Position& position, uint8_t side, uint8_t squareIndex, bool onlyCaptures, MoveList& moves);
    static void generateBishopMoves(const Position& position, uint8_t side, uint8_t squareIndex, bool onlyCaptures, MoveList& moves);
    static void generateRookMoves(const Position& position, uint8_t side, uint8_t squareIndex, bool onlyCaptures, MoveList& moves);
    static void generateQueenMoves(const Position& position, uint8_t side, uint8_t squareIndex, bool onlyCaptures, MoveList& moves);
    static void generateKingMoves(const Position& position, uint8_t side, uint8_t squareIndex, bool onlyCaptures, MoveList& moves);
};