// Include
#include <stdio.h>
#include "engine.h"

int PositionToIndex[BOARD_SIZE];
int IndexToPosition[64];

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
  //   |    | A1 | B1 | C1 | D1 | E1 | F1 | G1 | H1 |    |
  //   +----+----+----+----+----+----+----+----+----+----+
  //   |    | A2 | B2 | C2 | D2 | E2 | F2 | G2 | H2 |    |
  //   +----+----+----+----+----+----+----+----+----+----+
  //   |    | A3 | B3 | C3 | D3 | E3 | F3 | G3 | H3 |    |
  //   +----+----+----+----+----+----+----+----+----+----+
  //   |    | A4 | B4 | C4 | D4 | E4 | F4 | G4 | H4 |    |
  //   +----+----+----+----+----+----+----+----+----+----+
  //   |    | A5 | B5 | C5 | D5 | E5 | F5 | G5 | H5 |    |
  //   +----+----+----+----+----+----+----+----+----+----+
  //   |    | A6 | B6 | C6 | D6 | E6 | F6 | G6 | H6 |    |
  //   +----+----+----+----+----+----+----+----+----+----+
  //   |    | A7 | B7 | C7 | D7 | E7 | F7 | G7 | H7 |    |
  //   +----+----+----+----+----+----+----+----+----+----+
  //   |    | A8 | B8 | C8 | D8 | E8 | F8 | G8 | H8 |    |
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
  for (int i = 0; i < BOARD_SIZE; ++i)
  {
    PositionToIndex[i] = 64;
  }
  for (int j = 0; j < 64; ++j)
  {
    IndexToPosition[j] = 120;
  }

  // Iterate
  for (int rank = RANK_1; rank <= RANK_8; ++rank)
  {
    for (int file = FILE_A; file <= FILE_H; ++file)
    {
      position = rank * 10 + file + 21;
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
  for (int i = 0; i < BOARD_SIZE; ++i)
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
  for (int j = 0; j < 64; ++j)
  {
    if (j % 8 == 0 && j > 0)
    {
      printf("\n");
    }
    printf("%5d", IndexToPosition[j]);
  }
}

// Main function
int main(int argc, char* argv[])
{
  // Initialize engine
  init();

  // Print boards
  printPositionBoard();
  printf("\n\n");
  printIndexBoard();

  // Return
  return 0;
}
