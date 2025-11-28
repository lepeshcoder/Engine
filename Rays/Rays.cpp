//
// Created by nelep on 25.11.2025.
//

#include "Rays.h"

Bitboard::Bitboard Rays::rays[64][8] = {};

void Rays::Init() {
    for (int i = 0; i < 64; i++) {
        rays[i][NORTH] = (Bitboard::FILE_H & ~Bitboard::RANK_1) << i;
        rays[i][SOUTH] = (Bitboard::FILE_A & ~Bitboard::RANK_8) >> (63 - i);
        rays[i][EAST] = (Bitboard::ONE << i) - (Bitboard::ONE << (i & 56));
        rays[i][WEST] = ((Bitboard::ONE << (i | 7)) - (Bitboard::ONE << i)) << 1;

        rays[i][NORTH_EAST] = DiagRightShift(DiagLeftShift(Bitboard::DIAG_A1_H8,Row(i) + 1), 8 - Col(i));
        rays[i][SOUTH_WEST] = DiagLeftShift(DiagRightShift(Bitboard::DIAG_A1_H8,8 - Row(i)), Col(i) + 1);
        rays[i][NORTH_WEST] = DiagLeftShift(DiagRightShift(Bitboard::DIAG_A8_H1,Row(i) + 1), Col(i) + 1);
        rays[i][SOUTH_EAST] = DiagRightShift(DiagLeftShift(Bitboard::DIAG_A8_H1, 8 - Row(i)),8 - Col(i));
    }
}


Bitboard::Bitboard Rays::DiagLeftShift(Bitboard::Bitboard bb, int num) {
    for (int i = 0; i < num; i++) {
        bb = bb << 1 & ~Bitboard::FILE_H;
    }
    return bb;
}

Bitboard::Bitboard Rays::DiagRightShift(Bitboard::Bitboard bb, int num) {
    for (int i = 0; i < num; i++) {
        bb = bb >> 1 & ~Bitboard::FILE_A;
    }
    return bb;
}

Bitboard::Bitboard Rays::GetRay(int sq, DIRECTION dir) {
    return rays[sq][dir];
}



