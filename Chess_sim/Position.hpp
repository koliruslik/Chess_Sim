
#include <cmath>
#include <string>
#include "RepetitionHistory.hpp"
#include "Move.hpp"
#include "MoveList.hpp"
#include "Timer.h"
#pragma once


class Position {
public:
    Position();
    Position(const std::string& Fen);
    Position(const std::string& shortFen, uint8_t enPassant, bool wlCastling, bool wsCastling, bool blCastling, bool bsCastling, float moveCtr);
    friend std::ostream& operator <<(std::ostream& ostream, const Position& position);
    Position& operator=(const Position& other);

    void move(Move move);
    void moveList(MoveList moves);
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
    uint8_t getSideToMove() const;
	uint8_t getOpponentSide() const;
    uint8_t countPieces(uint8_t type, uint8_t side) const;
    int countPiecesTotal(uint8_t side) const;
	float getMoveCtr() const { return moveCtr; }
    std::string toFEN() const;
    void placePiece(uint8_t square, uint8_t type, uint8_t side);
    void deletePiece(uint8_t square, uint8_t type, uint8_t side);
    static constexpr uint8_t NONE = 255;
    void setTimers();
    void save(const std::string& filePath) const;
    static Position load(const std::string& filePath);

	void setTime(int wTime, int bTime) {
		this->wTime = wTime;
		this->bTime = bTime;
	}
	int getWhiteTime() const { return wTime; }
	int getBlackTime() const { return bTime; }
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
    
    int wTime;
	int bTime;
	const int defaultTime = 1800; // 30 minutes in seconds
};