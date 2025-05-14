# CMakeList.txt: проект CMake для Chess_sim; включите исходный код и определения,
# укажите здесь логику для конкретного проекта.
#
# Добавьте источник в исполняемый файл этого проекта.
add_executable(Chess_sim "Chess_sim.cpp" "Chess_sim.h"
						 "BitBoard.hpp" 
						"Pieces.cpp" "Pieces.hpp" 
						"MainMenu.cpp" "MainMenu.h"
						"Menu.cpp" "Menu.h" 
						  
						"Button.cpp" "Button.h" 
						"Timer.h" "Timer.cpp" 
						"ZobristHash.hpp" "ZobristHash.cpp" "ZobristHashConstants.hpp"
						"RepetitionHistory.hpp" "RepetitionHistory.cpp" 
						"Move.hpp" "Move.cpp" 
						"Position.hpp" "Position.cpp"
						"PsLegalMoveMaskGen.hpp" "PsLegalMoveMaskGen.cpp"
						"MoveList.hpp" "MoveList.cpp" 
						"LegalMoveGen.hpp" "LegalMoveGen.cpp"
						"LegalMoveGenTester.hpp" "LegalMoveGenTester.cpp"  
						"Game.hpp" "Game.cpp" 
						"StaticEvaluator.h" "StaticEvaluator.cpp"
						"PassedPawnMasks.h" "PawnShieldMasks.h"
						"MoveSorter.cpp" "MoveSorter.hpp" 
						"Entry.cpp" "Entry.hpp" 
						"TranspositionTable.h" "TranspositionTable.cpp"
						"OpeningBook.h" "OpeningBook.cpp" 
						"AI.h" "AI.cpp" "BTrans.h" "MenuResult.h")


if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Chess_sim PROPERTY CXX_STANDARD 20)
endif()

# TODO: Добавьте тесты и целевые объекты, если это необходимо.
target_link_libraries(Chess_sim raylib)