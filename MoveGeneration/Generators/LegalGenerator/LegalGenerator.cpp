//
// Created by nelep on 03.12.2025.
//

#include "LegalGenerator.h"

#include <iostream>

void LegalGenerator::GenerateCaptures(const Position& pos, MoveList& moveList)
{
    auto curColor = pos.GetCurrentColor();
    auto opColor = curColor ^ 1;
    auto target = pos.GetPiecesByColor(opColor);
    GeneratePieceMove(pos,moveList,QUEEN,target);
    GeneratePieceMove(pos,moveList,ROOK,target);
    GeneratePieceMove(pos,moveList,BISHOP,target);
    GeneratePieceMove(pos,moveList,KNIGHT,target);
    GeneratePieceMove(pos,moveList,KING,target);
    GeneratePawnMoves(pos,moveList,target);
}

void LegalGenerator::GenerateLegalMoves(const Position& pos, MoveList& moveList)
{
    auto curColor = pos.GetCurrentColor();
    auto checkers = pos.GetCheckers();
    auto blockers = pos.GetBlockers();
    auto kingSq = pos.GetKingSq(curColor);
    // if check generate pseudolegal evasions otherwise pseudolegal allmoves 
    checkers ? GenerateEvasions(pos,moveList) : 
               GenerateAllMoves(pos,moveList);

    Move::Move* curr = moveList.Begin();
    while(curr != moveList.End()){                                             // for every move
        if( (Bitboard::sqBb[curr->getFrom()] & blockers) ||                    // if blocker go away 
            (Bitboard::sqBb[curr->getFrom()] & Bitboard::sqBb[kingSq]) ||      // if king moves and castles
            (curr->isEnPassant()) ){                                           // if enPassant
            if (pos.IsLegal(*curr)) {                                          // check for legality
                curr++;                                                        // if legal go to next move
            }
            else {
                *curr = *(moveList.End() - 1);                                 // if not copy last move in curr
                moveList.PopBack();                                            // decrease size
            }
        }
        else {
            curr++;                                                            // if move not in potential danger dont check for legal and go next
        }
    }
}

void LegalGenerator::GenerateCastles(const Position& pos, MoveList& moveList)
{
    constexpr Bitboard::Bitboard shortCastleBlockersByColor[2] = {
        Bitboard::sqBb[Bitboard::F1] | Bitboard::sqBb[Bitboard::G1],
        Bitboard::sqBb[Bitboard::F8] | Bitboard::sqBb[Bitboard::G8]
    };

    constexpr Bitboard::Bitboard longCastleBlockersByColor[2] = {
        Bitboard::sqBb[Bitboard::D1] | Bitboard::sqBb[Bitboard::C1] | Bitboard::sqBb[Bitboard::B1],
        Bitboard::sqBb[Bitboard::D8] | Bitboard::sqBb[Bitboard::C8] | Bitboard::sqBb[Bitboard::B8]
    };

    auto allPieces = pos.GetAllPieces();
    auto curColor = pos.GetCurrentColor();   
    auto kingSq = pos.GetKingSq(curColor);

    //short castle
    auto isShortCastleAvailable = pos.IsShortCastleAvailable(curColor) &&                      // if king and rook doesnt move
                                  !(shortCastleBlockersByColor[curColor] & allPieces);         // if no pieces beetween king and rook

    if (isShortCastleAvailable) {
        moveList.Push({ kingSq, (uint8_t)(kingSq - 2), Move::ShortCastleFlag, KING, NONE, NONE });
    }

    // long castle
    auto isLongCastleAvailable = pos.IsLongCastleAvailable(curColor) &&                       // same as short castle
                                 !(longCastleBlockersByColor[curColor] & allPieces);

    if (isLongCastleAvailable) {
        moveList.Push({ kingSq, (uint8_t)(kingSq + 2), Move::LongCastleFlag, KING, NONE, NONE });
    }
}

void LegalGenerator::GeneratePawnMoves(const Position& pos, MoveList& moveList, Bitboard::Bitboard target)
{
    auto curColor = pos.GetCurrentColor();
    auto pawns = pos.GetPieceBitboard(curColor, PAWN);
    auto blockers = pos.GetAllPieces();
    auto enemies = pos.GetPiecesByColor(curColor ^ 1);
    auto enPassantField = pos.GetEnPassantField();
   
    while (pawns) {
        auto pawnSq = Bitboard::BitScanForward(pawns);
        auto pawnMoves = Cache::GetPawnMoves(curColor, pawnSq, blockers) & target;
        // MOVES
        while (pawnMoves) {
            auto to = Bitboard::BitScanForward(pawnMoves);
            bool isPromotion = to >> 3 == (curColor ? 0 : 7);
            if (isPromotion) {
                moveList.Push({ pawnSq, to, Move::PromotionFlag, PAWN, NONE, QUEEN });
                moveList.Push({ pawnSq, to, Move::PromotionFlag, PAWN, NONE, ROOK });
                moveList.Push({ pawnSq, to, Move::PromotionFlag, PAWN, NONE, BISHOP });
                moveList.Push({ pawnSq, to, Move::PromotionFlag, PAWN, NONE, KNIGHT });
            }
            else {
                moveList.Push({ pawnSq, to, 0, PAWN, NONE, NONE });
            }
            pawnMoves &= pawnMoves - 1;
        }

        //ATTACKS
        auto pawnAttacksRaw = Cache::GetPawnAttacks(curColor, pawnSq);
        auto pawnAttacks = pawnAttacksRaw & enemies & target;
        while (pawnAttacks) {
            auto to = Bitboard::BitScanForward(pawnAttacks);
            auto capturedPieceType = pos.GetPieceTypeBySquare(to);
            bool isPromotion = to >> 3 == (curColor ? 0 : 7);
            if (isPromotion) {
                moveList.Push({ pawnSq, to, Move::PromotionFlag | Move::CaptureFlag, PAWN, capturedPieceType, QUEEN });
                moveList.Push({ pawnSq, to, Move::PromotionFlag | Move::CaptureFlag, PAWN, capturedPieceType, ROOK });
                moveList.Push({ pawnSq, to, Move::PromotionFlag | Move::CaptureFlag, PAWN, capturedPieceType, BISHOP });
                moveList.Push({ pawnSq, to, Move::PromotionFlag | Move::CaptureFlag, PAWN, capturedPieceType, KNIGHT });
            }
            else {
                moveList.Push({ pawnSq, to, Move::CaptureFlag, PAWN, capturedPieceType, NONE });
            }
            pawnAttacks &= pawnAttacks - 1;
        }
        

        //En passant
        if (enPassantField != Bitboard::SQ_NONE) { 
            auto newTarget = target;
            if (target != Bitboard::ALLONE) // if evasions generated
            {
                // en passant can only block direct check not discover
                if (target == pos.GetCheckers()) // if en passant capture the pawn that gives check
                {
                    newTarget = curColor == WHITE ? target << 8 : target >> 8;
                }
                else
                {
                    pawns &= pawns - 1;
                    continue;
                }
            }
            auto enPassantBb = Bitboard::sqBb[enPassantField];
            bool isEnPassant = pawnAttacksRaw & enPassantBb & newTarget;
            if (isEnPassant) {
                moveList.Push({ pawnSq, enPassantField, Move::CaptureFlag | Move::EnPassantFlag,
                    PAWN, PAWN, NONE });
            }
        }
        pawns &= pawns - 1;
    }
}

void LegalGenerator::GeneratePieceMove(const Position& pos, MoveList& moveList, uint8_t pieceType,Bitboard::Bitboard target)
{
    auto blockers = pos.GetAllPieces();
    auto curColor = pos.GetCurrentColor();
    auto opColor = curColor ^ 1; 
    auto friends = pos.GetPiecesByColor(curColor);
    auto enemies = pos.GetPiecesByColor(opColor);
    auto pieces = pos.GetPieceBitboard(curColor,pieceType);
    while (pieces) {
        auto pieceSq = Bitboard::BitScanForward(pieces);
        auto pieceMoves = Cache::GetPieceMoves(pieceType,pieceSq, blockers) & ~friends & target;
        while (pieceMoves)
        {
            auto to = Bitboard::BitScanForward(pieceMoves);
            auto isCapture = Bitboard::sqBb[to] & enemies;
            if (isCapture){
                moveList.Push({pieceSq,to,Move::CaptureFlag,pieceType,pos.GetPieceTypeBySquare(to),NONE});
            }
            else {
                moveList.Push({pieceSq,to,0,pieceType,NONE,NONE});
            }
            pieceMoves &= pieceMoves - 1;
        }
        pieces &= pieces - 1;
    }
}

void LegalGenerator::GenerateEvasions(const Position& pos, MoveList& moveList)
{
    auto curColor = pos.GetCurrentColor();
    auto kingSq = pos.GetKingSq(curColor);
    auto opColor = curColor ^ 1;
    auto enemies = pos.GetPiecesByColor(opColor);
    auto friends = pos.GetPiecesByColor(curColor);
    auto checkers = pos.GetCheckers();
    // if double check
    if (Bitboard::PopCount(checkers) > 1)
    {
        // generate only king moves
        auto kingMoves = Cache::GetKingMoves(kingSq) & ~friends;
        while (kingMoves)
        {
            auto to = Bitboard::BitScanForward(kingMoves);
            auto isCapture = Bitboard::sqBb[to] & enemies;
            if(isCapture){ 
                moveList.Push({kingSq,to,Move::CaptureFlag,KING,pos.GetPieceTypeBySquare(to),NONE});
            }
            else {
                moveList.Push({kingSq, to, 0, KING, NONE, NONE});
            }
            kingMoves &= kingMoves - 1;
        }
        return;
    }
    // if direct or discovered check (popcnt(checkers) == 1)
    auto checkerSq = Bitboard::BitScanForward(checkers);
    // we can block the check in line beetween king and checker or capture the checker or king moves
    auto target = Cache::GetBetweenBb(kingSq,checkerSq) | Bitboard::sqBb[checkerSq];
    GeneratePieceMove(pos,moveList,QUEEN,target);
    GeneratePieceMove(pos,moveList,ROOK,target);
    GeneratePieceMove(pos,moveList,BISHOP,target);
    GeneratePieceMove(pos,moveList,KNIGHT,target);
    GeneratePieceMove(pos,moveList,KING);                         
    GeneratePawnMoves(pos,moveList,target);
    
}

void LegalGenerator::GenerateAllMoves(const Position& pos, MoveList& moveList)
{  
    GeneratePieceMove(pos,moveList,QUEEN);
    GeneratePieceMove(pos,moveList,ROOK);
    GeneratePieceMove(pos,moveList,BISHOP);
    GeneratePieceMove(pos,moveList,KNIGHT);
    GeneratePieceMove(pos,moveList,KING);
    GeneratePawnMoves(pos,moveList);
    GenerateCastles(pos,moveList); 
}

void LegalGenerator::ComparePositions(const Position& a, const Position& b)
{
    bool equal = true;

    auto diff = [&](const std::string& msg) {
        std::cout << "[DIFF] " << msg << "\n";
        equal = false;
        };

    // Side to move
    if (a.GetCurrentColor() != b.GetCurrentColor())
        diff("Side to move differs");

    // King squares
    for (int c = 0; c < 2; ++c) {
        if (a.GetKingSq(c) != b.GetKingSq(c)) {
            diff("King square differs for color " + std::to_string(c));
        }
    }

    // Pieces bitboards
    for (int c = 0; c < 2; ++c) {
        for (int p = 0; p < 6; ++p) {
            auto bb1 = a.GetPieceBitboard(c, p);
            auto bb2 = b.GetPieceBitboard(c, p);
            if (bb1 != bb2) {
                diff("Bitboard differs: color=" + std::to_string(c) +
                    " piece=" + std::to_string(p));
            }
        }
    }

    // Types array
    for (int sq = 0; sq < 64; ++sq) {
        if (a.GetPieceTypeBySquare(sq) != b.GetPieceTypeBySquare(sq)) {
            diff("Types differ on square " + std::to_string(sq));
        }
    }

    // Castling rights
    for (int c = 0; c < 2; ++c) {
        if (a.IsShortCastleAvailable(c) != b.IsShortCastleAvailable(c))
            diff("Short castle right differs for color " + std::to_string(c));

        if (a.IsLongCastleAvailable(c) != b.IsLongCastleAvailable(c))
            diff("Long castle right differs for color " + std::to_string(c));
    }

    // En passant
    if (a.GetEnPassantField() != b.GetEnPassantField())
        diff("En passant square differs");

    // All pieces
    if (a.GetAllPieces() != b.GetAllPieces())
        diff("allPieces bitboard differs");

    for (int c = 0; c < 2; ++c) {
        if (a.GetPiecesByColor(c) != b.GetPiecesByColor(c))
            diff("allPiecesByColor differs for color " + std::to_string(c));
    }

    if (equal)
        std::cout << "[OK] Positions are identical\n";
}
