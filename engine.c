// Include
#include <stdio.h>
#include "engine.h"

//// Variables ////

// Conversion arrays
int PositionToIndex[POSITION_SIZE];
int PositionToRank[POSITION_SIZE];
int PositionToFile[POSITION_SIZE];
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

// Characters
const char PieceCharacters[] = ".PNBRQKpnbrqk";
const char SideCharacters[] = "wb-";
const char RankCharacters[] = "12345678";
const char FileCharacters[] = "abcdefgh";

// Pieces
const int BigPieces[PIECE_SIZE] = { FALSE, FALSE, TRUE, TRUE, TRUE, TRUE, TRUE, FALSE, TRUE, TRUE, TRUE, TRUE, TRUE };
const int MajorPieces[PIECE_SIZE] = { FALSE, FALSE, FALSE, FALSE, TRUE, TRUE, TRUE, FALSE, FALSE, FALSE, TRUE, TRUE, TRUE };
const int MinorPieces[PIECE_SIZE] = { FALSE, FALSE, TRUE, TRUE, FALSE, FALSE, FALSE, FALSE, TRUE, TRUE, FALSE, FALSE, FALSE };
const int PieceValues[PIECE_SIZE] = { 0, 100, 325, 325, 550, 1000, 50000, 100, 325, 325, 550, 1000, 50000 };
const int PieceColors[PIECE_SIZE] = { FALSE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK };

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
    PositionToRank[i] = RANK_X;
    PositionToFile[i] = FILE_X;
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
      PositionToIndex[position] = index;
      PositionToRank[position] = rank;
      PositionToFile[position] = file;
      IndexToPosition[index] = position;
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

  for (int i = 0; i < SIDE_SIZE; ++i)
  {
    board->pawns[i] = 0ULL;
    board->bigPieces[i] = 0;
    board->majorPieces[i] = 0;
    board->minorPieces[i] = 0;
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

int CheckBoard(const Board* board)
{
  int counts[PIECE_SIZE] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
  int bigPieces[SIDE_SIZE] = { 0, 0 };
  int majorPieces[SIDE_SIZE] = { 0, 0 };
  int minorPieces[SIDE_SIZE] = { 0, 0 };
  int materials[SIDE_SIZE] = { 0, 0 };

  u64 pawns[PLAYER_SIZE] = { 0ULL, 0ULL, 0ULL };
  pawns[WHITE] = board->pawns[WHITE];
  pawns[BLACK] = board->pawns[BLACK];
  pawns[BOTH] = board->pawns[BOTH];

  // Check piece positions
  for (int i = WHITE_PAWN; i <= BLACK_KING; ++i)
  {
    for (int j = 0; j < board->counts[i]; ++j)
    {
      int position = board->pieceList[i][j];
      ASSERT(board->pieces[position] == i);
    }
  }

  // Update temporary counters
  for (int i = 0; i < INDEX_SIZE; ++i)
  {
    int position = IndexToPosition[i];
    int piece = board->pieces[position];
    int color = PieceColors[piece];

    counts[piece]++;
    if (BigPieces[piece] == TRUE)
    {
      bigPieces[color]++;
    }
    if (MajorPieces[piece] == TRUE)
    {
      majorPieces[color]++;
    }
    if (MinorPieces[piece] == TRUE)
    {
      minorPieces[color]++;
    }
    materials[color] += PieceValues[piece];
  }

  // Check piece counts
  for (int i = WHITE_PAWN; i < BLACK_KING; ++i)
  {
    ASSERT(counts[i] == board->counts[i]);
  }

  // Check bitboards
  int bits = 0;
  bits = CountBit(pawns[WHITE]);
  ASSERT(bits == board->counts[WHITE_PAWN]);
  bits = CountBit(pawns[BLACK]);
  ASSERT(bits == board->counts[BLACK_PAWN]);
  bits = CountBit(pawns[BOTH]);
  ASSERT(bits == board->counts[WHITE_PAWN] + board->counts[BLACK_PAWN]);

  int index = 0;
  int piece = EMPTY;
  while (pawns[WHITE])
  {
    index = PopBit(&pawns[WHITE]);
    piece = board->pieces[IndexToPosition[index]];
    ASSERT(piece == WHITE_PAWN);
  }
  while (pawns[BLACK])
  {
    index = PopBit(&pawns[BLACK]);
    piece = board->pieces[IndexToPosition[index]];
    ASSERT(piece == BLACK_PAWN);
  }
  while (pawns[BOTH])
  {
    index = PopBit(&pawns[BOTH]);
    piece = board->pieces[IndexToPosition[index]];
    ASSERT(piece == WHITE_PAWN || piece == BLACK_PAWN);
  }

  // Check materials
  ASSERT(bigPieces[WHITE] == board->bigPieces[WHITE]);
  ASSERT(bigPieces[BLACK] == board->bigPieces[BLACK]);
  ASSERT(majorPieces[WHITE] == board->majorPieces[WHITE]);
  ASSERT(majorPieces[BLACK] == board->majorPieces[BLACK]);
  ASSERT(minorPieces[WHITE] == board->minorPieces[WHITE]);
  ASSERT(minorPieces[BLACK] == board->minorPieces[BLACK]);
  ASSERT(materials[WHITE] == board->materials[WHITE]);
  ASSERT(materials[BLACK] == board->materials[BLACK]);

  // Check general information
  ASSERT(board->side == WHITE || board->side == BLACK);
  ASSERT(GeneratePositionKey(board) == board->positionKey);

  int isWhiteEnPassant = board->side == WHITE && PositionToRank[board->enPassant] == RANK_6;
  int isBlackEnPassant = board->side == BLACK && PositionToRank[board->enPassant] == RANK_3;
  ASSERT(board->enPassant == XX || isWhiteEnPassant || isBlackEnPassant);

  ASSERT(board->pieces[board->kingSquares[WHITE]] == WHITE_KING);
  ASSERT(board->pieces[board->kingSquares[BLACK]] == BLACK_KING);

  // Return
  return TRUE;
}

//// Material ////
void UpdateMaterial(Board *board)
{
  for (int i = 0; i < POSITION_SIZE; ++i)
  {
    int piece = board->pieces[i];
    if (piece != OB && piece != EMPTY)
    {
      int color = PieceColors[piece];

      // Piece type
      if (BigPieces[piece])
      {
        board->bigPieces[color]++;
      }
      if (MajorPieces[piece])
      {
        board->majorPieces[color]++;
      }
      if (MinorPieces[piece])
      {
        board->minorPieces[color]++;
      }

      // Material and count
      board->materials[color] += PieceValues[piece];
      board->pieceList[piece][board->counts[piece]] = i;
      board->counts[piece]++;

      // Kings
      if (piece == WHITE_KING)
      {
        board->kingSquares[WHITE] = i;
      }
      if (piece == BLACK_KING)
      {
        board->kingSquares[BLACK] = i;
      }

      // Pawns
      if (piece == WHITE_PAWN)
      {
        SetBit(&board->pawns[WHITE], PositionToIndex[i]);
        SetBit(&board->pawns[BOTH], PositionToIndex[i]);
      }
      if (piece == BLACK_PAWN)
      {
        SetBit(&board->pawns[BLACK], PositionToIndex[i]);
        SetBit(&board->pawns[BOTH], PositionToIndex[i]);
      }
    }
  }
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
  UpdateMaterial(board);
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

// Print board
void PrintBoard(const Board* board)
{
  printf("Game board:\n\n");

  // Board
  for (int rank = RANK_8; rank >= RANK_1; --rank)
  {
    printf("%c ", RankCharacters[rank]);
    for (int file = FILE_A; file <= FILE_H; ++file)
    {
      int index = rank * 8 + file;
      int position = IndexToPosition[index];
      int piece = board->pieces[position];
      printf("%3c", PieceCharacters[piece]);
    }
    printf("\n");
  }
  printf("\n  ");
  for (int file = FILE_A; file <= FILE_H; ++file)
  {
    printf("%3c", FileCharacters[file]);
  }
  printf("\n\n");

  // Statistics
  char castleWhiteKing = board->castle & CASTLE_WHITE_KING ? 'K' : '-';
  char castleWhiteQueen = board->castle & CASTLE_WHITE_QUEEN ? 'Q' : '-';
  char castleBlackKing = board->castle & CASTLE_BLACK_KING ? 'k' : '-';
  char castleBlackQueen = board->castle & CASTLE_BLACK_QUEEN ? 'q' : '-';

  printf("Side: %c\n", SideCharacters[board->side]);
  printf("En passant: %d\n", board->enPassant);
  printf("Castle: %c%c%c%c\n", castleWhiteKing, castleWhiteQueen, castleBlackKing, castleBlackQueen);
  printf("Position key: %llX\n", board->positionKey);
}

//// Main ////
#define FEN_TEST "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1"

// Main function
int main(int argc, char* argv[])
{
  // Initialize engine
  Init();

  // Print game board
  Board board[1];
  ParseFen(FEN_TEST, board);
  PrintBoard(board);
  ASSERT(CheckBoard(board));

  // Return
  return 0;
}
