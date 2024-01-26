// Include
#include "engine.h"

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
