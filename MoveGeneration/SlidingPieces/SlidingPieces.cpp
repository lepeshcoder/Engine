//
// Created by nelep on 26.11.2025.
//
#include "SlidingPieces.h"




Bitboard::Bitboard SlidingPieces::GetBishopMoves(int bishop, Bitboard::Bitboard blockers) {
    Bitboard::Bitboard moves = Bitboard::ZERO;

    Bitboard::Bitboard rayNE = Rays::GetRay(bishop,Rays::NORTH_EAST);
    Bitboard::Bitboard rayBlockers = rayNE & blockers;
    moves |= rayNE;
    if (rayBlockers) {
        moves &= ~Rays::GetRay(Bitboard::BitScanForward(rayBlockers),Rays::NORTH_EAST);;
    }

    Bitboard::Bitboard raySE = Rays::GetRay(bishop,Rays::SOUTH_EAST);
    rayBlockers = raySE & blockers;
    moves |= raySE;
    if (rayBlockers) {
        moves &= ~Rays::GetRay(Bitboard::BitScanReverse(rayBlockers),Rays::SOUTH_EAST);
    }

    Bitboard::Bitboard raySW = Rays::GetRay(bishop,Rays::SOUTH_WEST);
    rayBlockers = raySW & blockers;
    moves |= raySW;
    if (rayBlockers) {
        moves &= ~Rays::GetRay(Bitboard::BitScanReverse(rayBlockers),Rays::SOUTH_WEST);
    }

    Bitboard::Bitboard rayNW = Rays::GetRay(bishop,Rays::NORTH_WEST);
    rayBlockers = rayNW & blockers;
    moves |= rayNW;
    if (rayBlockers) {
        moves &= ~Rays::GetRay(Bitboard::BitScanForward(rayBlockers),Rays::NORTH_WEST);
    }

    return moves;
}

Bitboard::Bitboard SlidingPieces::GetRookMoves(int rook, Bitboard::Bitboard blockers) {
    Bitboard::Bitboard moves = Bitboard::ZERO;

    Bitboard::Bitboard rayN = Rays::GetRay(rook,Rays::NORTH);
    Bitboard::Bitboard rayBlockers = rayN & blockers;
    moves |= rayN;
    if (rayBlockers) {
        moves &= ~Rays::GetRay(Bitboard::BitScanForward(rayBlockers),Rays::NORTH);
    }

    Bitboard::Bitboard rayE = Rays::GetRay(rook,Rays::EAST);
    rayBlockers = rayE & blockers;
    moves |= rayE;
    if (rayBlockers) {
        moves &= ~Rays::GetRay(Bitboard::BitScanReverse(rayBlockers),Rays::EAST);
    }

    Bitboard::Bitboard rayS = Rays::GetRay(rook,Rays::SOUTH);
    rayBlockers = rayS & blockers;
    moves |= rayS;
    if (rayBlockers) {
        moves &= ~Rays::GetRay(Bitboard::BitScanReverse(rayBlockers),Rays::SOUTH);
    }

    Bitboard::Bitboard rayW = Rays::GetRay(rook,Rays::WEST);
    rayBlockers = rayW & blockers;
    moves |= rayW;
    if (rayBlockers) {
        moves &= ~Rays::GetRay(Bitboard::BitScanForward(rayBlockers),Rays::WEST);
    }

    return moves;
}

Bitboard::Bitboard SlidingPieces::GetQueenMoves(int queen, Bitboard::Bitboard blockers) {
    return GetRookMoves(queen,blockers) | GetBishopMoves(queen,blockers);
}
