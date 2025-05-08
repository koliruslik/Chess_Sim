#include "Game.hpp"

const Color maskColor = BLUE;
const Color promoteColor = GRAY;
const Color whiteSquareColor = WHITE;
const Color blackSquareColor = DARKGRAY;
Game::Game(Position position, SIDE aiSideToPlay)
	:ai("..\\..\\..\\..\\recources\\Start\\Start.txt")
{
	this->aiSide = aiSideToPlay;
	selectedSquare = -1;
	this->position = position;
	loadPieceTextures();
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
			selectedSquare = -1;
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

void Game::loadPieceTextures()
{
	for (const auto& name : names)
	{
		std::string whiteTexturePath = whitePiecesPath + name + ".png";
		std::string blackTexturePath = blackPiecesPath + name + ".png";

		//std::cout << "Loading white piece texture: " << whiteTexturePath << std::endl;
		Texture2D whiteTexture = LoadTexture(whiteTexturePath.c_str());
		if (whiteTexture.id == 0) {
			std::cerr << "Failed to load white piece texture: " << whiteTexturePath << std::endl;
		}
		else {
			whitePieces[name] = whiteTexture;
		}

		//std::cout << "Loading black piece texture: " << blackTexturePath << std::endl;
		Texture2D blackTexture = LoadTexture(blackTexturePath.c_str());
		if (blackTexture.id == 0) {
			std::cerr << "Failed to load black piece texture: " << blackTexturePath << std::endl;
		}
		else {
			blackPieces[name] = blackTexture;
		}
	}
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
	if ((whiteNoMoves && whiteInCheck) || (blackNoMoves && blackInCheck)) {
		return whiteNoMoves ? SIDE::Black : SIDE::White;
	}

	// Stalemate
	if ((whiteNoMoves && !whiteInCheck) || (blackNoMoves && !blackInCheck)) {
		return SIDE::Stalemate;
	}

	// Check (but not mate or stalemate)
	if ((whiteInCheck && !whiteNoMoves) || (blackInCheck && !blackNoMoves)) {
		return SIDE::Checked;
	}

	// Draw by rules
	if (fiftyMoves || threefold) {
		return SIDE::Draw;
	}

	return SIDE::None;
}

bool Game::PrintMove()
{
	float moveCtr = position.getMoveCtr();
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

	currentMove.Print(annotation, moveCtr);
	currentMove.ToFile(annotation, moveCtr, savePath);
	return true;
}

void Game::drawBoard()
{
	int rowIndex = 8;
	const char* files[8] = { "a", "b", "c", "d", "e", "f", "g", "h" };
	for (int row = 0; row < boardSize; ++row)
	{
		for (int col = 0; col < boardSize; ++col)
		{
			Color squareColor = ((row + col) % 2 == 0) ? whiteSquareColor : blackSquareColor;
			Color textColor = ((row + col) % 2 == 0) ? blackSquareColor : whiteSquareColor;
			DrawRectangle(col * squareSize, row * squareSize, squareSize, squareSize, squareColor);

			std::string rowIndexStr = std::to_string(rowIndex);
			std::string fileIndex = files[col];
			if (col == 7)
			{
				DrawText(rowIndexStr.c_str(), col * squareSize + OffsetXForRows, row * squareSize + OffsetYForRows, 20, textColor);
			}
			if (row == 7)
			{
				DrawText(fileIndex.c_str(), col * squareSize + OffsetXForFiles, row * squareSize + OffsetYForFiles, 20, textColor);
			}
		}
		rowIndex--;
	}
	
	drawPieces();
	drawMask();
	if (promotionOption) drawPromotionOptions();
}

void Game::drawPieces()
{
	std::string fen = position.toFEN();
	int row = 0, col = 0;

	float pieceSize = squareSize * 1.5f;

	for (char c : fen)
	{
		if (c == '/')
		{
			row++;
			col = 0;
		}
		else if (isdigit(c))
		{
			col += c - '0';
		}
		else
		{
			bool isWhite = isupper(c);
			std::string pieceType;

			switch (tolower(c))
			{
			case 'k': pieceType = "King"; break;
			case 'q': pieceType = "Queen"; break;
			case 'r': pieceType = "Rook"; break;
			case 'b': pieceType = "Bishop"; break;
			case 'n': pieceType = "Knight"; break;
			case 'p': pieceType = "Pawn"; break;
			}

			Texture2D tex = isWhite ? whitePieces[pieceType] : blackPieces[pieceType];

			float texAspectRatio = static_cast<float>(tex.width) / static_cast<float>(tex.height);

			float newWidth = pieceSize;
			float newHeight = pieceSize / texAspectRatio;

			if (newHeight > squareSize)
			{
				newHeight = squareSize;
				newWidth = squareSize * texAspectRatio;
			}
			Vector2 offset = centerPiece(pieceSize, tex.width, tex.height);
			float xOffset = offset.x;
			float yOffset = offset.y; 

			Rectangle source = { 0, 0, tex.width, tex.height }; 
			Rectangle dest = { col * squareSize + xOffset, row * squareSize + yOffset, newWidth, newHeight }; 

			Vector2 origin = { newWidth / 2.0f, newHeight / 2.0f };
			float rotation = 0.0f; 
			Color tint = WHITE; 

			DrawTexturePro(tex, source, dest, origin, rotation, tint);
			col++;
			
		}
	}
}

void Game::drawMask()
{
	if (selectedSquare == -1)
	{
		return;
	}
	//std::cout << "selected square: " << selectedSquare << std::endl;
	MoveList moves = LegalMoveGen::generate(position, position.getPieceSideAt(selectedSquare), false);
	std::vector<std::string> squares;
	for (int i = 0; i < moves.getSize(); ++i)
	{
		int from = moves[i].getFrom();
		if (from != selectedSquare || position.getPieceSideAt(from) != position.getSideToMove())
		{
			continue;
		}
		int to = moves[i].getTo();
		//std::cout << "to " << to << std::endl;
		int col = to % 8;
		int row = 7 - (to / 8); 
		//std::cout << "possible move: " << indexToSquare(to) << std::endl;
		Color maskColor = BLUE;
		DrawRectangle(col * squareSize, row * squareSize, squareSize, squareSize, maskColor);
	}

}
std::string Game::handlePromotionSelection(int promotionIndex) {
	/*if (promotionIndex < 0 || promotionIndex >= 4)
	{
		std::cout << "Invalid promotion selection.\n";
		return;
	}*/

	std::string selectedPiece = promotionPieces[promotionIndex];
	if(debugMode) std::cout << "Selected piece for promotion: " << selectedPiece << std::endl;
	if (selectedPiece == "Rook") return "Rook";
	else if (selectedPiece == "Knight") return "Knight";
	else if (selectedPiece == "Bishop") return "Bishop";
	else return "Queen";
	selectedSquare = -1;
	promotionOption = false;

}
void Game::drawPromotionOptions()
{
	if (!promotionOption) return;
	float pieceSize = squareSize * 1.5f;
	int col = Btrans::indexToRowCol(promotionSquare).second;
	int row = Btrans::indexToRowCol(promotionSquare).first;

	for (int i = 0; i < 4; ++i)
	{
		Texture2D tex = (promotionSide == SIDE::White) ? whitePieces[promotionPieces[i]] : blackPieces[promotionPieces[i]];
		float texAspectRatio = static_cast<float>(tex.width) / static_cast<float>(tex.height);

		float newWidth = pieceSize;
		float newHeight = pieceSize / texAspectRatio;

		if (newHeight > squareSize)
		{
			newHeight = squareSize;
			newWidth = squareSize * texAspectRatio;
		}
		Vector2 offset = centerPiece(pieceSize, tex.width, tex.height);
		float xOffset = offset.x;
		float yOffset = offset.y;

		Rectangle source = { 0, 0, tex.width, tex.height };
		Rectangle dest = { col * squareSize + xOffset, row * squareSize + yOffset, newWidth, newHeight };

		Vector2 origin = { newWidth / 2.0f, newHeight / 2.0f };
		float rotation = 0.0f;
		Color tint = WHITE;
		DrawRectangle(col * squareSize, row * squareSize, squareSize, squareSize, promoteColor);
		DrawTexturePro(tex, source, dest, origin, rotation, tint);
		if (row >= 0 && row <= 3) row++;
		else if (row <= 7 && row >= 4) row--;
	}
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

	if (col == optionCol && row <= 7 && row >= 0)
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

Vector2 Game::centerPiece(float pieceSize, float texWidth, float texHeight) const
{
	const float xOffset = squareSize/2;
	const float yOffset = squareSize/2 - constOffset; 

	return { xOffset, yOffset };
}



void Game::selectPiece()
{
	int col = mousePos.x / squareSize;
	int row = boardSize - (mousePos.y / squareSize);

	if (col >= 0 && col < boardSize && row >= 0 && row < boardSize)
	{
		std::string square = std::string(1, char('a' + col)) + std::string(1, char('1' + row));
		int squareIndex = Btrans::squareToIndex(square);

		
		if (position.getPieceSideAt(squareIndex) == position.getSideToMove() && position.getPieceTypeAt(squareIndex, position.getSideToMove()) != Position::NONE)
		{
			
			if (selectedSquare == squareIndex)
			{
				selectedSquare = -1; 
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
			
			if (debugMode) std::cout << "Invalid selection or opponent's piece. Deselecting.\n";
			selectedSquare = -1; 
			isSelected = false;  
		}
	}
	else
	{
		
		if (debugMode) std::cout << "Invalid selection. Deselecting.\n";
		selectedSquare = -1;  
		isSelected = false;  
	}
}

void Game::update()
{
	wonSide = checkVictory();
	if (printMove && debugMoves)
	{
		PrintMove();
		printMove = false;
	}
	if (wonSide != SIDE::None && wonSide != SIDE::Checked) return;
	
	if (aiSide != position.getSideToMove() && aiSide)
	{
		if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
		{
			mousePos = GetMousePosition();
			if (promotionOption)
			{
				proccesPromotionClick(currentMove);
				if (promotionOption) position.move(currentMove);
				promotionOption = false;
				selectedSquare = -1;
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