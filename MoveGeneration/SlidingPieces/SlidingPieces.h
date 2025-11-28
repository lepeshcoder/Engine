//
// Created by nelep on 26.11.2025.
//

#ifndef ENGINE_SLIDINGPIECES_H
#define ENGINE_SLIDINGPIECES_H
#include "../../Bitboard/Bitboard.h"
#include "../../Rays/Rays.h"

namespace SlidingPieces {

    Bitboard::Bitboard GetBishopMoves(int bishop, Bitboard::Bitboard blockers);

    Bitboard::Bitboard GetRookMoves(int rook, Bitboard::Bitboard blockers);

    Bitboard::Bitboard GetQueenMoves(int queen, Bitboard::Bitboard blockers);



}

#endif //ENGINE_SLIDINGPIECES_H