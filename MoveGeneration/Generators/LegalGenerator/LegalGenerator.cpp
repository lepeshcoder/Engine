//
// Created by nelep on 03.12.2025.
//

#include "LegalGenerator.h"

#include <iostream>

std::vector<Move::Move> LegalGenerator::GenerateAllMoves(const Position &pos) {
    std::vector<Move::Move> moves;
    moves.reserve(50);

    auto currentColor = pos.GetCurrentColor();
    auto allPieces = pos.GetAllPieces();
    auto friends = pos.GetPiecesByColor(currentColor);
    auto enemies = pos.GetPiecesByColor(currentColor ^ 1);
    auto kingSq = Bitboard::BitScanForward(pos.GetPieceBitboard(currentColor, KING));

    // queens
    auto queens = pos.GetPieceBitboard(currentColor, QUEEN);
    while (queens) {
        int queenSq = Bitboard::BitScanForward(queens);

        auto queenMovesBb = Cache::GetQueenMoves(queenSq,allPieces) & ~friends;
        while (queenMovesBb) {
            int to = Bitboard::BitScanForward(queenMovesBb);
            bool isCapture = enemies & 1ull << to;
            int capType    = isCapture ? pos.GetPieceTypeBySquare(to) : NONE;
            int flags      = isCapture ? Move::CaptureFlag : 0;
            moves.emplace_back(queenSq,to,flags,QUEEN,capType,NONE);
            queenMovesBb &= queenMovesBb - 1;
        }
        queens &= queens - 1;
    }

    //rooks
    auto rooks = pos.GetPieceBitboard(currentColor, ROOK);
    while (rooks) {
        int rookSq = Bitboard::BitScanForward(rooks);
        auto rookMovesBb = Cache::GetRookMoves(rookSq,allPieces) & ~friends;
        while (rookMovesBb) {
            int to = Bitboard::BitScanForward(rookMovesBb);
            bool isCapture = enemies & 1ull << to;
            int capType    = isCapture ? pos.GetPieceTypeBySquare(to) : NONE;
            int flags      = isCapture ? Move::CaptureFlag : 0;
            moves.emplace_back(rookSq,to,flags,ROOK,capType,NONE);
            rookMovesBb &= rookMovesBb - 1;
        }
        rooks &= rooks - 1;
    }

    //bishops
    auto bishops = pos.GetPieceBitboard(currentColor, BISHOP);
    while (bishops) {
        int bishopSq = Bitboard::BitScanForward(bishops);
        auto bishopMovesBb = Cache::GetBishopMoves(bishopSq,allPieces) & ~friends;
        while (bishopMovesBb) {
            int to = Bitboard::BitScanForward(bishopMovesBb);
            bool isCapture = enemies & 1ull << to;
            int capType    = isCapture ? pos.GetPieceTypeBySquare(to) : NONE;
            int flags      = isCapture ? Move::CaptureFlag : 0;
            moves.emplace_back(bishopSq,to,flags,BISHOP,capType,NONE);
            bishopMovesBb &= bishopMovesBb - 1;
        }
        bishops &= bishops - 1;
    }

    // knights
    auto knights = pos.GetPieceBitboard(currentColor, KNIGHT);
    while (knights) {
        int knightSq = Bitboard::BitScanForward(knights);
        auto knightMovesBb = Cache::GetKnightMoves(knightSq) & ~friends;
        while (knightMovesBb) {
            int to = Bitboard::BitScanForward(knightMovesBb);
            bool isCapture = enemies & 1ull << to;
            int capType    = isCapture ? pos.GetPieceTypeBySquare(to) : NONE;
            int flags      = isCapture ? Move::CaptureFlag : 0;
            moves.emplace_back(knightSq,to,flags,KNIGHT,capType,NONE);
            knightMovesBb &= knightMovesBb - 1;
        }
        knights &= knights - 1;
    }

    //pawns
    auto pawns = pos.GetPieceBitboard(currentColor, PAWN);
    while (pawns) {
        int pawnSq = Bitboard::BitScanForward(pawns);
        auto pawnMoves = Cache::GetPawnMoves(currentColor,pawnSq,allPieces);
        // MOVES
        while (pawnMoves) {
            int to = Bitboard::BitScanForward(pawnMoves);
            bool isPromotion = to >> 3 == (currentColor ? 7 : 0);
            if (isPromotion) {
                moves.emplace_back(pawnSq,to,Move::PromotionFlag,PAWN,NONE,QUEEN);
                moves.emplace_back(pawnSq,to,Move::PromotionFlag,PAWN,NONE,ROOK);
                moves.emplace_back(pawnSq,to,Move::PromotionFlag,PAWN,NONE,BISHOP);
                moves.emplace_back(pawnSq,to,Move::PromotionFlag,PAWN,NONE,KNIGHT);
            }
            else {
                moves.emplace_back(pawnSq,to,0,PAWN,NONE,NONE);
            }
            pawnMoves &= pawnMoves - 1;
        }

        //EnPassant
        auto pawnAttacksRaw = Cache::GetPawnAttacks(currentColor,pawnSq);
        auto enPassantField = pos.GetEnPassantField();
        if (enPassantField) {
            auto enPassantBb = Bitboard::ONE << enPassantField;
            bool isEnPassant = pawnAttacksRaw & enPassantBb;
            if (isEnPassant) {
                moves.emplace_back(pawnSq,enPassantField,Move::CaptureFlag | Move::EnPassantFlag,
                    PAWN,PAWN,NONE);
            }
        }

        //ATTACKS
        auto pawnAttacks = pawnAttacksRaw & enemies;
        while (pawnAttacks) {
            int to = Bitboard::BitScanForward(pawnAttacks);
            int capturedPieceType = pos.GetPieceTypeBySquare(to);
            bool isPromotion = to >> 3 == (currentColor ? 7 : 0);
            if (isPromotion) {
                moves.emplace_back(pawnSq,to,Move::PromotionFlag | Move::CaptureFlag,PAWN,capturedPieceType,QUEEN);
                moves.emplace_back(pawnSq,to,Move::PromotionFlag | Move::CaptureFlag,PAWN,capturedPieceType,ROOK);
                moves.emplace_back(pawnSq,to,Move::PromotionFlag | Move::CaptureFlag,PAWN,capturedPieceType,BISHOP);
                moves.emplace_back(pawnSq,to,Move::PromotionFlag | Move::CaptureFlag,PAWN,capturedPieceType,KNIGHT);
            }
            else {
                moves.emplace_back(pawnSq,to,Move::CaptureFlag,PAWN,capturedPieceType,NONE);
            }
            pawnAttacks &= pawnAttacks - 1;
        }
        pawns &= pawns - 1;
    }

    // king moves

    auto kingMoves = Cache::GetKingMoves(kingSq) & ~friends;
    while (kingMoves) {
        int to = Bitboard::BitScanForward(kingMoves);
        bool isCapture = enemies & 1ull << to;
        int capType    = isCapture ? pos.GetPieceTypeBySquare(to) : NONE;
        int flags      = isCapture ? Move::CaptureFlag : 0;
        moves.emplace_back(kingSq,to,flags,KING,capType,NONE);
        kingMoves &= kingMoves - 1;
    }
    //short castle
    auto shortCastleBlockers = (currentColor == WHITE ?
        6 :
        Bitboard::ONE << 57 | Bitboard::ONE << 58) &
        allPieces;

    auto isShortCastleAvailable =
        pos.IsShortCastleAvailable(currentColor) &&
        !shortCastleBlockers;

    if (isShortCastleAvailable) {
        moves.emplace_back(kingSq,kingSq - 2,Move::ShortCastleFlag,KING,NONE,NONE);
    }
    // long castle

    auto longCastleBlockers = (currentColor == WHITE ?
        Bitboard::ONE << 4 | Bitboard::ONE << 5 | Bitboard::ONE << 6 :
        Bitboard::ONE << 62 | Bitboard::ONE << 61 | Bitboard::ONE << 60) &
        allPieces;

    auto isLongCastleAvailable =
        pos.IsLongCastleAvailable(currentColor) &&
        !longCastleBlockers;

    if (isLongCastleAvailable) {
        moves.emplace_back(kingSq,kingSq + 2,Move::LongCastleFlag,KING,NONE,NONE);
    }

    return moves;
}

