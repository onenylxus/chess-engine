// Include
#include "engine.h"

// Generate position key
u64 GeneratePositionKey(const Board *board)
{
  // Define variable
  u64 key = 0;

  // Pieces
  for (int i = 0; i < POSITION_SIZE; ++i)
  {
    int piece = board->pieces[i];
    if (piece != EMPTY && piece != XX)
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
  if (board->enPassant != XX)
  {
    ASSERT(board->enPassant >= 0 && board->enPassant < POSITION_SIZE);
    key ^= PieceKeys[EMPTY][board->enPassant];
  }

  // Castling
  ASSERT(board->castle >= 0 && board->castle < CASTLE_SIZE);
  key ^= CastleKeys[board->castle];

  // Return
  return key;
}
