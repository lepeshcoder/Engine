//
// Created by nelep on 27.11.2025.
//

#include "Cache.h"

#include <iostream>

#include "../NonSlidingPieces/NonSlidingPieces.h"

 Bitboard::Bitboard* Cache::RookCache[64];
 Bitboard::Bitboard* Cache::BishopCache[64];
 Bitboard::Bitboard Cache::KingCache[64];
 Bitboard::Bitboard Cache::KnightCache[64];

 Bitboard::Bitboard* Cache::PawnMoveCache[2][64];
 Bitboard::Bitboard Cache::PawnAttackCache[2][64];

 Bitboard::Bitboard Cache::RookMasks[64];
 Bitboard::Bitboard Cache::BishopMasks[64];
 Bitboard::Bitboard Cache::PawnMoveMasks[2][64];

int Cache::RookBits[64] = {
    12,11,11,11,11,11,11,12,
    11,10,10,10,10,10,10,11,
    11,10,10,10,10,10,10,11,
    11,10,10,10,10,10,10,11,
    11,10,10,10,10,10,10,11,
    11,10,10,10,10,10,10,11,
    11,10,10,10,10,10,10,11,
    12,11,11,11,11,11,11,12,
};

int Cache::BishopBits[64] = {
    6, 5, 5, 5, 5, 5, 5, 6,
    5, 5, 5, 5, 5, 5, 5, 5,
    5, 5, 7, 7, 7, 7, 5, 5,
    5, 5, 7, 9, 9, 7, 5, 5,
    5, 5, 7, 9, 9, 7, 5, 5,
    5, 5, 7, 7, 7, 7, 5, 5,
    5, 5, 5, 5, 5, 5, 5, 5,
    6, 5, 5, 5, 5, 5, 5, 6
};

void Cache::InitCache() {
    InitMasks();
    InitBishopCache();
    InitRookCache();
    InitKnightCache();
    InitKingCache();
    InitPawnCache();
}

void Cache::ClearCache() {
    for (int i = 0; i < 64; i++) {
        delete[] BishopCache[i];
        delete[] RookCache[i];
        delete[] PawnMoveCache[WHITE][i];
        delete[] PawnMoveCache[BLACK][i];
    }
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

        if (i / 8 == 1) {
            PawnMoveMasks[WHITE][i] = Bitboard::ONE << i + 8 | Bitboard::ONE << i + 16;
        }
        else {
            PawnMoveMasks[WHITE][i] = i > 55 ? Bitboard::ZERO : Bitboard::ONE << (i + 8);
        }
        if (i / 8 == 6) {
            PawnMoveMasks[BLACK][i] = Bitboard::ONE << i - 8 | Bitboard::ONE << i - 16;
        }
        else {
            PawnMoveMasks[BLACK][i] = i < 8 ? Bitboard::ZERO : Bitboard::ONE << i - 8;
        }
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

void Cache::InitPawnCache() {
    for (int sq = 0; sq < 64; sq++ ) {
        PawnAttackCache[WHITE][sq] = NonSlidingPieces::GetPawnAttacks(Bitboard::ONE << sq,WHITE);
        PawnAttackCache[BLACK][sq] = NonSlidingPieces::GetPawnAttacks(Bitboard::ONE << sq,BLACK);
        int newWhiteSize = sq / 8 == 1 ? 4 : 2;
        int newBlackSize = sq / 8 == 6 ? 4 : 2;
        PawnMoveCache[WHITE][sq] = new Bitboard::Bitboard[newWhiteSize];
        PawnMoveCache[BLACK][sq] = new Bitboard::Bitboard[newBlackSize];
        // for white
        for (int i = 0; i < newWhiteSize; i++) {
            if (sq / 8 == 1) {
                PawnMoveCache[WHITE][sq][0] = Bitboard::ONE << sq + 8 | Bitboard::ONE << sq + 16;
                PawnMoveCache[WHITE][sq][1] = Bitboard::ZERO;
                PawnMoveCache[WHITE][sq][2] = Bitboard::ONE << sq + 8;
                PawnMoveCache[WHITE][sq][3] = Bitboard::ZERO;
            }
            else {
                PawnMoveCache[WHITE][sq][0] = sq > 55 ? Bitboard::ZERO : Bitboard::ONE << sq + 8;
                PawnMoveCache[WHITE][sq][1] = Bitboard::ZERO;
            }
        }
        // for black
        for (int i = 0; i < newBlackSize; i++) {
            if (sq / 8 == 6) {
                PawnMoveCache[BLACK][sq][0] = Bitboard::ONE << sq - 8 | Bitboard::ONE << sq - 16;
                PawnMoveCache[BLACK][sq][1] = Bitboard::ONE << sq - 8;
                PawnMoveCache[BLACK][sq][2] = Bitboard::ZERO;
                PawnMoveCache[BLACK][sq][3] = Bitboard::ZERO;
            }
            else {
                PawnMoveCache[BLACK][sq][0] = sq < 8 ? Bitboard::ZERO : Bitboard::ONE << sq - 8;
                PawnMoveCache[BLACK][sq][1] = Bitboard::ZERO;
            }
        }
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

Bitboard::Bitboard Cache::GetPawnMoves(int color, int sq, Bitboard::Bitboard blockers) {
    Bitboard::Bitboard hashBlockers = _pext_u64(blockers & PawnMoveMasks[color][sq], PawnMoveMasks[color][sq]);
    return PawnMoveCache[color][sq][hashBlockers];
}

Bitboard::Bitboard Cache::GetPawnAttacks(int color, int sq) {
    return PawnAttackCache[color][sq];
}

Bitboard::Bitboard Cache::GetSlidingPieceAttacks(int pieceType, int sq, Bitboard::Bitboard blockers) {
    switch (pieceType) {
        case BISHOP: return GetBishopMoves(sq,blockers);
        case ROOK: return GetRookMoves(sq,blockers);
        case QUEEN: return GetQueenMoves(sq,blockers);
    }
}

