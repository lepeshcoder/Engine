//
// Created by nelep on 25.11.2025.
//

#ifndef ENGINE_BITBOARD_H
#define ENGINE_BITBOARD_H

#include <string>
#include <cstdint>

namespace Bitboard {
    typedef uint64_t Bitboard;

    enum class BitOrder {
        LSB_first,
        MSB_first
    };

    // smallest index of set(1) bit
    inline uint8_t BitScanForward(Bitboard bb) {
        return _tzcnt_u64(bb);
    }

    // largest index of set(1) bit
    inline uint8_t BitScanReverse(Bitboard bb) {
        //return 63 - __builtin_clzll(bb);
        unsigned long index;
        _BitScanReverse64(&index, bb);
        return static_cast<int>(index);
    }

    inline uint8_t PopCount(Bitboard bb) {
        return __popcnt64(bb);
    }

    std::string BitboardToString(Bitboard bb, BitOrder order = BitOrder::MSB_first);

    constexpr Bitboard ZERO = 0ull;
    constexpr Bitboard ONE = 1ull;
    constexpr Bitboard ALLONE = UINT64_MAX;

    constexpr Bitboard RANK_1 = (1ull << 8) - 1;
    constexpr Bitboard RANK_2 = RANK_1 << 8;
    constexpr Bitboard RANK_3 = RANK_2 << 8;
    constexpr Bitboard RANK_4 = RANK_3 << 8;
    constexpr Bitboard RANK_5 = RANK_4 << 8;
    constexpr Bitboard RANK_6 = RANK_5 << 8;
    constexpr Bitboard RANK_7 = RANK_6 << 8;
    constexpr Bitboard RANK_8 = RANK_7 << 8;

    constexpr Bitboard FILE_A = 0x8080808080808080ull;
    constexpr Bitboard FILE_B = FILE_A >> 1;
    constexpr Bitboard FILE_C = FILE_B >> 1;
    constexpr Bitboard FILE_D = FILE_C >> 1;
    constexpr Bitboard FILE_E = FILE_D >> 1;
    constexpr Bitboard FILE_F = FILE_E >> 1;
    constexpr Bitboard FILE_G = FILE_F >> 1;
    constexpr Bitboard FILE_H = FILE_G >> 1;

    constexpr Bitboard DIAG_A1_H8 = 0x0102040810204080;
    constexpr Bitboard DIAG_A8_H1 = 0x8040201008040201;

    constexpr Bitboard EDGES = RANK_1 | RANK_8 | FILE_A | FILE_H;

    enum sq {
        H1,G1,F1,E1,D1,C1,B1,A1,
        H2,G2,F2,E2,D2,C2,B2,A2,
        H3,G3,F3,E3,D3,C3,B3,A3,
        H4,G4,F4,E4,D4,C4,B4,A4,
        H5,G5,F5,E5,D5,C5,B5,A5,
        H6,G6,F6,E6,D6,C6,B6,A6,
        H7,G7,F7,E7,D7,C7,B7,A7,
        H8,G8,F8,E8,D8,C8,B8,A8
    };

    constexpr Bitboard sqBb[64] = {
        ONE << H1, ONE << G1, ONE << F1, ONE << E1, ONE << D1, ONE << C1, ONE << B1, ONE << A1,
        ONE << H2, ONE << G2, ONE << F2, ONE << E2, ONE << D2, ONE << C2, ONE << B2, ONE << A2,
        ONE << H3, ONE << G3, ONE << F3, ONE << E3, ONE << D3, ONE << C3, ONE << B3, ONE << A3,
        ONE << H4, ONE << G4, ONE << F4, ONE << E4, ONE << D4, ONE << C4, ONE << B4, ONE << A4,
        ONE << H5, ONE << G5, ONE << F5, ONE << E5, ONE << D5, ONE << C5, ONE << B5, ONE << A5,
        ONE << H6, ONE << G6, ONE << F6, ONE << E6, ONE << D6, ONE << C6, ONE << B6, ONE << A6,
        ONE << H7, ONE << G7, ONE << F7, ONE << E7, ONE << D7, ONE << C7, ONE << B7, ONE << A7,
        ONE << H8, ONE << G8, ONE << F8, ONE << E8, ONE << D8, ONE << C8, ONE << B8, ONE << A8,
    };


    
    
}

#endif //ENGINE_BITBOARD_H