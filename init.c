// Include
#include "engine.h"

// Random u64 value generator
#define RAND_U64 (           \
  (u64)rand() +              \
  (u64)rand() << 15 +        \
  (u64)rand() << 30 +        \
  (u64)rand() << 45 +        \
  ((u64)rand() & 0x0f) << 60 \
)

// Define global variables
int PositionToIndex[POSITION_SIZE];
int IndexToPosition[INDEX_SIZE];
u64 SetMask[INDEX_SIZE];
u64 ClearMask[INDEX_SIZE];
u64 PieceKeys[PIECE_SIZE][POSITION_SIZE];
u64 SideKey;
u64 CastleKeys[CASTLE_SIZE];

// Initialize conversions
void InitConversion()
{
  // Define variables
  int position = XX;
  int index = 0;

  // Fill conversion tables
  for (int i = 0; i < POSITION_SIZE; ++i)
  {
    PositionToIndex[i] = INDEX_SIZE;
  }
  for (int i = 0; i < INDEX_SIZE; ++i)
  {
    IndexToPosition[i] = POSITION_SIZE;
  }

  // Replace with actual values
  for (int r = RANK_1; r <= RANK_8; ++r)
  {
    for (int f = FILE_A; f <= FILE_H; ++f)
    {
      position = FR2POS(f, r);
      PositionToIndex[position] = index;
      IndexToPosition[index] = position;
      index++;
    }
  }
}

// Initialize bit masks
void InitBitMask()
{
  // Fill bit masks
  for (int i = 0; i < INDEX_SIZE; ++i)
  {
    SetMask[i] = (1ULL << i);
    ClearMask[i] = ~SetMask[i];
  }
}

// Initialize hash keys
void InitHashKeys()
{
  // Fill hash keys
  for (int i = 0; i < PIECE_SIZE; ++i)
  {
    for (int j = 0; j < POSITION_SIZE; ++j)
    {
      PieceKeys[i][j] = RAND_U64;
    }
  }
  SideKey = RAND_U64;
  for (int i = 0; i < CASTLE_SIZE; ++i)
  {
    CastleKeys[i] = RAND_U64;
  }
}

// Initialize function
void Init()
{
  InitConversion();
  InitBitMask();
  InitHashKeys();
}
