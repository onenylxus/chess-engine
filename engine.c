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
int GetPosition(int file, int rank)
{
  return rank * 10 + file + 21;
}

//// Init ////

// Initialize board conversion
void InitBoardConversion()
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
      position = GetPosition(file, rank);
      IndexToPosition[index] = position;
      PositionToIndex[position] = index;
      index++;
    }
  }
}

// Initalize bit masks
void InitBitMask()
{
  for (int i = 0; i < INDEX_SIZE; ++i)
  {
    SetMask[i] = (1ULL << i);
    ClearMask[i] = ~SetMask[i];
  }
}

// Initalize hash keys
void InitHashKeys()
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
void Init()
{
  InitBoardConversion();
  InitBitMask();
  InitHashKeys();
}

//// Bitboard ////

// Pop bit
int PopBit(u64* bb)
{
  u64 b = *bb ^ (*bb - 1);
  unsigned int fold = (unsigned int)((b & 0xffffffff) ^ (b >> 32));
  *bb &= *bb - 1;
  return MagicBitboard[(fold * 0x783a9b23) >> 26];
}

// Set bit
void SetBit(u64* bb, int index)
{
  *bb |= SetMask[index];
}

// Clear bit
void ClearBit(u64* bb, int index)
{
  *bb &= ClearMask[index];
}

// Count bit
int CountBit(u64 bit)
{
  int count = 0;
  while (bit)
  {
    count++;
    bit &= bit - 1;
  }
  return count;
}

//// Hash ////
u64 GeneratePositionKey(const Board* board)
{
  u64 key = 0;

  // Pieces
  for (int i = 0; i < POSITION_SIZE; ++i)
  {
    int piece = board->pieces[i];
    if (piece != XX && piece != OB && piece != EMPTY)
    {
      ASSERT(piece >= WHITE_PAWN && piece <= BLACK_KING);
      key ^= PieceKeys[piece][i];
    }
  }

  // Side
  if (board->side == WHITE)
  {
    key ^= SideKey;
  }

  // En passant
  if (board->enPassant == XX)
  {
    ASSERT(board->enPassant >= 0 && board->enPassant <= POSITION_SIZE);
    key ^= PieceKeys[EMPTY][board->enPassant];
  }

  // Castle
  ASSERT(board->castle >= 0 && board->castle < CASTLE_SIZE);
  key ^= CastleKeys[board->castle];

  return key;
}

//// Process ////
void ResetBoard(Board* board)
{
  for (int i = 0; i < POSITION_SIZE; ++i)
  {
    board->pieces[i] = OB;
  }

  for (int i = 0; i < INDEX_SIZE; ++i)
  {
    board->pieces[IndexToPosition[i]] = EMPTY;
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

//// Forsyth–Edwards Notation (FEN) ////
int ParseFen(char* fen, Board* board)
{
  ASSERT(fen != NULL);
  ASSERT(board != NULL);

  int rank = RANK_8;
  int file = FILE_A;
  int piece = 0;
  int count = 0;

  ResetBoard(board);

  // Piece states
  while (rank >= RANK_1 && *fen)
  {
    count = 1;
    switch (*fen)
    {
      // Pieces
      case 'p': piece = BLACK_PAWN; break;
      case 'r': piece = BLACK_ROOK; break;
      case 'n': piece = BLACK_KNIGHT; break;
      case 'b': piece = BLACK_BISHOP; break;
      case 'q': piece = BLACK_QUEEN; break;
      case 'k': piece = BLACK_KING; break;
      case 'P': piece = WHITE_PAWN; break;
      case 'R': piece = WHITE_ROOK; break;
      case 'N': piece = WHITE_KNIGHT; break;
      case 'B': piece = WHITE_BISHOP; break;
      case 'Q': piece = WHITE_QUEEN; break;
      case 'K': piece = WHITE_KING; break;

      // Empties
      case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8':
        piece = EMPTY;
        count = *fen - '0';
        break;

      // New rank
      case '/': case ' ':
        rank--;
        file = FILE_A;
        fen++;
        continue;

      // Default
      default:
        printf("FEN syntax error\n");
        return -1;
    }

    for (int i = 0; i < count; ++i)
    {
      int index = rank * 8 + file;
      int position = IndexToPosition[index];
      if (piece != EMPTY)
      {
        board->pieces[position] = piece;
      }
      file++;
    }
    fen++;
  }

  // Side
  ASSERT(*fen == 'w' || *fen == 'b');
  board->side = *fen == 'w' ? WHITE : BLACK;
  fen += 2;

  // Castle
  for (int i = 0; i < 4; ++i)
  {
    if (*fen == ' ')
    {
      break;
    }

    switch (*fen)
    {
      case 'K': board->castle |= CASTLE_WHITE_KING; break;
      case 'Q': board->castle |= CASTLE_WHITE_QUEEN; break;
      case 'k': board->castle |= CASTLE_BLACK_KING; break;
      case 'q': board->castle |= CASTLE_BLACK_QUEEN; break;
    }
    fen++;
  }
  fen++;
  ASSERT(board->castle >= 0 && board->castle < CASTLE_SIZE);

  // En passant
  if (*fen != '-')
  {
    file = fen[0] - 'a';
    rank = fen[1] - '1';

    ASSERT(file >= FILE_A && file <= FILE_H);
    ASSERT(rank >= RANK_1 && rank <= RANK_8);
    board->enPassant = GetPosition(file, rank);
  }

  // Finalize
  board->positionKey = GeneratePositionKey(board);
  return 0;
}

//// Print ////

// Print position board
void PrintPositionBoard()
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
void PrintIndexBoard()
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
void PrintBitboard(u64 key)
{
  int position = 0;
  int index = 0;

  printf("Bitboard:\n");
  for (int rank = RANK_8; rank >= RANK_1; --rank)
  {
    for (int file = FILE_A; file <= FILE_H; ++file)
    {
      position = GetPosition(file, rank);
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
  Init();

  // Print boards
  printf("\n\n");
  PrintPositionBoard();
  printf("\n\n");
  PrintIndexBoard();
  printf("\n\n");

  // Testing with bitboard
  for (int i = 0; i < INDEX_SIZE; ++i)
  {
    printf("Index %d:\n", i);
    PrintBitboard(SetMask[i]);
    printf("\n");
  }

  // Return
  return 0;
}
