// Include
#include "engine.h"

// Conversion test
void ConversionTest()
{
  // Print message
  printf("Running conversion tests...\n");

  // Expected results
  int positionTable[POSITION_SIZE] = {
    64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
    64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
    64,  0,  1,  2,  3,  4,  5,  6,  7, 64,
    64,  8,  9, 10, 11, 12, 13, 14, 15, 64,
    64, 16, 17, 18, 19, 20, 21, 22, 23, 64,
    64, 24, 25, 26, 27, 28, 29, 30, 31, 64,
    64, 32, 33, 34, 35, 36, 37, 38, 39, 64,
    64, 40, 41, 42, 43, 44, 45, 46, 47, 64,
    64, 48, 49, 50, 51, 52, 53, 54, 55, 64,
    64, 56, 57, 58, 59, 60, 61, 62, 63, 64,
    64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
    64, 64, 64, 64, 64, 64, 64, 64, 64, 64
  };
  int indexTable[INDEX_SIZE] = {
    21, 22, 23, 24, 25, 26, 27, 28,
    31, 32, 33, 34, 35, 36, 37, 38,
    41, 42, 43, 44, 45, 46, 47, 48,
    51, 52, 53, 54, 55, 56, 57, 58,
    61, 62, 63, 64, 65, 66, 67, 68,
    71, 72, 73, 74, 75, 76, 77, 78,
    81, 82, 83, 84, 85, 86, 87, 88,
    91, 92, 93, 94, 95, 96, 97, 98
  };

  // Test position table
  for (int i = 0; i < POSITION_SIZE; ++i)
  {
    ASSERT(PositionToIndex[i] == positionTable[i]);
  }

  // Test index table
  for (int i = 0; i < INDEX_SIZE; ++i)
  {
    ASSERT(IndexToPosition[i] == indexTable[i]);
  }
}
