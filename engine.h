// Include
#pragma once
#include "stdlib.h"

// Constants
#define DEBUG
#define NAME "Chess Engine"
#define POSITION_SIZE 120
#define INDEX_SIZE 64
#define MAX_MOVES 2048
#define FEN_INIT "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"

// Type definitions
typedef unsigned long long u64;

// Assert
#ifndef DEBUG
#define ASSERT(n)
#else
#define ASSERT(n)                     \
  if (!(n))                           \
  {                                   \
    printf("Assert: %s ", #n);        \
    printf("on %s ", __DATE__);       \
    printf("at %s ", __TIME__);       \
    printf("in file %s ", __FILE__);  \
    printf("at line %d\n", __LINE__); \
    exit(1);                          \
  }
#endif

// Random hash key
#ifndef DEBUG
#define RAND_HASH
#else
#define RAND_HASH           \
(                           \
  (u64)rand() +             \
  (u64)rand() << 15 +       \
  (u64)rand() << 30 +       \
  (u64)rand() << 45 +       \
  ((u64)rand() & 0xf) << 60 \
)
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
  PLAYER_SIZE,
  SIDE_SIZE = 2
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
  A1 = 21, B1, C1, D1, E1, F1, G1, H1,
  A2 = 31, B2, C2, D2, E2, F2, G2, H2,
  A3 = 41, B3, C3, D3, E3, F3, G3, H3,
  A4 = 51, B4, C4, D4, E4, F4, G4, H4,
  A5 = 61, B5, C5, D5, E5, F5, G5, H5,
  A6 = 71, B6, C6, D6, E6, F6, G6, H6,
  A7 = 81, B7, C7, D7, E7, F7, G7, H7,
  A8 = 91, B8, C8, D8, E8, F8, G8, H8,
  XX, OB
};

// Castling
enum
{
  CASTLE_WHITE_KING = 1,
  CASTLE_WHITE_QUEEN = 2,
  CASTLE_BLACK_KING = 4,
  CASTLE_BLACK_QUEEN = 8,
  CASTLE_SIZE = 16
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
  int pieces[POSITION_SIZE];            // Piece states of each position
  u64 pawns[PLAYER_SIZE];               // Pawn positions in bit for each player
  int kingSquares[SIDE_SIZE]; // King positions for each player
  int side;                             // Current player side to move
  int castle;                           // Castle permutation
  int enPassant;                        // En passant position
  int fiftyMoves;                       // Move counter for fifty moves
  int currentPly;                       // Current ply (half move)
  int historyPly;                       // History ply
  u64 positionKey;                      // Unique key for position
  int counts[PIECE_SIZE];               // Total number of pieces on the board
  int bigPieces[SIDE_SIZE];   // Number of big pieces (not pawns) on the board for each player
  int majorPieces[SIDE_SIZE]; // Number of major pieces (rooks and queens) on the board for each player
  int minorPieces[SIDE_SIZE]; // Number of minor pieces (knights and bishops) on the board for each player
  int materials[SIDE_SIZE];   // Materials for each player
  int pieceList[PIECE_SIZE][10];        // Position of each piece sorted by piece type
  UndoMove history[MAX_MOVES];          // Move history
} Board;
