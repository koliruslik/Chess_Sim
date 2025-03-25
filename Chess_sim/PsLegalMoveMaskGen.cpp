#include "PsLegalMoveMaskGen.h"

BitBoard PsLegalMoveMaskGen::generate_knight_mask(Pieces pieces, uint8_t p, uint8_t side, bool only_captures)
{
	if (only_captures)
	{
		return KingMasks::Masks[p] & pieces._side_bitboards[pieces.inverse(side)];
	}
	return KingMasks::Masks[p] & pieces._inversion_side_bitboards[side];
}

BitBoard PsLegalMoveMaskGen::generate_king_mask(Pieces pieces, uint8_t p, uint8_t side, bool only_captures)
{
	if (only_captures)
	{
		return KnightMasks::Masks[p] & pieces._side_bitboards[pieces.inverse(side)];
	}
	return KnightMasks::Masks[p] & pieces._inversion_side_bitboards[side];
}

BitBoard PsLegalMoveMaskGen::generate_bishop_mask(Pieces pieces, uint8_t p, uint8_t side, bool only_captures)
{
	BitBoard nw = PsLegalMoveMaskGen::_calc_ray(pieces, p, side, only_captures, SliderMasks::Direction::NorthWest, false);
	BitBoard ne = PsLegalMoveMaskGen::_calc_ray(pieces, p, side, only_captures, SliderMasks::Direction::NorthEast, false);
	BitBoard sw = PsLegalMoveMaskGen::_calc_ray(pieces, p, side, only_captures, SliderMasks::Direction::SouthWest, true);
	BitBoard se = PsLegalMoveMaskGen::_calc_ray(pieces, p, side, only_captures, SliderMasks::Direction::SouthEast, true);

	return nw | ne | sw | se;
}

BitBoard PsLegalMoveMaskGen::generate_rook_mask(Pieces pieces, uint8_t p, uint8_t side, bool only_captures)
{
	BitBoard n = PsLegalMoveMaskGen::_calc_ray(pieces, p, side, only_captures, SliderMasks::Direction::North, false);
	BitBoard s = PsLegalMoveMaskGen::_calc_ray(pieces, p, side, only_captures, SliderMasks::Direction::South, true);
	BitBoard w = PsLegalMoveMaskGen::_calc_ray(pieces, p, side, only_captures, SliderMasks::Direction::West, true);
	BitBoard e = PsLegalMoveMaskGen::_calc_ray(pieces, p, side, only_captures, SliderMasks::Direction::East, false);

	return n | s | w | e;
}

BitBoard PsLegalMoveMaskGen::generate_queen_mask(Pieces pieces, uint8_t p, uint8_t side, bool only_captures) {
	BitBoard bishop_mask = PsLegalMoveMaskGen::generate_bishop_mask(pieces, p, side, only_captures);
	BitBoard rook_mask = PsLegalMoveMaskGen::generate_rook_mask(pieces, p, side, only_captures);

	return bishop_mask | rook_mask;
}


BitBoard PsLegalMoveMaskGen::generate_pawn_default_mask(Pieces pieces, uint8_t side)
{
	if (side == Pieces::White)
	{
		return(pieces._piece_bitboards[Pieces::White][Pieces::Pawn] << 8) & pieces._empty;
	}
	return(pieces._piece_bitboards[Pieces::Black][Pieces::Pawn] >> 8) & pieces._empty;
}
BitBoard PsLegalMoveMaskGen::generate_pawn_long_mask(Pieces pieces, uint8_t side)
{
	BitBoard default_mask = PsLegalMoveMaskGen::generate_pawn_default_mask(pieces, side);

	if (side == Pieces::White)
	{
		return((default_mask & BitBoardRows::Rows[2]) << 8) & pieces._empty;
	}
	return((default_mask & BitBoardRows::Rows[5]) >> 8) & pieces._empty;
}
BitBoard PsLegalMoveMaskGen::generate_pawn_left_captures_mask(Pieces pieces, uint8_t side, bool include_all_possible_captures)
{
	if (side == Pieces::White)
	{
		BitBoard mask = (pieces._piece_bitboards[Pieces::White][Pieces::Pawn] << 7) & BitBoardColumns::InversionColumns[7];
		if (!include_all_possible_captures) mask = mask & pieces._side_bitboards[Pieces::Black];

		return mask;
	}

	BitBoard mask = (pieces._piece_bitboards[Pieces::Black][Pieces::Pawn] >> 9) & BitBoardColumns::InversionColumns[7];
	if (!include_all_possible_captures) mask = mask & pieces._side_bitboards[Pieces::White];
	return mask;
}
BitBoard PsLegalMoveMaskGen::generate_pawn_right_captures_mask(Pieces pieces, uint8_t side, bool include_all_possible_captures)
{
	if (side == Pieces::White) {
		BitBoard mask = (pieces._piece_bitboards[Pieces::White][Pieces::Pawn] << 9) & BitBoardColumns::InversionColumns[0];
		if (!include_all_possible_captures) mask = mask & pieces._side_bitboards[Pieces::Black];

		return mask;
	}

	BitBoard mask = (pieces._piece_bitboards[Pieces::Black][Pieces::Pawn] >> 7) & BitBoardColumns::InversionColumns[0];
	if (!include_all_possible_captures) mask = mask & pieces._side_bitboards[Pieces::White];
	return mask;
}

BitBoard PsLegalMoveMaskGen::_calc_ray(Pieces pieces, uint8_t p, uint8_t side, bool only_captures, int8_t direction, bool bsr)
{
	BitBoard blockers = SliderMasks::Masks[p][direction] & pieces._all;
	uint8_t blocking_square;

	if (blockers == 0)
	{
		if (only_captures) return 0;
		return SliderMasks::Masks[p][direction];
	}
	if (bsr) blocking_square = BitBoard::bsr(blockers);
	BitBoard moves;

	if (only_captures) moves = 0;
	else moves = SliderMasks::Masks[p][direction] ^ SliderMasks::Masks[blocking_square][direction];

	if (BitBoard::get_bit(pieces._side_bitboards[side], blocking_square)) BitBoard::set_0(moves, blocking_square);
	else BitBoard::set_1(moves, blocking_square);
	
	return moves;
}

bool PsLegalMoveMaskGen::is_under_attack(Pieces pieces, uint8_t p, uint8_t side)
{
	BitBoard opposite_pawns_left_captures = PsLegalMoveMaskGen::generate_pawn_left_captures_mask(pieces, pieces.inverse(side), true);
	BitBoard opposite_pawns_right_captures = PsLegalMoveMaskGen::generate_pawn_right_captures_mask(pieces, pieces.inverse(side), true);
	BitBoard opposite_pawns_captures = opposite_pawns_left_captures | opposite_pawns_right_captures;

	if (BitBoard::get_bit(opposite_pawns_captures, p)) return true;
	
	if (BitBoard::count_1((PsLegalMoveMaskGen::generate_knight_mask(pieces, p, side, true) & pieces._piece_bitboards[pieces.inverse(side)][Pieces::Knight])) > 0) return true;
	if (BitBoard::count_1((PsLegalMoveMaskGen::generate_bishop_mask(pieces, p, side, true) & pieces._piece_bitboards[pieces.inverse(side)][Pieces::Bishop])) > 0) return true;
	if (BitBoard::count_1((PsLegalMoveMaskGen::generate_rook_mask(pieces, p, side, true) & pieces._piece_bitboards[pieces.inverse(side)][Pieces::Rook])) > 0) return true;
	if (BitBoard::count_1((PsLegalMoveMaskGen::generate_queen_mask(pieces, p, side, true) & pieces._piece_bitboards[pieces.inverse(side)][Pieces::Queen])) > 0) return true;
	if (BitBoard::count_1((PsLegalMoveMaskGen::generate_king_mask(pieces, p, side, true) & pieces._piece_bitboards[pieces.inverse(side)][Pieces::King])) > 0) return true;
	return false;
}

namespace KingMasks
{
	std::array<BitBoard, 64> Masks = calc_masks();


	constexpr uint8_t abs_subtract(uint8_t left, uint8_t right)
	{
		return left > right ? left - right : right - left;
	}
	std::array<BitBoard, 64> calc_masks()
	{
		std::array<BitBoard, 64> masks{};

		uint8_t dx;
		uint8_t dy;
		
		for (uint8_t x0 = 0; x0 < 8; x0 = x0 + 1)
		{
			for (uint8_t y0 = 0; y0 < 8; y0 = y0 + 1)
			{
				for (uint8_t x1 = 0; x1 < 8; x1 = x1 + 1)
				{
					for (uint8_t y1 = 0; y1 < 8; y1 = y1 + 1)
					{
						dx = KingMasks::abs_subtract(x0, x1);
						dy = KingMasks::abs_subtract(y0, y1);

						if ((dx <= 2 && dy <= 1) || (dx == 1 && dy == 2)) BitBoard::set_1(masks[y0 * 8 + x0], y1 * 8 + x1);
					}
				}
			}
		}

		return masks;
	}
}

namespace KnightMasks
{
	std::array<BitBoard, 64> Masks = calc_masks();


	constexpr uint8_t abs_subtract(uint8_t left, uint8_t right)
	{
		return left > right ? left - right : right - left;
	}
	std::array<BitBoard, 64> calc_masks()
	{
		std::array<BitBoard, 64> masks{};

		uint8_t dx;
		uint8_t dy;

		for (uint8_t x0 = 0; x0 < 8; x0 = x0 + 1)
		{
			for (uint8_t y0 = 0; y0 < 8; y0 = y0 + 1)
			{
				for (uint8_t x1 = 0; x1 < 8; x1 = x1 + 1)
				{
					for (uint8_t y1 = 0; y1 < 8; y1 = y1 + 1)
					{
						dx = KnightMasks::abs_subtract(x0, x1);
						dy = KnightMasks::abs_subtract(y0, y1);

						if (dx <= 1 && dy <= 1) BitBoard::set_1(masks[y0 * 8 + x0], y1 * 8 + x1);
					}
				}
			}
		}

		return masks;
	}


}

namespace SliderMasks
{
	BitBoard calc_mask(uint8_t p, int8_t direction) 
	{
		BitBoard mask = 0;

		int8_t x = p % 8;
		int8_t y = p / 8;

		for (; ;)
		{
			switch (direction)
			{
				case Direction::North: y = y + 1; break;
				case Direction::South: y = y - 1; break;
				case Direction::West: x = x - 1; break;
				case Direction::East: x = x + 1; break;

				case Direction::NorthWest: y = y + 1; x = x - 1; break;
				case Direction::NorthEast: y = y + 1; x = x + 1; break;
				case Direction::SouthWest: y = y - 1; x = x - 1; break;
				case Direction::SouthEast: y = y - 1; x = x + 1; break;
			}

			if (x > 7 or x < 0 or y > 7 or y < 0) break;

			BitBoard::set_1(mask, y * 8 + x);
		}

		return mask;
	}

	std::array<std::array<BitBoard, 8>, 64> calc_masks()
	{
		std::array<std::array<BitBoard, 8>, 64> masks{};

		for (uint8_t i = 0; i < 64; i = i + 1) 
		{
			for (uint8_t j = 0; j < 8; j = j + 1) masks[i][j] = calc_mask(i, j);
		}

		return masks;
	}
	std::array<std::array<BitBoard, 8>, 64> Masks = calc_masks();
}