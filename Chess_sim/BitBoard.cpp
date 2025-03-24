#include "BitBoard.h"

void BitBoard::set_1(BitBoard& bb, uint8_t square)
{
	bb.board |= (1ULL << square);
}
void BitBoard::set_0(BitBoard& bb, uint8_t square)
{
	bb.board &= ~(1ULL << square);
}
bool BitBoard::get_bit(BitBoard bb, uint8_t square)
{
	return (bb.board & (1ull << square));
}
uint8_t BitBoard::count_1(BitBoard bb)
{
	uint64_t count = 0;
	while (bb.board) {
		count += bb.board & 1;
		bb.board >>= 1;
	}
	return static_cast<uint8_t>(count);
}
uint8_t BitBoard::bsf(BitBoard bb)
{
	return BitBoard::BitScanTable[((bb.board ^ (bb.board - 1)) * 0x03f79d71b4cb0a89) >> 58];
}
uint8_t BitBoard::bsr(BitBoard bb) 
{
	return std::countr_zero(bb.board);
}

namespace BitBoardRows
{
	 std::array<BitBoard, 8> calc_rows()
	{
		std::array<BitBoard, 8> rows{};
		for (uint8_t y = 0; y < 8; y = y + 1) {
			for (uint8_t x = 0; x < 8; x = x + 1)BitBoard::set_1(rows[y], y * 8 + x);
		}
		return rows;
	}

	 static  std::array<BitBoard, 8> Rows = BitBoardRows::calc_rows();

	 std::array<BitBoard, 8> calc_inversion_rows()
	{
		std::array<BitBoard, 8> inversion_rows{};
		for (uint8_t i = 0; i < 8; i = i + 1)inversion_rows[i] = ~Rows[i];
		return inversion_rows;
	}
	 static std::array<BitBoard, 8> InversionColumns = BitBoardColumns::calc_inversion_columns();
}

namespace BitBoardColumns
{
	 std::array<BitBoard, 8> calc_columns()
	{
		std::array<BitBoard, 8> columns{};
		for (uint8_t x = 0; x < 8; x = x + 1) {
			for (uint8_t y = 0; y < 8; y = y + 1)BitBoard::set_1(columns[x], y * 8 + x);
		}
		return columns;
	}
	 static std::array<BitBoard, 8> Columns = BitBoardColumns::calc_columns();

	 std::array<BitBoard, 8> calc_inversion_columns()
	{
		std::array<BitBoard, 8> inversion_columns{};
		for (uint8_t i = 0; i < 8; i = i + 1)inversion_columns[i] = ~Columns[i];
		return inversion_columns;
	}
	 static std::array<BitBoard, 8> InversionColumns = BitBoardColumns::calc_inversion_columns();
}