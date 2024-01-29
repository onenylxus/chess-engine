// Include
#include "engine.h"

// Define global variables
int PositionToIndex[POSITION_SIZE];
int IndexToPosition[INDEX_SIZE];
u64 SetMask[INDEX_SIZE];
u64 ClearMask[INDEX_SIZE];

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

// Initialize function
void Init()
{
  InitConversion();
  InitBitMask();
}
