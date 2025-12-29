//
// Created by nelep on 25.11.2025.
//
#include "Bitboard.h"



std::string Bitboard::BitboardToString(const Bitboard bb, BitOrder order) {
    std::string result;
    if (order == BitOrder::MSB_first) {
        Bitboard mask = 1ull << 63;
        for (int i = 0; i < 64; i++) {
            result += mask & bb ? "1" : "0";
            result += i % 8 == 7 ? "\n" : " ";
            mask >>= 1;
        }
    }
    else {
        Bitboard mask = 1ull;
        for (int i = 0; i < 64; i++) {
            result += mask & bb ? "1" : "0";
            result += i % 8 == 7 ? "\n" : " ";
            mask <<= 1;
        }
    }
    return result;
}


