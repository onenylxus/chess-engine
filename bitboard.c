// Include
#include "engine.h"

// Bit table
const int BitTable[INDEX_SIZE] = {
  63, 30,  3, 32, 25, 41, 22, 33,
  15, 50, 42, 13, 11, 53, 19, 34,
  61, 29,  2, 51, 21, 43, 45, 10,
  18, 47,  1, 54,  9, 57,  0, 35,
  62, 31, 40,  4, 49,  5, 52, 26,
  60,  6, 23, 44, 46, 27, 56, 16,
   7, 39, 48, 24, 59, 14, 12, 55,
  38, 28, 58, 20, 37, 17, 36,  8
};

// Pop bit from bitboard
int PopBit(u64 *bb)
{
  u64 bit = *bb ^ (*bb - 1);
  u32 fold = (u32)((bit & 0xffffffff) ^ (bit >> 32));
  *bb &= (*bb - 1);
  return BitTable[(fold * 0x783a9b23) >> 26];
}

// Count number of bits in bitboard
int CountBit(u64 bb)
{
  int count;
  for (count = 0; bb; ++count)
  {
    bb &= bb - 1;
  }
  return count;
}

// Print bitboard
void PrintBitboard(u64 bb)
{
  // Define variable
  int position = XX;
  int index = 0;

  // Print
  printf("Bitboard:\n");
  printf("+---+---+---+---+---+---+---+---+\n");
  for (int r = RANK_8; r >= RANK_1; --r)
  {
    for (int f = FILE_A; f <= FILE_H; ++f)
    {
      position = FR2POS(f, r);
      index = POS2IDX(position);
      printf(((1ULL << index) & bb) ? "| x " : "| - ");
    }
    printf("|\n");
    printf("+---+---+---+---+---+---+---+---+\n");
  }
  printf("\n");
}
