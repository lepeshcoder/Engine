//
// Created by nelep on 27.11.2025.
//

#ifndef ENGINE_CACHE_H
#define ENGINE_CACHE_H
#include "../../Bitboard/Bitboard.h"
#include"../../Rays/Rays.h"
#include<immintrin.h>
#include"../SlidingPieces/SlidingPieces.h"
#include <iostream>
#include "../NonSlidingPieces/NonSlidingPieces.h"
#include <random>


class Cache {

public:

    static void InitCache();
    static void ClearCache();

    static Bitboard::Bitboard GetBishopMoves(int sq, Bitboard::Bitboard blockers);
    static Bitboard::Bitboard GetRookMoves(int sq, Bitboard::Bitboard blockers);
    static Bitboard::Bitboard GetQueenMoves(int sq, Bitboard::Bitboard blockers);

    static Bitboard::Bitboard GetKingMoves(int sq);
    static Bitboard::Bitboard GetKnightMoves(int sq);
    static Bitboard::Bitboard GetPawnMoves(int color, int sq, Bitboard::Bitboard blockers);
    static Bitboard::Bitboard GetPawnAttacks(int color, int sq);
    static Bitboard::Bitboard GetSlidingPieceAttacks(int pieceType,int sq,Bitboard::Bitboard blockers);
    static Bitboard::Bitboard GetLineBb(int from,int to);
    static Bitboard::Bitboard GetBetweenBb(int from,int to);
    static Bitboard::Bitboard GetPieceMoves(uint32_t pieceType,uint32_t sq, Bitboard::Bitboard blockers);

    static Key GetZobristTable(uint8_t color, uint8_t pieceType, uint8_t sq);
    static Key GetWhiteZobristSideMove();
    static Key GetZobristEnPassant(uint8_t enPassantFile);
    static Key GetZobristCastleRights(uint8_t castleRights);

    


private:
    
     static constexpr uint64_t ZobristSeed = 0x123456789ABCDEFull;

     static void InitMasks();

     static void InitBishopCache();
     static void InitRookCache();
     static void InitKingCache();
     static void InitKnightCache();
     static void InitPawnCache();
     static void InitLinesBb();
     static void InitBetweenBb();
     static void InitZobrist();

     static Key ZobristTable[2][6][64];
     static Key ZobristCastleRights[16];
     static Key ZobristEnPassant[8];
     static Key ZobristWhiteSideToMove;
     
     static Bitboard::Bitboard LineBb[64][64];
     static Bitboard::Bitboard BetweenBb[64][64];
     
     static Bitboard::Bitboard* RookCache[64];
     static Bitboard::Bitboard* BishopCache[64];
     static Bitboard::Bitboard KingCache[64];
     static Bitboard::Bitboard KnightCache[64];

     static Bitboard::Bitboard* PawnMoveCache[2][64];
     static Bitboard::Bitboard PawnAttackCache[2][64];

     static Bitboard::Bitboard RookMasks[64];
     static Bitboard::Bitboard BishopMasks[64];
     static Bitboard::Bitboard PawnMoveMasks[2][64];



     static int RookBits[64];
     static int BishopBits[64];

};


#endif //ENGINE_CACHE_H