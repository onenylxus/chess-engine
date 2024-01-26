//// Include ////
#pragma once
#include <stdlib.h>

//// Constants ////
#define DEBUG               // Debug mode
#define NAME "Chess Engine" // Engine name

#define POSITION_SIZE 120   // Position size (12x10 virtual board)
#define INDEX_SIZE 64       // Index size (8x8 real board)
#define PLAYER_SIZE 2       // Number of players (white and black)
#define GROUP_SIZE 3        // Number of groups (white, black and both)
#define PIECE_TYPE_SIZE 13  // Number of piece types

//// Type Definitions ////
typedef unsigned long long u64;

//// Enumerators ////

// Boolean
enum Boolean
{
  FALSE,
  TRUE
};

// Players
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

// Positions
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
