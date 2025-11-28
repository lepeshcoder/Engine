//
// Created by nelep on 27.11.2025.
//

#include "Cache.h"

#include <iostream>

#include "../NonSlidingPieces/NonSlidingPieces.h"

void Cache::InitCache() {
    InitMasks();
    InitBishopCache();
    InitRookCache();
    InitKnightCache();
    InitKingCache();
}


void Cache::InitMasks() {
    Rays::Init();
    for (int i = 0; i < 64; i++) {
        BishopMasks[i] = (Rays::GetRay(i,Rays::NORTH_EAST) | Rays::GetRay(i,Rays::SOUTH_EAST) |
            Rays::GetRay(i,Rays::SOUTH_WEST) | Rays::GetRay(i,Rays::NORTH_WEST)) & ~Bitboard::EDGES;

        RookMasks[i] = Rays::GetRay(i,Rays::NORTH) & ~Bitboard::RANK_8 |
            Rays::GetRay(i,Rays::SOUTH) & ~Bitboard::RANK_1 |
            Rays::GetRay(i,Rays::EAST) & ~Bitboard::FILE_H |
            Rays::GetRay(i,Rays::WEST) & ~Bitboard::FILE_A;
    }
}

void Cache::InitBishopCache() {
    for (int sq = 0; sq < 64; sq++ ) {
        BishopCache[sq] = new Bitboard::Bitboard[1 << BishopBits[sq]];
        for (int hashBlockers = 0; hashBlockers < 1 << BishopBits[sq]; hashBlockers++) {
            Bitboard::Bitboard trueBlockers = _pdep_u64(hashBlockers,BishopMasks[sq]);
            BishopCache[sq][hashBlockers] = SlidingPieces::GetBishopMoves(sq,trueBlockers);
        }
    }
}

void Cache::InitRookCache() {
    for (int sq = 0; sq < 64; sq++ ) {
        RookCache[sq] = new Bitboard::Bitboard[1 << RookBits[sq]];
        for (int hashBlockers = 0; hashBlockers < 1 << RookBits[sq]; hashBlockers++) {
            Bitboard::Bitboard trueBlockers = _pdep_u64(hashBlockers,RookMasks[sq]);
            RookCache[sq][hashBlockers] = SlidingPieces::GetRookMoves(sq,trueBlockers);
        }
    }
}

void Cache::InitKingCache() {
    for (int sq = 0; sq < 64; sq++ ) {
        KingCache[sq] = NonSlidingPieces::GetKingMoves(Bitboard::ONE << sq);
    }
}

void Cache::InitKnightCache() {
    for (int sq = 0; sq < 64; sq++) {
        KnightCache[sq] = NonSlidingPieces::GetKnightMoves(Bitboard::ONE << sq);
    }
}

Cache::Cache() {
    InitCache();
}

Cache::~Cache() {
    for (int i = 0; i < 64; i++) {
        delete BishopCache[i];
        delete RookCache[i];
    }
}

Bitboard::Bitboard Cache::GetBishopMoves(int sq, Bitboard::Bitboard blockers) {
    Bitboard::Bitboard key = _pext_u64(blockers & BishopMasks[sq],BishopMasks[sq]);
    return BishopCache[sq][key];
}

Bitboard::Bitboard Cache::GetRookMoves(int sq, Bitboard::Bitboard blockers) {
    Bitboard::Bitboard key = _pext_u64(blockers & RookMasks[sq],RookMasks[sq]);
    return RookCache[sq][key];
}

Bitboard::Bitboard Cache::GetQueenMoves(int sq, Bitboard::Bitboard blockers) {
    return GetRookMoves(sq,blockers) | GetBishopMoves(sq,blockers);
}

Bitboard::Bitboard Cache::GetKingMoves(int sq) {
    return KingCache[sq];
}

Bitboard::Bitboard Cache::GetKnightMoves(int sq) {
    return KnightCache[sq];
}
