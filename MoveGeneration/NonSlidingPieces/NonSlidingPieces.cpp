//
// Created by nelep on 25.11.2025.
//

#include "NonSlidingPieces.h"


Bitboard::Bitboard NonSlidingPieces::GetPawnAttacks(Bitboard::Bitboard pawns, PieceColor color) {
    switch (color){
        case PieceColor::WHITE:
            return pawns << 9 & ~Bitboard::FILE_H |
                   pawns << 7 & ~Bitboard::FILE_A;
        case PieceColor::BLACK:
            return pawns >> 9 & ~Bitboard::FILE_A |
                   pawns >> 7 & ~Bitboard::FILE_H;
    }
}

Bitboard::Bitboard NonSlidingPieces::GetPawnMoves(Bitboard::Bitboard pawns, PieceColor color) {
    switch (color) {
        case WHITE: {
            return pawns << 8 |
                (pawns & Bitboard::RANK_2) << 16;
        }
        case BLACK: {
            return pawns >> 8 |
                (pawns & Bitboard::RANK_7) >> 16;
        }
    }
}

Bitboard::Bitboard NonSlidingPieces::GetKnightMoves(Bitboard::Bitboard knights) {
    Bitboard::Bitboard l1 = knights << 1 & ~Bitboard::FILE_H;
    Bitboard::Bitboard r1 = knights >> 1 & ~Bitboard::FILE_A;
    Bitboard::Bitboard l2 = knights << 2 & ~(Bitboard::FILE_G | Bitboard::FILE_H);
    Bitboard::Bitboard r2 = knights >> 2 & ~(Bitboard::FILE_A | Bitboard::FILE_B);
    return (l1 | r1) >> 16 |
           (l1 | r1) << 16 |
           (l2 | r2) << 8 |
           (l2 | r2) >> 8;
}

Bitboard::Bitboard NonSlidingPieces::GetKingMoves(Bitboard::Bitboard king) {
    return (king << 1 | king << 9 | king >> 7) & ~Bitboard::FILE_H | // left
           (king >> 1 | king >> 9 | king << 7) & ~Bitboard::FILE_A | // right
           (king << 8 | king >> 8); // up and down
}
