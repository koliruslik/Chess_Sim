#include "Game.hpp"

Game::Game(Position position)
{
	selectedSquare = -1;
	this->position = position;
	loadPieceTextures();
}

int Game::processMove(Position& position, MoveList& moves, uint8_t from, uint8_t to,uint8_t side)
{

	if (from == -1 || to == -1) {
		std::cout << "INVALID.\n";
		return -1;
	}

	moves = LegalMoveGen::generate(position, side);
	bool validMove = false;

	for (int i = 0; i < moves.getSize(); ++i) {
		if (moves[i].getFrom() == from && moves[i].getTo() == to) {
			handlePawnPromotion(moves[i]);
			position.move(moves[i]);
			validMove = true;
			break;
		}
	}

	if (!validMove) {
		std::cout << "Invalid move.\n";
		return -1;
	}
	if (checkVictory(side)) {
		return 0;
	}

	return 1;
}

int Game::processMoveWithClick(Vector2 mousePos)
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
			processMove(position, moves, from, to, position.getSideToMove());
			selectedSquare = -1;
			isSelected = false;
			return 1;
		}
	}
	return -1;
}

int Game::processGame()
{

	drawBoard();
	update();
	//EndDrawing();
	/*MoveList moves = LegalMoveGen::generate(position, (position.getSideToMove() == "White") ? SIDE::White : SIDE::Black);
	std::cout << position;

	std::string from, to;
	SIDE side = (position.getSideToMove() == "White") ? SIDE::White : SIDE::Black;
	std::cout << "\nEnter move (" << (side == SIDE::White ? "White" : "Black") << "): ";
	std::cin >> from >> to;

	int game = processMove(position, moves, from, to, side);

	if (game == -1 || game == 1) 
	{
		return 1;
	}
	std::cout << "Game over. Enter 'r' to restart or 'q' to quit: ";
	char choice;
	std::cin >> choice;
	if (choice == 'r') 
	{
		position = Position(startingPosition, Position::NONE, true, true, true, true, 0.0f);
	}
	else if (choice == 'q') 
	{
		return -1;
	}*/
	return 1;
}

void Game::handlePawnPromotion(Move& move)
{
	uint8_t to = move.getTo();
	if (move.getAttackerType() == PIECE::PAWN)
	{
		if ((move.getAttackerSide() == SIDE::White && to >= 56) ||
			(move.getAttackerSide() == SIDE::Black && to <= 7))
		{
			std::string pieceChoice;
			std::cout << "Pawn reached the last row! Choose a promotion piece (queen, rook, knight, bishop): ";
			std::cin >> pieceChoice;

			if (pieceChoice == "queen") {
				std::cout << "PROMOTE TO QUEEN\n";
				move.setFlag(Move::FLAG::PROMOTE_TO_QUEEN);
			}
			else if (pieceChoice == "rook") {
				std::cout << "PROMOTE TO ROOK\n";
				move.setFlag(Move::FLAG::PROMOTE_TO_ROOK);
			}
			else if (pieceChoice == "knight") {
				std::cout << "PROMOTE TO KNIGHT\n";
				move.setFlag(Move::FLAG::PROMOTE_TO_KNIGHT);
			}
			else if (pieceChoice == "bishop") {
				std::cout << "PROMOTE TO BISHOP\n";
				move.setFlag(Move::FLAG::PROMOTE_TO_BISHOP);
			}
			else {
				std::cout << "Invalid choice, defaulting to queen.\n";
				move.setFlag(Move::FLAG::PROMOTE_TO_QUEEN);
			}
		}
	}
}

bool Game::checkVictory(uint8_t side) 
{
	bool isCheck = PsLegalMoveMaskGen::inDanger(position.getPieces(), BOp::bsf(position.getPieces().getPieceBitboard(position.getSideToMove(), PIECE::KING)), position.getSideToMove());
	bool fiftymoves = position.fiftyMovesRuleDraw();
	bool threefold = position.threefoldRepetitionDraw();
	SIDE opponent = (side == SIDE::White) ? SIDE::Black : SIDE::White;
	MoveList opponentMoves = LegalMoveGen::generate(position, opponent);
	if (!opponentMoves.hasMoves() && isCheck) 
	{
		std::cout << position;
		std::cout << (side == SIDE::White ? "\n\t-----WHITE WON-----\n" : "\n\t-----BLACK WON-----\n");
		return 1;
	}
	else if ((!opponentMoves.hasMoves() && !isCheck) || fiftymoves || threefold)
	{
		std::cout << position;
		std::cout << "\t-----DRAW-----\n";
		return 1;
	}
	return 0;
}

uint8_t Game::squareToIndex(const std::string& square) const
{
	if (square.length() != 2) return -1;
	char file = square[0];
	char rank = square[1];

	if (file < 'a' || file > 'h' || rank < '1' || rank > '8') return -1;

	return (rank - '1') * 8 + (file - 'a');
}

std::string Game::indexToSquare(uint8_t index) const
{
	if (index < 0 || index > 63) return "??";

	char file = 'a' + (index % 8);
	char rank = '1' + (index / 8);

	return std::string(1, file) + std::string(1, rank);
}

void Game::loadPieceTextures()
{
	for (const auto& name : names)
	{
		std::string whiteTexturePath = whitePiecesPath + name + ".png";
		std::string blackTexturePath = blackPiecesPath + name + ".png";

		// Проверка белой текстуры
		std::cout << "Loading white piece texture: " << whiteTexturePath << std::endl;
		Texture2D whiteTexture = LoadTexture(whiteTexturePath.c_str());
		if (whiteTexture.id == 0) {
			std::cerr << "Failed to load white piece texture: " << whiteTexturePath << std::endl;
		}
		else {
			whitePieces[name] = whiteTexture;
		}

		// Проверка черной текстуры
		std::cout << "Loading black piece texture: " << blackTexturePath << std::endl;
		Texture2D blackTexture = LoadTexture(blackTexturePath.c_str());
		if (blackTexture.id == 0) {
			std::cerr << "Failed to load black piece texture: " << blackTexturePath << std::endl;
		}
		else {
			blackPieces[name] = blackTexture;
		}
	}
}

void Game::drawBoard()
{
	const char* files[8] = { "a", "b", "c", "d", "e", "f", "g", "h" };
	for (int row = 0; row < boardSize; ++row)
	{
		for (int col = 0; col < boardSize; ++col)
		{
			Color squareColor = ((row + col) % 2 == 0) ? WHITE : DARKGRAY;
			Color textColor = ((row + col) % 2 == 0) ? DARKGRAY : WHITE;
			DrawRectangle(col * squareSize, row * squareSize, squareSize, squareSize, squareColor);

			std::string rowIndex = std::to_string(row);
			std::string fileIndex = files[col];
			if (col == 0)
			{
				DrawText(rowIndex.c_str(), col * squareSize+5, row * squareSize, 20, textColor);
			}
			if (row == 7)
			{
				DrawText(fileIndex.c_str(), col * squareSize+5, row * squareSize, 20, textColor);
			}
		}
	}

	drawPieces();
	drawMask();
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
	std::cout << "selected square: " << selectedSquare << std::endl;
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
		std::cout << "to " << to << std::endl;
		int col = to % 8;
		int row = 7 - (to / 8); 
		std::cout << "possible move: " << indexToSquare(to) << std::endl;
		DrawRectangle(col * squareSize, row * squareSize, squareSize, squareSize, BLUE);
	}

}
Vector2 Game::centerPiece(float pieceSize, float texWidth, float texHeight) const
{
	const float xOffset = squareSize/2;
	const float yOffset = squareSize/2 - 5; 

	return { xOffset, yOffset };
}



void Game::selectPiece(Vector2 mousePos)
{
	int col = mousePos.x / squareSize;
	int row = boardSize - (mousePos.y / squareSize);

	if (col >= 0 && col < boardSize && row >= 0 && row < boardSize)
	{
		std::string square = std::string(1, char('a' + col)) + std::string(1, char('1' + row));
		int squareIndex = squareToIndex(square);
		std::cout << "Clicked square: " << square << " (" << squareIndex << ")" << std::endl;

		
		if (position.getPieceSideAt(squareIndex) == position.getSideToMove() && position.getPieceTypeAt(squareIndex, position.getSideToMove()) != Position::NONE)
		{
			
			if (selectedSquare == squareIndex)
			{
				std::cout << "Deselecting piece at: " << square << std::endl;
				selectedSquare = -1; 
				isSelected = false;  
			}
			else
			{
				std::cout << "Selected piece at: " << square << std::endl;
				selectedSquare = squareIndex;  
				isSelected = true;             
			}
		}
		else
		{
			
			std::cout << "Invalid selection or opponent's piece. Deselecting.\n";
			selectedSquare = -1; 
			isSelected = false;  
		}
	}
	else
	{
		
		std::cout << "Invalid selection. Deselecting.\n";
		selectedSquare = -1;  
		isSelected = false;  
	}
}

void Game::update()
{
	Vector2 mousePos = GetMousePosition();

	if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))  
	{
		
		if (isSelected)
		{
			if (processMoveWithClick(mousePos) == 1) return;
			clickCount = 0;
			selectPiece(mousePos);
		}
		else
		{
			selectPiece(mousePos);
		}
	}
}