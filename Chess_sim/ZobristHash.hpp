
#include "Pieces.hpp"
#include "ZobristHashConstants.hpp"


#pragma once


class ZobristHash {
public:
    ZobristHash();
    ZobristHash(Pieces pieces, bool blackToMove, bool wlCastling, bool wsCastling, bool blCastling, bool bsCastling);

    friend bool operator ==(ZobristHash left, ZobristHash right);
	friend bool operator <(ZobristHash left, ZobristHash right) {
		return (left.value < right.value);
	}

    void invertPiece(uint8_t square, uint8_t type, uint8_t side);
    void invertMove();
    void invertWLCastling();
    void invertWSCastling();
    void invertBLCastling();
    void invertBSCastling();

    [[nodiscard]] uint64_t getValue() const;
private:
    uint64_t value;
};