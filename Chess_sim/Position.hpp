
#include <cmath>
#include <string>
#include "RepetitionHistory.hpp"
#include "Move.hpp"

#pragma once


class Position {
public:
    Position();
    Position(const std::string& shortFen, uint8_t enPassant, bool wlCastling, bool wsCastling, bool blCastling, bool bsCastling, float moveCtr);

    friend std::ostream& operator <<(std::ostream& ostream, const Position& position);

    void move(Move move);

    [[nodiscard]] Pieces getPieces() const;
    [[nodiscard]] uint8_t getEnPassant() const;
    [[nodiscard]] bool getWLCastling() const;
    [[nodiscard]] bool getWSCastling() const;
    [[nodiscard]] bool getBLCastling() const;
    [[nodiscard]] bool getBSCastling() const;
    [[nodiscard]] bool whiteToMove() const;
    [[nodiscard]] bool blackToMove() const;
    [[nodiscard]] ZobristHash getHash() const;
    [[nodiscard]] bool fiftyMovesRuleDraw() const;
    [[nodiscard]] bool threefoldRepetitionDraw() const;
	bool getWhiteCastlingHappened() const { return wlCastling + wsCastling; }
	bool getBlackCastlingHappened() const { return blCastling + bsCastling; }
	float getFiftyMovesCtr() const { return fiftyMovesCtr; }
	RepetitionHistory getRepetitionHistory() const { return repetitionHistory; }

    uint8_t getPieceTypeAt(uint8_t square, uint8_t side) const;
    uint8_t getPieceSideAt(uint8_t square) const;
    uint8_t getSideToMove();
	uint8_t getOpponentSide();
	float getMoveCtr() const { return moveCtr; }
    std::string toFEN() const;
    
    static constexpr uint8_t NONE = 255;
private:
    void addPiece(uint8_t square, uint8_t type, uint8_t side);
    void removePiece(uint8_t square, uint8_t type, uint8_t side);
    void changeEnPassant(uint8_t en_passant);

    void removeWLCastling();
    void removeWSCastling();
    void removeBLCastling();
    void removeBSCastling();

    void updateMoveCtr();
    void updateFiftyMovesCtr(bool breakEvent);

    Pieces pieces;
    uint8_t enPassant;

    bool wlCastling;
    bool wsCastling;
    bool blCastling;
    bool bsCastling;
    
    
    ZobristHash hash;
    float fiftyMovesCtr;
    RepetitionHistory repetitionHistory;

    float moveCtr;
};