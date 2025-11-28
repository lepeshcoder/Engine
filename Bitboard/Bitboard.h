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
    inline int BitScanForward(Bitboard bb) {
        if (bb == 0ull) return -1;
        return __builtin_ctzll(bb);
    }

    // largest index of set(1) bit
    inline int BitScanReverse(Bitboard bb) {
        if (bb == 0) return -1;
        return 63 - __builtin_clzll(bb);
    }

    std::string BitboardToString(Bitboard bb, BitOrder order = BitOrder::MSB_first);

    constexpr Bitboard ZERO = 0ull;
    constexpr Bitboard ONE = 1ull;

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


}

#endif //ENGINE_BITBOARD_H