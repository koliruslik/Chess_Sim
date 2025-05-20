#pragma once

enum class MenuState {
	Main,
	AiSideSelection,
	Settings
};

enum class Theme {
	Theme1,
	Theme2,
	Theme3
};

enum class MenuResult {
	None,
	StartGame,
	OpenSettings,
	ConstructPosition,
	ESCMenu,
	Exit
};

enum class ESCResult {
	None,
	Resume,
	ToMainMenu,
	Save,
	Load
};


enum PIECE {
	PAWN = 0,
	KNIGHT = 1,
	BISHOP = 2,
	ROOK = 3,
	QUEEN = 4,
	KING = 5,
	NONE = -1
};


enum SIDE {
	White = 0,
	Black = 1,
	Draw = -1,
	None = -2,
	Stalemate = -3,
	Checked = -4
};

