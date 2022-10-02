// Include
#pragma once
#include "stdlib.h"

// Constants
#define DEBUG
#define NAME "Chess Engine"
#define POSITION_SIZE 120
#define INDEX_SIZE 64
#define MAX_MOVES 2048

// Type definitions
typedef unsigned long long u64;

// Assert
#ifndef DEBUG
#define ASSERT(n)
#else
#define ASSERT(n)                    \
  if (!n)                            \
  {                                  \
    printf("Assert: %s", #n);        \
    printf("on %s ", __DATE__);      \
    printf("at %s ", __TIME__);      \
    printf("in file %s", __FILE__);  \
    printf("at line %s ", __LINE__); \
    exit(1);                         \
  }
#endif

// Boolean
enum {
  FALSE,
  TRUE
};

// Players
enum
{
  WHITE,
  BLACK,
  BOTH,
  PLAYER_SIZE
};

// Pieces
enum
{
  EMPTY,
  WHITE_PAWN,
  WHITE_KNIGHT,
  WHITE_BISHOP,
  WHITE_ROOK,
  WHITE_QUEEN,
  WHITE_KING,
  BLACK_PAWN,
  BLACK_KNIGHT,
  BLACK_BISHOP,
  BLACK_ROOK,
  BLACK_QUEEN,
  BLACK_KING,
  PIECE_SIZE
};

// Files
enum
{
  FILE_A,
  FILE_B,
  FILE_C,
  FILE_D,
  FILE_E,
  FILE_F,
  FILE_G,
  FILE_H,
  FILE_X
};

// Ranks
enum
{
  RANK_1,
  RANK_2,
  RANK_3,
  RANK_4,
  RANK_5,
  RANK_6,
  RANK_7,
  RANK_8,
  RANK_X
};

// Positions
enum
{
  A1 = 21, A2, A3, A4, A5, A6, A7, A8,
  B1 = 31, B2, B3, B4, B5, B6, B7, B8,
  C1 = 41, C2, C3, C4, C5, C6, C7, C8,
  D1 = 51, D2, D3, D4, D5, D6, D7, D8,
  E1 = 61, E2, E3, E4, E5, E6, E7, E8,
  F1 = 71, F2, F3, F4, F5, F6, F7, F8,
  G1 = 81, G2, G3, G4, G5, G6, G7, G8,
  H1 = 91, H2, H3, H4, H5, H6, H7, H8,
  XX
};

// Castling
enum
{
  CASTLE_WHITE_KING = 1,
  CASTLE_WHITE_QUEEN = 2,
  CASTLE_BLACK_KING = 4,
  CASTLE_BLACK_QUEEN = 8
};

// Undo-move struct
typedef struct
{
  int move;             // Move value
  int castlePermission; // Castle permission
  int enPassant;        // En passant position
  int fiftyMoves;       // Move counter for fifty moves
  u64 positionKey;      // Unique key for position
} UndoMove;

// Board struct
typedef struct
{
  int pieceStates[POSITION_SIZE]; // Piece states of each position
  u64 pawns[PLAYER_SIZE];         // Pawn positions in bit for each player
  int kingSquares[2];             // King positions for white and black players
  int side;                       // Current player side to move
  int castle;                     // Castle permutation
  int enPassant;                  // En passant position
  int fiftyMoves;                 // Move counter for fifty moves
  int currentPly;                 // Current ply (half move)
  int historyPly;                 // History ply
  u64 positionKey;                // Unique key for position
  int counts[PIECE_SIZE];         // Total number of pieces on the board
  int bigPieceCounts[3];          // Number of big pieces (not pawns) on the board for each player
  int majorPieceCounts[3];        // Number of major pieces (rooks and queens) on the board for each player
  int minorPieceCounts[3];        // Number of minor pieces (knights and bishops) on the board for each player
  int pieceList[PIECE_SIZE][10];  // Position of each piece sorted by piece type
  UndoMove history[MAX_MOVES];    // Move history
} Board;
