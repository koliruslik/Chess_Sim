#include "OpeningBook.h"

OpeningBook::OpeningBook() = default;


OpeningBook::OpeningBook(const std::string& path)
{
	std::ifstream file(path);
	if (!file.is_open())
	{
		std::cout << "---------------------Failed to open file: ---------------------" << path << std::endl;
		std::exit(255);
	}
	else
	{
		std::cout << "---------------------File opened---------------------" << path << std::endl;
	}

	std::string game;
	std::stringstream gameThread;

	std::string stringMove;
	uint8_t from;
	uint8_t to;

	MoveList possibleMoves;
	bool moveFound;

	Position buff;

	while (std::getline(file, game))
	{
		gameThread = std::stringstream(game);
		this->moves.resize(this->moves.size() + 1);

		buff = { "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR", 255, true, true, true, true, 1 };

		while (gameThread >> stringMove and gameThread.good())
		{
			from = (stringMove[1] - '1') * 8 + stringMove[0] - 'a';
			to = (stringMove[3] - '1') * 8 + stringMove[2] - 'a';
			//std::cout << "Move: " << stringMove << " From: " << (from + 0) << " To: " << (to + 0) << std::endl;
			possibleMoves = LegalMoveGen::generate(buff, buff.getMoveCtr() - std::floor(buff.getMoveCtr()) > 1e-7);
			moveFound = false;
			for (uint8_t i = 0; i < possibleMoves.getSize(); i++)
			{
				if (possibleMoves[i].getFrom() == from and possibleMoves[i].getTo() == to)
				{
					this->moves.back().push_back(possibleMoves[i]);
					buff.move(possibleMoves[i]);
					moveFound = true;
					break;
				}
			}
			if (!moveFound)
			{
				std::cout << "Invalid move in opening book: " << stringMove << std::endl;
				std::exit(255);
			}
		}
	}
	file.close();
}

//std::tuple<Move, int32_t> OpeningBook::tryToFindMove(const Position& position)
//{
//	Position buff;
//	std::vector<Move> possibleMoves;
//	bool moveExist;
//
//	for (int32_t game = 0; game < this->moves.size(); game = game + 1)
//	{
//		buff = { "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR", 255, true, true, true, true, 1 };
//
//		if (buff.getPieces() == position.getPieces())
//		{
//			moveExist = false;
//			for (auto addedMove : possibleMoves)
//			{
//				moveExist = true;
//				break;
//			}
//			if (!moveExist) possibleMoves.push_back(this->moves[game][0]);
//			continue;
//		}
//		
//		for (int32_t move = 0; move < this->moves.size() - 1; move = move + 1)
//		{
//			buff.move(this->moves[game][move]);
//			
//			if (buff.getPieces() == position.getPieces())
//			{
//				moveExist = false;
//				for (auto addedMove : possibleMoves)
//				{
//					moveExist = true;
//					break;
//				}
//			}
//			if (!moveExist) possibleMoves.push_back(this->moves[game][move + 1]);
//		}
//	}
//
//	if (possibleMoves.empty())
//	{
//		return std::make_tuple(Move(), 0);
//	}
//
//	return std::make_tuple(possibleMoves[time(nullptr) % possibleMoves.size()], possibleMoves.size());
//	
//}

std::tuple<Move, int32_t> OpeningBook::tryToFindMove(const Position& position)
{
	//std::cout << "=== Starting tryToFindMove() ===" << std::endl;
	Position buff;
	std::vector<Move> possibleMoves;

	int gameIndex = 0;
	for (const auto& gameMoves : this->moves)
	{
		//std::cout << "Processing game #" << gameIndex++ << ", move count: " << gameMoves.size() << std::endl;
		buff = { "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR", 255, true, true, true, true, 1 };
		
		if (buff.getPieces() == position.getPieces() && !gameMoves.empty())
		{
			
			//std::cout << "Match found at start position. First move: from "
				//<< +gameMoves[0].getFrom() << " to " << +gameMoves[0].getTo() << std::endl;

			const Move& firstMove = gameMoves[0];
			bool exists = std::any_of(possibleMoves.begin(), possibleMoves.end(), [&](const Move& m) {
				return m.getFrom() == firstMove.getFrom() && m.getTo() == firstMove.getTo();
				});

			if (!exists)
			{
				//std::cout << "Adding first move to possibleMoves" << std::endl;
				possibleMoves.push_back(firstMove);
			}
				

			continue;
		}

		for (size_t i = 0; i + 1 < gameMoves.size(); ++i)
		{
			buff.move(gameMoves[i]);
			//std::cout << "After move " << i << ": from " << +gameMoves[i].getFrom()
				//<< " to " << +gameMoves[i].getTo() << std::endl;
			if (buff.getPieces() == position.getPieces())
			{
				//std::cout << "Position matched at move " << i << ". Next move is from "
					//<< +gameMoves[i + 1].getFrom() << " to " << +gameMoves[i + 1].getTo() << std::endl;

				const Move& nextMove = gameMoves[i + 1];
				bool exists = std::any_of(possibleMoves.begin(), possibleMoves.end(), [&](const Move& m) {
					return m.getFrom() == nextMove.getFrom() && m.getTo() == nextMove.getTo();
					});

				if (!exists)
				{
					//std::cout << "Adding next move to possibleMoves" << std::endl;
					possibleMoves.push_back(nextMove);
				}
					

				break;
			}
		}
	}

	//std::cout << "Total found moves: " << possibleMoves.size() << std::endl;

	if (possibleMoves.empty())
	{
		//std::cout << "No valid moves found in opening book." << std::endl;
		return { Move(), 0 };
	}
	Move selectedMove = possibleMoves[time(nullptr) % possibleMoves.size()];

	return { selectedMove, static_cast<int32_t>(possibleMoves.size()) };
}