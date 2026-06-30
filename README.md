# Chess Sim

A desktop chess application written in C++ with a custom chess engine, graphical interface, and built-in AI opponent.

The project focuses on implementing chess mechanics from scratch: position representation, legal move generation, game-state evaluation, search algorithms, saving/loading, and a Raylib-based user interface.

<img width="637" height="637" alt="image" src="https://github.com/user-attachments/assets/84d6a56e-5155-4785-8804-d8f6e83acd73" />


## Download

Download the latest Windows build from the GitHub Releases page:

[**Download latest release (.exe)**](https://github.com/koliruslik/Chess_Sim/releases/latest)

After downloading, extract the archive if needed and run `Chess_sim.exe`.

> Windows may display a SmartScreen warning because the application is not code-signed. Choose **More info → Run anyway** only after confirming that you downloaded it from this repository’s Releases page.

## Features

* Play locally against a friend.
* Play against the AI as either White or Black.
* Three visual board and piece themes.
* Custom position constructor with placement validation.
* Save and load game states through three save slots.
* Pause menu available during a game.
* Chess AI with:
  * opening book support;
  * alpha-beta pruning;
  * capture-only search at leaf nodes;
  * move ordering;
  * transposition table;
  * static position evaluation.
* Internal use of bitboards and Zobrist hashing for board representation and position tracking.

## Controls

| Action                 | Control                                    |
| ---------------------- | ------------------------------------------ |
| Select a piece         | Left mouse button                          |
| Make a move            | Left mouse button on a valid target square |
| Open pause menu        | `Esc`                                      |
| Select promotion piece | Mouse                                      |
| Save or load a game    | Pause menu                                 |

## Game Modes

### VS Friend

Local two-player mode on a single computer.

### VS AI

Choose the side you want to play. The AI controls the opposite side and searches for moves using alpha-beta pruning, move ordering, a transposition table, and an opening book.

### Construct Position

Create a custom chess position directly on the board. The constructor includes validation rules such as king placement restrictions and piece-count limits.

## Technologies

* C++20
* CMake
* Raylib
* Bitboards
* Alpha-beta search
* Zobrist hashing
* Transposition table
* Opening book

## Project Context

This project was originally developed as a university coursework project. Its goal was to combine chess-engine fundamentals, desktop UI development, file persistence, and AI search techniques in one practical application.

## Limitations and Future Improvements

The project is published as an educational project and has room for further technical improvements:

* Separate the chess engine, UI, persistence, and application layers into clearer modules.
* Add automated unit tests for move generation, position validation, and game-end conditions.
* Improve code consistency and reduce coupling between gameplay and rendering logic.
* Introduce continuous integration for automated builds and test execution.
* Improve AI search depth, evaluation heuristics, and performance profiling.
* Add support for more chess rules and formats, such as PGN export/import.


## Core Components

| Component            | Responsibility                                                               |
| -------------------- | ---------------------------------------------------------------------------- |
| `Position`           | Stores board state, side to move, castling rights, move counters, and hashes |
| `LegalMoveGen`       | Generates legal chess moves                                                  |
| `Game`               | Coordinates gameplay, input, timers, rendering, and victory checks           |
| `AI`                 | Selects moves using alpha-beta search                                        |
| `OpeningBook`        | Provides opening moves when available                                        |
| `TranspositionTable` | Caches evaluated positions during search                                     |
| `BoardRenderer`      | Draws the board, pieces, highlights, and UI feedback                         |
| `PosConstructor`     | Creates and validates custom positions                                       |

## AI Overview

The AI first attempts to select a move from the opening book. If no suitable opening move is available, it evaluates positions with alpha-beta search.

The search includes:

* move ordering to examine promising moves earlier;
* a transposition table to reuse previously evaluated positions;
* static position evaluation;
* capture-only continuation search at leaf nodes;
* draw detection for the fifty-move rule and threefold repetition.

## Project Structure

```text
Chess_Sim/
├── Chess_sim/            # Application, chess engine, UI, AI
├── external/
│   └── raylib/           # Rendering library
├── saves/                # Runtime save files
├── CMakeLists.txt
└── README.md
```

## Educational Goal

This project was created as a practical exploration of chess-engine development and desktop game programming in C++. It combines algorithmic work with UI development, state management, file persistence, and third-party library integration.
