// Include
#pragma once
#include <stdio.h>
#include <stdlib.h>

//// Constants ////

#define DEBUG               // Debug mode
#define NAME "Chess Engine" // Engine name

#define POSITION_SIZE 120   // Position size (12x10 virtual board)
#define INDEX_SIZE 64       // Index size (8x8 real board)
#define PLAYER_SIZE 2       // Number of players (white and black)
#define GROUP_SIZE 3        // Number of groups (white, black and both)
#define PIECE_SIZE 13       // Number of piece types
#define CASTLE_SIZE 16      // Castle permutation size
#define MAX_MOVES 2048      // Maximum number of moves
#define MAX_CHOICES 256     // Maximum number of choices
#define MAX_PIECES 10       // Maximum number of pieces of same type

//// Type Definitions ////
typedef unsigned int u32;
typedef unsigned long long u64;

//// Enumerators ////

// Boolean
enum Boolean
{
  FALSE,
  TRUE
};

// Player
enum Player
{
  WHITE,
  BLACK,
  BOTH,
};

// Piece type
enum PieceType
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
};

// File
enum File
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

// Rank
enum Rank
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

// Position
enum Position
{
  A1 = 21, B1, C1, D1, E1, F1, G1, H1,
  A2 = 31, B2, C2, D2, E2, F2, G2, H2,
  A3 = 41, B3, C3, D3, E3, F3, G3, H3,
  A4 = 51, B4, C4, D4, E4, F4, G4, H4,
  A5 = 61, B5, C5, D5, E5, F5, G5, H5,
  A6 = 71, B6, C6, D6, E6, F6, G6, H6,
  A7 = 81, B7, C7, D7, E7, F7, G7, H7,
  A8 = 91, B8, C8, D8, E8, F8, G8, H8,
  XX
};

// Castling
enum Castling
{
  CASTLE_WHITE_KING = 1,
  CASTLE_WHITE_QUEEN = 2,
  CASTLE_BLACK_KING = 4,
  CASTLE_BLACK_QUEEN = 8
};

//// Structures ////

// Record
typedef struct
{
  int side;        // Current player side to move
  int castle;      // Castle permutation
  int enPassant;   // En passant position
  int fiftyMoves;  // Move counter for fifty moves
  u64 positionKey; // Unique key for position
} Record;

// Board
typedef struct
{
  int pieces[POSITION_SIZE];             // Piece type of each position
  u64 pawns[GROUP_SIZE];                 // Pawn positions in bit for each player and combined
  int kings[PLAYER_SIZE];                // King positions for each player
  int side;                              // Current player side to move
  int castle;                            // Castle permutation
  int enPassant;                         // En passant position
  int fiftyMoves;                        // Move counter for fifty moves
  int currentPly;                        // Current ply
  int historyPly;                        // History ply
  u64 positionKey;                       // Unique key for position
  int counts[PIECE_SIZE];                // Total number of pieces on the board
  int bigPieces[GROUP_SIZE];             // Number of big pieces (not pawns) on the board for each player
  int majorPieces[GROUP_SIZE];           // Number of major pieces (rooks and queens) on the board for each player
  int minorPieces[GROUP_SIZE];           // Number of minor pieces (knights and bishops) on the board for each player
  int pieceList[PIECE_SIZE][MAX_PIECES]; // Position of each piece sorted by piece type
  Record history[MAX_MOVES];             // History records of each move
} Board;

//// Macros ////

// Assert function
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

#define FR2POS(f, r) ((r) * 10 + (f) + 21) // Conversion from file and rank to position
#define POS2IDX(p) PositionToIndex[p]      // Conversion from position to index
#define IDX2POS(i) IndexToPosition[i]      // Conversion from index to position
#define POP(b) PopBit(b)                   // Pop bit
#define COUNT(b) CountBit(b)               // Count bit
#define SET(b, i) ((b) |= SetMask[i])      // Set bit
#define CLEAR(b, i) ((b) &= ClearMask[i])  // Clear bit

//// Global variables ////

extern int PositionToIndex[POSITION_SIZE]; // Conversion table from position to index
extern int IndexToPosition[INDEX_SIZE];    // Conversion table from index to position
extern u64 SetMask[INDEX_SIZE];            // Bitboard set bit mask value
extern u64 ClearMask[INDEX_SIZE];          // Bitboard clear bit mask value

//// External functions ////

// init.c
extern void Init();

// bitboard.c
extern int PopBit(u64 *bb);
extern int CountBit(u64 bb);
extern void PrintBitboard(u64 bb);

// tests.c
extern void Test();
