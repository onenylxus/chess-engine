// Include
#include <stdio.h>
#include "engine.h"

// Default values
int PositionToIndex[POSITION_SIZE];
int IndexToPosition[INDEX_SIZE];

// Get position from file and rank
int getPosition(int file, int rank)
{
  return rank * 10 + file + 21;
}

// Setup board conversion
void setupBoardConversion()
{
  // One extra file and two extra ranks are added to each respective side
  // Hence for this chess engine the board is 12x10
  //
  //   +----+----+----+----+----+----+----+----+----+----+
  //   |    |    |    |    |    |    |    |    |    |    |
  //   +----+----+----+----+----+----+----+----+----+----+
  //   |    |    |    |    |    |    |    |    |    |    |
  //   +----+----+----+----+----+----+----+----+----+----+
  //   |    | A8 | B8 | C8 | D8 | E8 | F8 | G8 | H8 |    |
  //   +----+----+----+----+----+----+----+----+----+----+
  //   |    | A7 | B7 | C7 | D7 | E7 | F7 | G7 | H7 |    |
  //   +----+----+----+----+----+----+----+----+----+----+
  //   |    | A6 | B6 | C6 | D6 | E6 | F6 | G6 | H6 |    |
  //   +----+----+----+----+----+----+----+----+----+----+
  //   |    | A5 | B5 | C5 | D5 | E5 | F5 | G5 | H5 |    |
  //   +----+----+----+----+----+----+----+----+----+----+
  //   |    | A4 | B4 | C4 | D4 | E4 | F4 | G4 | H4 |    |
  //   +----+----+----+----+----+----+----+----+----+----+
  //   |    | A3 | B3 | C3 | D3 | E3 | F3 | G3 | H3 |    |
  //   +----+----+----+----+----+----+----+----+----+----+
  //   |    | A2 | B2 | C2 | D2 | E2 | F2 | G2 | H2 |    |
  //   +----+----+----+----+----+----+----+----+----+----+
  //   |    | A1 | B1 | C1 | D1 | E1 | F1 | G1 | H1 |    |
  //   +----+----+----+----+----+----+----+----+----+----+
  //   |    |    |    |    |    |    |    |    |    |    |
  //   +----+----+----+----+----+----+----+----+----+----+
  //   |    |    |    |    |    |    |    |    |    |    |
  //   +----+----+----+----+----+----+----+----+----+----+
  //

  // Define variables
  int position = A1;
  int index = 0;

  // Set default values
  for (int i = 0; i < POSITION_SIZE; ++i)
  {
    PositionToIndex[i] = INDEX_SIZE;
  }
  for (int j = 0; j < INDEX_SIZE; ++j)
  {
    IndexToPosition[j] = POSITION_SIZE;
  }

  // Iterate
  for (int rank = RANK_1; rank <= RANK_8; ++rank)
  {
    for (int file = FILE_A; file <= FILE_H; ++file)
    {
      position = getPosition(file, rank);
      IndexToPosition[index] = position;
      PositionToIndex[position] = index;
      index++;
    }
  }
}

// Init function
void init()
{
  setupBoardConversion();
}

// Print position board
void printPositionBoard()
{
  printf("Position board:\n");
  for (int i = 0; i < POSITION_SIZE; ++i)
  {
    if (i % 10 == 0 && i > 0)
    {
      printf("\n");
    }
    printf("%5d", PositionToIndex[i]);
  }
}

// Print index board
void printIndexBoard()
{
  printf("Index board:\n");
  for (int j = 0; j < INDEX_SIZE; ++j)
  {
    if (j % 8 == 0 && j > 0)
    {
      printf("\n");
    }
    printf("%5d", IndexToPosition[j]);
  }
}

// Print bitboard
void printBitboard(u64 key)
{
  int position = 0;
  int index = 0;

  printf("Bitboard:\n");
  for (int rank = RANK_8; rank >= RANK_1; --rank)
  {
    for (int file = FILE_A; file <= FILE_H; ++file)
    {
      position = getPosition(file, rank);
      index = PositionToIndex[position];
      if (key & (1ULL << index))
      {
        printf(" X ");
      }
      else
      {
        printf(" - ");
      }
    }
    printf("\n");
  }
}

// Main function
int main(int argc, char* argv[])
{
  // Initialize engine
  init();

  // Print boards
  printf("\n\n");
  printPositionBoard();
  printf("\n\n");
  printIndexBoard();
  printf("\n\n");
  printBitboard(1ULL << PositionToIndex[A2]);
  printf("\n\n");

  // Return
  return 0;
}
