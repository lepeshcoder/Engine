//
// Created by nelep on 25.11.2025.
//

#ifndef ENGINE_RAYS_H
#define ENGINE_RAYS_H
#include "../Bitboard/Bitboard.h"


class Rays {

private:
    static Bitboard::Bitboard rays[64][8];

    static Bitboard::Bitboard DiagLeftShift(Bitboard::Bitboard bb, int num);

    static Bitboard::Bitboard DiagRightShift(Bitboard::Bitboard bb, int num);

    static int Col(int sq) {
        return sq % 8;
    }

    static int Row(int sq) {
        return sq / 8;
    }

public:

    enum DIRECTION{NORTH, NORTH_EAST, EAST, SOUTH_EAST, SOUTH, SOUTH_WEST, WEST, NORTH_WEST};

    static Bitboard::Bitboard GetRay(int sq, DIRECTION dir);

    static void Init();

};


#endif //ENGINE_RAYS_H