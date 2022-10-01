// Include
#pragma once
#define NAME "Chess Engine"
#define BOARD_SIZE 120

typedef unsigned long long u64;

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
  BOTH
};

// Pieces
enum
{
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
  EMPTY
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
