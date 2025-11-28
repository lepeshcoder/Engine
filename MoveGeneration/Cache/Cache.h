//
// Created by nelep on 27.11.2025.
//

#ifndef ENGINE_CACHE_H
#define ENGINE_CACHE_H
#include "../../Bitboard/Bitboard.h"
#include"../../Rays/Rays.h"
#include<immintrin.h>
#include"../SlidingPieces/SlidingPieces.h"


class Cache {

public:

    Cache();
    ~Cache();

    Bitboard::Bitboard GetBishopMoves(int sq, Bitboard::Bitboard blockers);
    Bitboard::Bitboard GetRookMoves(int sq, Bitboard::Bitboard blockers);
    Bitboard::Bitboard GetQueenMoves(int sq, Bitboard::Bitboard blockers);

    Bitboard::Bitboard GetKingMoves(int sq);
    Bitboard::Bitboard GetKnightMoves(int sq);


private:
     void InitCache();
     void InitMasks();

     void InitBishopCache();
     void InitRookCache();
     void InitKingCache();
     void InitKnightCache();

     Bitboard::Bitboard* RookCache[64];
     Bitboard::Bitboard* BishopCache[64];
     Bitboard::Bitboard KingCache[64];
     Bitboard::Bitboard KnightCache[64];


     Bitboard::Bitboard RookMasks[64] = {};
     Bitboard::Bitboard BishopMasks[64] = {};

     int RookBits[64] = {
        12,11,11,11,11,11,11,12,
        11,10,10,10,10,10,10,11,
        11,10,10,10,10,10,10,11,
        11,10,10,10,10,10,10,11,
        11,10,10,10,10,10,10,11,
        11,10,10,10,10,10,10,11,
        11,10,10,10,10,10,10,11,
        12,11,11,11,11,11,11,12,
    };

    int BishopBits[64] = {
        6, 5, 5, 5, 5, 5, 5, 6,
        5, 5, 5, 5, 5, 5, 5, 5,
        5, 5, 7, 7, 7, 7, 5, 5,
        5, 5, 7, 9, 9, 7, 5, 5,
        5, 5, 7, 9, 9, 7, 5, 5,
        5, 5, 7, 7, 7, 7, 5, 5,
        5, 5, 5, 5, 5, 5, 5, 5,
        6, 5, 5, 5, 5, 5, 5, 6
    };

};


#endif //ENGINE_CACHE_H