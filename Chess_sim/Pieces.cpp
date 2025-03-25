#include "Pieces.h"
Pieces::Pieces() = default;
Pieces::Pieces(const std::string& short_fen)
{
    uint8_t x = 0;
	uint8_t y = 7;
    
    uint8_t side;

    for (auto buff : short_fen) {
        if (buff == '/')
        {
            x = 0;
            y = y - 1;
        }
        else if (std::isdigit(buff))
        {
            x = x + buff - '0';
        }
        else
        {
            if (std::isupper(buff))
            {
                buff = std::tolower(buff);
                side = Pieces::White;

            }
            else side = Pieces::Black;

            switch (buff) {
                case 'p': BitBoard::set_1(this->_piece_bitboards[side][Pieces::Pawn], y * 8 + x); break;
                case 'n': BitBoard::set_1(this->_piece_bitboards[side][Pieces::Knight], y * 8 + x); break;
                case 'b': BitBoard::set_1(this->_piece_bitboards[side][Pieces::Bishop], y * 8 + x); break;
                case 'r': BitBoard::set_1(this->_piece_bitboards[side][Pieces::Rook], y * 8 + x); break;
                case 'q': BitBoard::set_1(this->_piece_bitboards[side][Pieces::Queen], y * 8 + x); break;
                case 'k': BitBoard::set_1(this->_piece_bitboards[side][Pieces::King], y * 8 + x); break;
            }
            x = x + 1;
        }
    }
	this->uptade_bitboards();
}
uint8_t Pieces::inverse(uint8_t side) 
{
	return ~side;
}

void Pieces::uptade_bitboards()
{
    this->_side_bitboards[Pieces::White] = this->_piece_bitboards[Pieces::White][Pieces::Pawn] |
                                            this->_piece_bitboards[Pieces::White][Pieces::Knight] |
                                            this->_piece_bitboards[Pieces::White][Pieces::Bishop] |
                                            this->_piece_bitboards[Pieces::White][Pieces::Rook] |
                                            this->_piece_bitboards[Pieces::White][Pieces::Queen] |
                                            this->_piece_bitboards[Pieces::White][Pieces::King];

    this->_side_bitboards[Pieces::Black] = this->_piece_bitboards[Pieces::Black][Pieces::Pawn] |
                                            this->_piece_bitboards[Pieces::Black][Pieces::Knight] |
                                            this->_piece_bitboards[Pieces::Black][Pieces::Bishop] |
                                            this->_piece_bitboards[Pieces::Black][Pieces::Rook] |
                                            this->_piece_bitboards[Pieces::Black][Pieces::Queen] |
                                            this->_piece_bitboards[Pieces::Black][Pieces::King];

    this->_inversion_side_bitboards[Pieces::White] = ~this->_side_bitboards[Pieces::White];
    this->_inversion_side_bitboards[Pieces::Black] = ~this->_side_bitboards[Pieces::Black];

    this->_all = this->_side_bitboards[Pieces::White] | this->_side_bitboards[Pieces::Black];
    this->_empty = ~this->_all;
}

void Pieces::PrintPieces()
{
    const char* pieceSymbols[2][6] =
    {
        { "P", "N", "B", "R", "Q", "K" }, // Белые фигуры
        { "p", "n", "b", "r", "q", "k" }  // Черные фигуры
    };

    for (int8_t y = 7; y >= 0; --y)
    {
        for (uint8_t x = 0; x < 8; ++x)
        {
            std::cout << "|  ";
            bool pieceFound = false;

            for (uint8_t side = 0; side < 2; ++side) {
                for (uint8_t type = 0; type < 6; ++type) {
                    if (BitBoard::get_bit(this->_piece_bitboards[side][type], y * 8 + x)) {
                        std::cout << pieceSymbols[side][type];
                        pieceFound = true;
                        break;
                    }
                }
                if (pieceFound) break;
            }

            if (!pieceFound) std::cout << " ";
            std::cout << "  ";
        }
        std::cout << "|\n";
    }
}