#include "Game.hpp"


Game::Game(Position position, SIDE aiSideToPlay, std::shared_ptr<BoardRenderer>& renderer)
	:ai("recources/Start/Start.txt"),
	WhiteTimer(std::make_shared<Timer>(std::chrono::seconds(1800))),
	BlackTimer(std::make_shared<Timer>(std::chrono::seconds(1800))),
	renderer(renderer)
{
	this->aiSide = aiSideToPlay;
	selectedSquare = 255;
	this->position = position;	
}

void Game::resetPosition()
{
	position = Position("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
	timersStarted = false;
	BlackTimer->Reset();
	WhiteTimer->Reset();
	
	movesHistory.clear();

	currentMove = Move();

	selectedSquare = 255;
	isSelected = false;

	promotionOption = false;
	promotionDone = false;
	promotionSquare = 255;
	pieceToPromote = "";

	wonSide = SIDE::None;

	aiSide = SIDE::None;
	aiThinking = false;
	aiMoveThread = std::thread();
	minMS = 0;
	maxMs = 1500;

	printMove = false;
	clickCount = 0;
	renderer->setHighLight(false);
	
	clearFile(savePath);
	
}

int Game::processMove(Position& position, MoveList& moves, uint8_t from, uint8_t to,uint8_t side)
{
	moves = LegalMoveGen::generate(position, side);
	bool validMove = false;

	for (int i = 0; i < moves.getSize(); ++i) {
		if (moves[i].getFrom() == from && moves[i].getTo() == to) {
			currentMove = moves[i];
			if (isPromotionMove(moves[i]))
			{
				promotionOption = true;
				promotionSquare = moves[i].getTo();
				promotionSide = moves[i].getAttackerSide();
				currentMove = moves[i];
				return 2;
			}
			position.move(moves[i]);
			movesHistory.push(moves[i]);
			timersStarted = true;
			printMove = true;
			validMove = true;
			break;
		}
		
	}

	if (!validMove) {
		if (debugMode) std::cout << "Invalid move.\n";
		return -1;
	}

	return 1;
}

int Game::processMoveWithClick()
{
	int col = mousePos.x / squareSize;
	int row = boardSize - (mousePos.y / squareSize);  
	int clickedSquare = row * 8 + col;

	MoveList moves = LegalMoveGen::generate(position, position.getPieceSideAt(selectedSquare), false);
	for (int i = 0; i < moves.getSize(); ++i)
	{
		const uint8_t from = moves[i].getFrom();
		const uint8_t to = moves[i].getTo();
		if (from == selectedSquare && to == clickedSquare)
		{
			currentMove = moves[i];
			processMove(position, moves, from, to, position.getSideToMove());
			selectedSquare = 255;
			isSelected = false;
			return 1;
		}
	}
	return -1;

}

int Game::proccesAiMove()
{
	Move aiMove = ai.proccessBestMove(position, aiSide, minMS, maxMs, debugMode);
	position.move(aiMove);
	currentMove = aiMove;
	movesHistory.push(aiMove);
	timersStarted = true;
	printMove = true;
	//std::cout << position;
	return 1;
}

void Game::proccessAiMoveAsync()
{
	aiThinking = true;
	aiMoveThread = std::thread([this]()
		{
			proccesAiMove();
			aiThinking = false;
		});
	aiMoveThread.detach();
}

void Game::processGame()
{
	update();
	drawBoard();
	return;
}

SIDE Game::checkVictory(const Position position)
{
	bool whiteInCheck = PsLegalMoveMaskGen::inDanger(position.getPieces(), BOp::bsf(position.getPieces().getPieceBitboard(SIDE::White, PIECE::KING)), SIDE::White);
	bool blackInCheck = PsLegalMoveMaskGen::inDanger(position.getPieces(), BOp::bsf(position.getPieces().getPieceBitboard(SIDE::Black, PIECE::KING)), SIDE::Black);

	MoveList whiteMoves = LegalMoveGen::generate(position, SIDE::White);
	MoveList blackMoves = LegalMoveGen::generate(position, SIDE::Black);

	bool whiteNoMoves = !whiteMoves.hasMoves();
	bool blackNoMoves = !blackMoves.hasMoves();
	bool fiftyMoves = position.fiftyMovesRuleDraw();
	bool threefold = position.threefoldRepetitionDraw();
	if ((whiteNoMoves && whiteInCheck) && (blackNoMoves && blackInCheck)) {
		return SIDE::Incorrect;
	}
	if ((whiteNoMoves && whiteInCheck) || (blackNoMoves && blackInCheck)) {
		return whiteNoMoves ? SIDE::Black : SIDE::White;
	}

	if (whiteNoMoves && !whiteInCheck && position.getSideToMove() == SIDE::White) {
		return SIDE::Stalemate;
	}
	else if (blackNoMoves && !blackInCheck && position.getSideToMove() == SIDE::Black)
	{
		return SIDE::Stalemate;
	}

	if ((whiteInCheck && !whiteNoMoves) || (blackInCheck && !blackNoMoves)) {
		return SIDE::Checked;
	}

	if (fiftyMoves || threefold) {
		return SIDE::Draw;
	}

	return SIDE::None;
}

std::string Game::generateAnnotation()
{
	std::string annotation;
	if (promotionDone)
	{
		if (std::string(1, pieceToPromote[0]) != "K") annotation += "=" + std::string(1, pieceToPromote[0]);
		else if (std::string(1, pieceToPromote[0]) == "K") annotation += "=N";
		promotionDone = false;
	}
	if (wonSide != SIDE::None && wonSide != SIDE::Checked)
	{
		annotation += "#";
		
	}
	if (wonSide == SIDE::Draw)
	{
		annotation += "1/2-1/2";
	}
	if (wonSide == SIDE::Checked)
	{
		annotation += "+";
	}
	if (wonSide == SIDE::Stalemate)
	{
		annotation += "0-1";
	}

	return annotation;
}
bool Game::PrintMove()
{
	float moveCtr = position.getMoveCtr();
	std::string annotation = generateAnnotation();

	std::cout << currentMove << annotation << "\t";
	if (getCurrentSideToMove(moveCtr) == SIDE::Black) std::cout << "\n";
	currentMove.ToFile(annotation, moveCtr, savePath);
	//movesHistory.readMovesFromFile(savePath);
	//movesHistory.saveToFile(annotation, moveCtr, loadPath);
	//movesHistory.printMoves();
	//std::cout << movesHistory[0] << annotation;
	return true;
}

void Game::setTheme(Theme newTheme)
{
	theme = newTheme;
}

void Game::drawBoard()
{
	renderer->setTheme(theme);
	renderer->drawBoard(position, selectedSquare,
						promotionOption, promotionSquare,
						promotionSide);
	renderer->drawTimer(BlackTimer, WhiteTimer);
}

std::string Game::handlePromotionSelection(int promotionIndex) {
	
	std::string selectedPiece = promotionNames[promotionIndex];
	if(debugMode) std::cout << "Selected piece for promotion: " << selectedPiece << std::endl;
	if (selectedPiece == "Rook") return "Rook";
	else if (selectedPiece == "Knight") return "Knight";
	else if (selectedPiece == "Bishop") return "Bishop";
	else return "Queen";
	selectedSquare = 0;
	promotionOption = false;

}

bool Game::isPromotionMove(Move& move)
{
	if (move.getAttackerType() != PIECE::PAWN)
		return false;
	uint8_t to = move.getTo();
	return (move.getAttackerSide() == SIDE::White && to >= 56) ||
		(move.getAttackerSide() == SIDE::Black && to <= 7);
}

void Game::proccesPromotionClick(Move& move)
{
	int col = mousePos.x / squareSize;
	int row = boardSize - (mousePos.y / squareSize);
	int optionRow = Btrans::indexToRowCol(promotionSquare).first;
	int optionCol = Btrans::indexToRowCol(promotionSquare).second;

	if (col == optionCol && (row <= 7 && row >= 4 && promotionSide == SIDE::White) || (row >= 0 && row <= 3 && promotionSide == SIDE::Black))
	{
		int i;
		if (promotionSide == SIDE::White) i = 7 - row;
		else i = row;
		//std::cout << i << std::endl;
		pieceToPromote = handlePromotionSelection(i);
		promotionDone = true;
	}
	else
	{
		isSelected = false;
		promotionOption = false;
		return;
	}
	handlePromotion(move);
	printMove = true;
}

void Game::handlePromotion(Move& move)
{
	if (isPromotionMove(move))
	{
		if (pieceToPromote == "Queen")
		{
			move.setFlag(Move::FLAG::PROMOTE_TO_QUEEN);
		}
		else if (pieceToPromote == "Rook")
		{
			move.setFlag(Move::FLAG::PROMOTE_TO_ROOK);
		}
		else if (pieceToPromote == "Bishop")
		{
			move.setFlag(Move::FLAG::PROMOTE_TO_BISHOP);
		}
		else if (pieceToPromote == "Knight")
		{
			move.setFlag(Move::FLAG::PROMOTE_TO_KNIGHT);
		}

	}
}



void Game::selectPiece()
{
	int col = mousePos.x / squareSize;
	int row = boardSize - (mousePos.y / squareSize);

	if (col >= 0 && col < boardSize && row >= 0 && row < boardSize)
	{
		std::string square = std::string(1, char('a' + col)) + std::string(1, char('1' + row));
		int8_t squareIndex = Btrans::squareToIndex(square);

		
		if (position.getPieceSideAt(squareIndex) == position.getSideToMove() && position.getPieceTypeAt(squareIndex, position.getSideToMove()) != Position::NONE)
		{
			
			if (selectedSquare == squareIndex)
			{
				selectedSquare = 255; 
				isSelected = false;  
			}
			else
			{
				selectedSquare = squareIndex;  
				isSelected = true;             
			}
		}
		else
		{
			
			if (debugMode) std::cout << "Invalid selection or opponent's piece.\n";
			if(!isSelected && debugMode)renderer->displayWarning("Invalid selection or opponent's piece.", 0.4f);
			selectedSquare = 255; 
			isSelected = false;  
		}
	}
	else
	{
		
		if (debugMode) std::cout << "Invalid selection.\n";
		if (debugMode) renderer->displayWarning("Invalid selection. Deselecting.", 0.4f);
		selectedSquare = 255;  
		isSelected = false;  
	}
}


void Game::update()
{
	wTime = WhiteTimer->GetRemainingSeconds();
	bTime = BlackTimer->GetRemainingSeconds();
	position.setTime(wTime, bTime);
	//std::cout << std::to_string(selectedSquare) << '\n';
	wonSide = checkVictory();
	if (WhiteTimer->GetRemainingSeconds() == 0) wonSide = SIDE::Black;
	if (BlackTimer->GetRemainingSeconds() == 0) wonSide = SIDE::White;
	if (printMove && debugMoves)
	{
		PrintMove();
		std::vector<uint8_t> squares = { currentMove.getFrom(), currentMove.getTo() };
		renderer->setHighLight(true, squares);
		printMove = false;
	}

	if (wonSide != SIDE::None && wonSide != SIDE::Checked && timersStarted)
	{
		WhiteTimer->Stop();
		BlackTimer->Stop();
		return;
	}
	uint8_t currentSide = position.getSideToMove();
	if (timersStarted)
	{
		if (currentSide == SIDE::White)
		{
			whiteTimerFlag = true;
			blackTimerFlag = false;
			BlackTimer->Stop();
			WhiteTimer->Start();
		}
		else if (currentSide == SIDE::Black)
		{
			whiteTimerFlag = false;
			blackTimerFlag = true;
			WhiteTimer->Stop();
			BlackTimer->Start();
		}
	}
	
	if (aiSide != position.getSideToMove())
	{
		if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
		{
			mousePos = GetMousePosition();
			if (promotionOption)
			{
				proccesPromotionClick(currentMove);
				if (promotionOption) position.move(currentMove);
				promotionOption = false;
				selectedSquare = 255;
				isSelected = false;
				return;
			}
			else if (isSelected)
			{
				if (processMoveWithClick() == 1) return;
				clickCount = 0;
				selectPiece();
				return;
			}
			else
			{
				selectPiece();
				return;
			}
		}
	}
	else if (aiSide == position.getSideToMove() && aiSide != SIDE::None)
	{
		if (aiThinking)
		{
			if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
			{
				if(debugMode) std::cout << "AI is thinking, please wait..." << std::endl;
			}
			return;
		}

		proccessAiMoveAsync();
	}

}

void Game::clearFile(const std::string filePath) {
	std::ofstream file(filePath, std::ios::trunc);
	if (!file.is_open()) {
		std::cerr << "Unable to open file for clearing: " << filePath << '\n';
	}
	else {
		std::cout << "File has been cleared.\n";
	}
}