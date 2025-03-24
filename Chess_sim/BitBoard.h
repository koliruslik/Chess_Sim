#pragma once

#ifndef BITBOARD_H
#define BITBOARD_H

#include <cstdint>
#include <bitset>
#include <iostream>
#include <array>
#include <bit>
class BitBoard
{
private:
	uint64_t board;

public:
	BitBoard() : board(0) {}

	static void set_1(BitBoard& bb, uint8_t square);
	static void set_0(BitBoard& bb, uint8_t square);
	static bool get_bit(BitBoard bb, uint8_t square);
	static uint8_t count_1(BitBoard bb);
	
    static uint8_t bsf(BitBoard bb);
	static uint8_t bsr(BitBoard bb);

	BitBoard operator~() const {
		BitBoard result;
		result.board = ~board;
		return result;
	}
	bool operator!=(const BitBoard& other) const {
		return board != other.board;
	}
	bool operator==(const BitBoard& other) const {
		return board == other.board;
	}
	BitBoard operator|(const BitBoard& other) const {
		BitBoard result;
		result.board = this->board | other.board;
		return result;
	}
	inline static const std::array<uint8_t, 64> BitScanTable = {
	0, 47, 1, 56, 48, 27, 2, 60,
	57, 49, 41, 37, 28, 16, 3, 61,
	54, 58, 35, 52, 50, 42, 21, 44,
	38, 32, 29, 23, 17, 11, 4, 62,
	46, 55, 26, 59, 40, 36, 15, 53,
	34, 51, 20, 43, 31, 22, 10, 45,
	25, 39, 14, 33, 19, 30, 9, 24,
	13, 18, 8, 12, 7, 6, 5, 63
	};
};

namespace BitBoardRows
{
    static  std::array<BitBoard, 8> calc_rows();
    static  std::array<BitBoard, 8> calc_inversion_rows();
}

namespace BitBoardColumns
{
	static std::array<BitBoard, 8> calc_columns();
	static std::array<BitBoard, 8> calc_inversion_columns();
}

#endif // BITBOARD_H	