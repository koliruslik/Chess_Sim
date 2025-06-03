#include "AI.hpp"

#define nsecs std::chrono::high_resolution_clock::now().time_since_epoch().count()

static std::atomic<bool> stopSearch;

static int64_t evaluated;
static int32_t maximalDepth;
static int32_t tt_cutoffs;

AI::AI() = default;
AI::AI(const std::string& openingBookPath)
	: openingBook(openingBookPath)
{
	std::cout << "Opening book loaded." << std::endl;
}


Move AI::proccessBestMove(const Position& position, uint8_t side, int32_t minMs, int32_t maxMs, bool debugMode)
{
	if(debugMode) std::cout <<"ai move generation started" << std::endl;
	int32_t evaluate = StaticEvaluator::evaluate(
		position.getPieces(),
		position.getWLCastling(), position.getWSCastling(),
		position.getBLCastling(), position.getBSCastling(),
		position.getWhiteCastlingHappened(), position.getBlackCastlingHappened()
	);

	int64_t timeStart = nsecs;
	stopSearch = false;
	TranspositionTable tt;

	std::tuple<Move, int32_t> openingBookResult = this->openingBook.tryToFindMove(position);
	if (debugMode) std::cout  << "Number of available moves in the opening book: " << std::get<1>(openingBookResult) << std::endl;
	if (std::get<1>(openingBookResult) != 0)
	{
		if (debugMode) std::cout << "Book move selected"<< std::endl;
		int64_t elapsedMs = (nsecs - timeStart) / (int64_t)1e+6;
		int64_t sleepTimeMs = std::max((int64_t)0, minMs - elapsedMs);
		std::this_thread::sleep_for(std::chrono::milliseconds(sleepTimeMs));
		return std::get<0>(openingBookResult);
	}

	if (debugMode) std::cout << "Search started. " << std::endl;

	int32_t bestMoveEvaluation = 0;
	Move bestMove;
	std::future<std::tuple<int32_t, Move>> bestMoveThread;

	bool updateBestMove;

	for (int32_t i = 1; i < 1e+3 && !stopSearch; ++i)
	{
		evaluated = 0;
		maximalDepth = 0;
		tt_cutoffs = 0;

		auto bestMoveThread = std::async(std::launch::async, AI::bestMove, position, side, i, std::ref(tt));

		
		updateBestMove = true;

		while (bestMoveThread.wait_for(std::chrono::seconds(0)) != std::future_status::ready)
		{
			int64_t elapsedMs = (nsecs - timeStart) / (int64_t)1e+6;
			//std::cout << "Elapsed time: " << elapsedMs << "ms, maxMs: " << maxMs << ", stopSearch: " << stopSearch << std::endl;
			if (elapsedMs >= maxMs || stopSearch) 
			{
				updateBestMove = false;
				break;
			}

			std::this_thread::sleep_for(std::chrono::milliseconds(20));
		}

		if (updateBestMove || i == 1) {
			try {
				std::tie(bestMoveEvaluation, bestMove) = bestMoveThread.get();
			}
			catch (const std::exception& e) {
				std::cerr << "Exception during AI::bestMove: " << e.what() << std::endl;
				stopSearch = true;
				break;
			}
		}
		else if (!updateBestMove) {
			if (debugMode) std::cout << "Search stopped." << std::endl;
			stopSearch = true;
		}

		if (!debugMode) std::cout << "Base depth: " << std::setw(4) << i
			<< ". Maximal depth: " << std::setw(4) << maximalDepth
			<< ". Evaluated: " << std::setw(6) << float(bestMoveEvaluation) / 100.0f << " pawns. "
			<< "Evaluated(this iteration): " << std::setw(10) << evaluated
			<< ". TT cutoffs (this iteration): " << std::setw(10)
			<< (nsecs - timeStart) / (int64_t)1e+6 << "ms." << std::endl;

		if (bestMoveEvaluation > AI::Infinity::Positive - 2000
			|| bestMoveEvaluation < AI::Infinity::Negative + 2000)
		{
			break;
		}

		if (debugMode) std::cout << "Debug" << std::endl;
	}
	int64_t elapsedMs = (nsecs - timeStart) / (int64_t)1e+6;
	int64_t sleepTimeMs = std::max((int64_t)0, minMs - elapsedMs);
	std::this_thread::sleep_for(std::chrono::milliseconds(sleepTimeMs));

	if (debugMode) std::cout << "Search finished." << std::endl;
	
	return bestMove;
}

std::tuple<int32_t, Move> AI::bestMove(const Position& position, uint8_t side, int32_t depth, TranspositionTable& tt)
{
	if (side == SIDE::White) return AI::alphaBetaMax(position, AI::Infinity::Negative, AI::Infinity::Positive, depth, 0, tt);
	return AI::alphaBetaMin(position, AI::Infinity::Negative, AI::Infinity::Positive, depth, 0, tt);
}

std::tuple<int32_t, Move> AI::alphaBetaMin(Position position, int32_t alpha, int32_t beta, int32_t depth_left, int32_t depth_current, TranspositionTable& tt) {
	if (stopSearch) return std::make_tuple(0, Move());
	if (depth_current > maximalDepth) maximalDepth = depth_current;

	if (depth_left == 0) return std::make_tuple(AI::alphaBetaMinOnlyCaptures(position, alpha, beta, depth_current), Move());

	if (position.getFiftyMovesCtr() >= 50 or position.getRepetitionHistory().getRepetitionNumber(position.getHash()) >= 3) return std::make_tuple(0, Move());

	MoveList moves = LegalMoveGen::generate(position, SIDE::Black);
	moves = MoveSorter::sort(position.getPieces(), moves);
	Move move;
	Move best_move;
	uint8_t best_move_index = 0;

	bool in_check = PsLegalMoveMaskGen::inDanger(position.getPieces(), BOp::bsf(position.getPieces().getPieceBitboard(SIDE:: Black,PIECE::KING)), SIDE::Black);

	if (moves.getSize() == 0) {
		if (in_check) return std::make_tuple(AI::Infinity::Positive - depth_current, Move());
		return std::make_tuple(0, Move());
	}

	int32_t evaluation;

	Position copy;

	uint8_t tt_result = tt.tryToFindBestMoveIndex(position.getHash());

	for (uint8_t i = 0; i < moves.getSize(); i = i + 1) {
		if (tt_result >= moves.getSize()) move = moves[i];
		else {
			if (i == 0) move = moves[tt_result];
			else {
				if (i == tt_result) move = moves[0];
				else move = moves[i];
			}
		}

		copy = position;
		copy.move(move);
		evaluation = std::get<0>(AI::alphaBetaMax(copy, alpha, beta, depth_left - !in_check, depth_current + 1, tt));

		if (evaluation <= alpha) {
			if (tt_result >= moves.getSize() or i != 0) tt.addEntry({ position.getHash(), depth_left, best_move_index});
			else tt_cutoffs = tt_cutoffs + 1;
			return std::make_tuple(alpha, best_move);
		}
		if (evaluation < beta) {
			best_move = move;
			best_move_index = i;
			beta = evaluation;
		}
	}

	tt.addEntry({ position.getHash(), depth_left, best_move_index});
	return std::make_tuple(beta, best_move);
}

std::tuple<int32_t, Move> AI::alphaBetaMax(Position position, int32_t alpha, int32_t beta, int32_t depth_left, int32_t depth_current, TranspositionTable& tt) {
	if (stopSearch) return std::make_tuple(0, Move());
	if (depth_current > maximalDepth) maximalDepth = depth_current;

	if (depth_left == 0) return std::make_tuple(AI::alphaBetaMaxOnlyCaptures(position, alpha, beta, depth_current), Move());

	if (position.getFiftyMovesCtr() >= 50 or position.getRepetitionHistory().getRepetitionNumber(position.getHash()) >= 3) return std::make_tuple(0, Move());

	MoveList moves = LegalMoveGen::generate(position, SIDE::White);
	moves = MoveSorter::sort(position.getPieces(), moves);
	Move move;
	Move best_move;
	uint8_t best_move_index = 0;

	bool in_check = PsLegalMoveMaskGen::inDanger(position.getPieces(), BOp::bsf(position.getPieces().getPieceBitboard(SIDE::White,PIECE::KING)), SIDE::White);

	if (moves.getSize() == 0) {
		if (in_check) return std::make_tuple(AI::Infinity::Negative + depth_current, Move());
		return std::make_tuple(0, Move());
	}

	int32_t evaluation;

	Position copy;

	uint8_t tt_result = tt.tryToFindBestMoveIndex(position.getHash());

	for (uint8_t i = 0; i < moves.getSize(); i = i + 1) {
		if (tt_result >= moves.getSize()) move = moves[i];
		else {
			if (i == 0) move = moves[tt_result];
			else {
				if (i == tt_result) move = moves[0];
				else move = moves[i];
			}
		}

		copy = position;
		copy.move(move);
		evaluation = std::get<0>(AI::alphaBetaMin(copy, alpha, beta, depth_left - !in_check, depth_current + 1, tt));

		if (evaluation >= beta) {
			if (tt_result >= moves.getSize() or i != 0) tt.addEntry({ position.getHash(), depth_left, best_move_index});
			else tt_cutoffs = tt_cutoffs + 1;
			return std::make_tuple(beta, best_move);
		}
		if (evaluation > alpha) {
			best_move = move;
			best_move_index = i;
			alpha = evaluation;
		}
	}

	tt.addEntry({ position.getHash(), depth_left, best_move_index});
	return std::make_tuple(alpha, best_move);
}
int32_t AI::alphaBetaMinOnlyCaptures(const Position& position, int32_t alpha, int32_t beta, int32_t depth_current) {
	if (stopSearch) return 0;
	if (depth_current > maximalDepth) maximalDepth = depth_current;

	int32_t evaluation = StaticEvaluator::evaluate(position.getPieces(), position.getWLCastling(), position.getWSCastling(), position.getBLCastling(), position.getBSCastling(), position.getWhiteCastlingHappened(), position.getBlackCastlingHappened());
	evaluated = evaluated + 1;

	if (evaluation <= alpha) return alpha;
	if (evaluation < beta) beta = evaluation;

	MoveList moves = LegalMoveGen::generate(position, SIDE::Black, true);
	moves = MoveSorter::sort(position.getPieces(), moves);
	Move move;

	Position copy;

	for (uint8_t i = 0; i < moves.getSize(); i = i + 1) {
		move = moves[i];

		copy = position;
		copy.move(move);
		evaluation = AI::alphaBetaMaxOnlyCaptures(copy, alpha, beta, depth_current + 1);

		if (evaluation <= alpha) return alpha;
		if (evaluation < beta) beta = evaluation;
	}

	return beta;
}
int32_t AI::alphaBetaMaxOnlyCaptures(const Position& position, int32_t alpha, int32_t beta, int32_t depth_current) {
	if (stopSearch) return 0;
	if (depth_current > maximalDepth) maximalDepth = depth_current;

	int32_t evaluation = StaticEvaluator::evaluate(position.getPieces(), position.getWLCastling(), position.getWSCastling(), position.getBLCastling(), position.getBSCastling(), position.getWhiteCastlingHappened(), position.getBlackCastlingHappened());
	evaluated = evaluated + 1;

	if (evaluation >= beta) return beta;
	if (evaluation > alpha) alpha = evaluation;

	MoveList moves = LegalMoveGen::generate(position, SIDE::White, true);
	moves = MoveSorter::sort(position.getPieces(), moves);
	Move move;

	Position copy;

	for (uint8_t i = 0; i < moves.getSize(); i = i + 1) {
		move = moves[i];

		copy = position;
		copy.move(move);
		evaluation = AI::alphaBetaMinOnlyCaptures(copy, alpha, beta, depth_current + 1);

		if (evaluation >= beta) return beta;
		if (evaluation > alpha) alpha = evaluation;
	}

	return alpha;
}
