//
// Created by nelep on 25.11.2025.
//

#ifndef ENGINE_NONSLIDINGPIECES_H
#define ENGINE_NONSLIDINGPIECES_H

#include "../../Bitboard/Bitboard.h"
#include "../../Types.h"

namespace NonSlidingPieces {


    Bitboard::Bitboard GetPawnAttacks(Bitboard::Bitboard pawns, PieceColor color);

    Bitboard::Bitboard GetPawnMoves(Bitboard::Bitboard pawns, PieceColor color);

    Bitboard::Bitboard GetKnightMoves(Bitboard::Bitboard knights);

    Bitboard::Bitboard GetKingMoves(Bitboard::Bitboard king);
}

#endif //ENGINE_NONSLIDINGPIECES_H