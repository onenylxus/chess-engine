// Include
#include <stdio.h>
#include "engine.h"

//// Variables ////

// Conversion arrays
int PositionToIndex[POSITION_SIZE];
int IndexToPosition[INDEX_SIZE];

// Magic bitboard
// This bitboard provides block and move information simultaneously
// Hence provides faster lookup for piece movement
const int MagicBitboard[INDEX_SIZE] =
{
  63, 30,  3, 32, 25, 41, 22, 33,
  15, 50, 42, 13, 11, 53, 19, 34,
  61, 29,  2, 51, 21, 43, 45, 10,
  18, 47,  1, 54,  9, 57,  0, 35,
  62, 31, 40,  4, 49,  5, 52, 26,
  60,  6, 23, 44, 46, 27, 56, 16,
   7, 39, 48, 24, 59, 14, 12, 55,
  38, 28, 58, 20, 37, 17, 36,  8
};

// Bit masks
u64 SetMask[INDEX_SIZE];
u64 ClearMask[INDEX_SIZE];

// Hash keys
u64 PieceKeys[PIECE_SIZE][POSITION_SIZE];
u64 SideKey;
u64 CastleKeys[CASTLE_SIZE];

//// Getters ////

// Get position from file and rank
int getPosition(int file, int rank)
{
  return rank * 10 + file + 21;
}

//// Init ////

// Initialize board conversion
void initBoardConversion()
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

// Initalize bit masks
void initBitMask()
{
  for (int i = 0; i < INDEX_SIZE; ++i)
  {
    SetMask[i] = (1ULL << i);
    ClearMask[i] = ~SetMask[i];
  }
}

// Initalize hash keys
void initHashKeys()
{
  for (int i = 0; i < PIECE_SIZE; ++i)
  {
    for (int j = 0; j < POSITION_SIZE; ++j)
    {
      PieceKeys[i][j] = RAND_HASH;
    }
  }
  SideKey = RAND_HASH;
  for (int i = 0; i < CASTLE_SIZE; ++i)
  {
    CastleKeys[i] = RAND_HASH;
  }
}

// Init function
void init()
{
  initBoardConversion();
  initBitMask();
  initHashKeys();
}

//// Bitboard ////

// Pop bit
int popBit(u64* bb)
{
  u64 b = *bb ^ (*bb - 1);
  unsigned int fold = (unsigned int)((b & 0xffffffff) ^ (b >> 32));
  *bb &= *bb - 1;
  return MagicBitboard[(fold * 0x783a9b23) >> 26];
}

// Set bit
void setBit(u64* bb, int sq)
{
  *bb |= SetMask[sq];
}

// Clear bit
void clearBit(u64* bb, int sq)
{
  *bb &= ClearMask[sq];
}

// Count bit
int countBit(u64 b)
{
  int count = 0;
  while (b)
  {
    count++;
    b &= b - 1;
  }
  return count;
}

//// Hash ////
u64 generatePositionKey(const Board* board)
{
  u64 key = 0;

  for (int i = 0; i < POSITION_SIZE; ++i)
  {
    int piece = board->pieceStates[i];
    if (piece != XX && piece != EMPTY)
    {
      ASSERT(piece >= WHITE_PAWN && piece <= BLACK_KING);
      key ^= PieceKeys[piece][i];
    }
  }

  if (board->side == WHITE)
  {
    key ^= SideKey;
  }

  if (board->enPassant == XX)
  {
    ASSERT(board->enPassant >= 0 && board->enPassant <= POSITION_SIZE);
    key ^= PieceKeys[EMPTY][board->enPassant];
  }

  ASSERT(board->castle >= 0 && board->castle < CASTLE_SIZE);
  key ^= CastleKeys[board->castle];
  return key;
}

//// Process ////
void resetBoard(Board* board)
{
  for (int i = 0; i < POSITION_SIZE; ++i)
  {
    board->pieceStates[i] = OB;
  }

  for (int i = 0; i < INDEX_SIZE; ++i)
  {
    board->pieceStates[IndexToPosition[i]] = EMPTY;
  }

  for (int i = 0; i < PLAYER_SIZE; ++i)
  {
    board->bigPieceCounts[i] = 0;
    board->majorPieceCounts[i] = 0;
    board->minorPieceCounts[i] = 0;
    board->pawns[i] = 0ULL;
  }

  for (int i = 0; i < PIECE_SIZE; ++i)
  {
    board->counts[i] = 0;
  }

  board->kingSquares[WHITE] = XX;
  board->kingSquares[BLACK] = XX;
  board->side = BOTH;
  board->castle = 0;
  board->enPassant = XX;
  board->fiftyMoves = 0;
  board->currentPly = 0;
  board->historyPly = 0;
  board->positionKey = 0ULL;
}

//// Print ////

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

//// Main ////

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

  // Testing with bitboard
  for (int i = 0; i < INDEX_SIZE; ++i)
  {
    printf("Index %d:\n", i);
    printBitboard(SetMask[i]);
    printf("\n");
  }

  // Return
  return 0;
}
