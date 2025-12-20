//
// Created by nelep on 25.11.2025.
//

#include "Position.h"

#include <iostream>

#include "../MoveGeneration/NonSlidingPieces/NonSlidingPieces.h"


void Position::MakeShortCastleMove(const Move::Move &move) {
    auto rookBeforeMoveSq = CurrentColor == WHITE ? Bitboard::H1 : Bitboard::H8;
    auto rookAfterMoveSq = CurrentColor == WHITE ? Bitboard::F1 : Bitboard::F8;
    MakeCastleMove(move, rookBeforeMoveSq, rookAfterMoveSq);
}

void Position::MakeLongCastleMove(const Move::Move &move) {
    auto rookBeforeMoveSq = CurrentColor == WHITE ? Bitboard::A1 : Bitboard::A8;
    auto rookAfterMoveSq = CurrentColor == WHITE ? Bitboard::D1 : Bitboard::D8;
    MakeCastleMove(move, rookBeforeMoveSq, rookAfterMoveSq);
}

void Position::MakeCastleMove(const Move::Move &move, Bitboard::Bitboard rookBeforeMoveSq, Bitboard::Bitboard rookAfterMoveSq) {
    auto from = move.getFrom();
    auto to = move.getTo();
    //Update Pieces
    Pieces[CurrentColor][KING] &= ~Bitboard::sqBb[from];
    Pieces[CurrentColor][KING] |= Bitboard::sqBb[to];

    Pieces[CurrentColor][ROOK] &= ~Bitboard::sqBb[rookBeforeMoveSq];
    Pieces[CurrentColor][ROOK] |= Bitboard::sqBb[rookAfterMoveSq];

    //Update Types
    Types[from] = NONE;
    Types[to] = KING;
    Types[rookBeforeMoveSq] = NONE;
    Types[rookAfterMoveSq] = ROOK;

    //Update king Sq
    kingsSq[CurrentColor] = to;

    //Update castleRules
    castleRights &= ~castlingMasks[from];
    castleRights &= ~castlingMasks[to];

    //UpdateAllPieces
    allPiecesByColor[CurrentColor] &= ~(Bitboard::sqBb[from] | Bitboard::sqBb[rookBeforeMoveSq]);
    allPiecesByColor[CurrentColor] |=  Bitboard::sqBb[to] | Bitboard::sqBb[rookAfterMoveSq];
    allPieces = allPiecesByColor[WHITE] | allPiecesByColor[BLACK];

    //UpdateEnPassantField
    EnPassantField = 0;

    //UpdateCounters
    ++MoveCounter;
    ++FiftyMovesCounter;

    //Change CurrentColor
    CurrentColor = CurrentColor == WHITE ? BLACK : WHITE;
}

void Position::MakeCaptureMove(const Move::Move &move) {
    auto from = move.getFrom();
    auto to = move.getTo();
    auto movingPieceType = move.getMovingPieceType();
    auto capturedPieceType = move.getCapturedPieceType();
    auto opColor = CurrentColor ^ 1;

    //Update Pieces
    Pieces[CurrentColor][movingPieceType] &= ~Bitboard::sqBb[from];
    Pieces[CurrentColor][movingPieceType] |= Bitboard::sqBb[to];
    Pieces[opColor][capturedPieceType] &= ~Bitboard::sqBb[to];

    //Update Types
    Types[from] = NONE;
    Types[to] = movingPieceType;

    //Update king Sq
    if (movingPieceType == KING) {
        kingsSq[CurrentColor] = to;
    }

    //Update castle rules
    castleRights &= ~castlingMasks[from];
    castleRights &= ~castlingMasks[to];

    //UpdateAllPieces
    allPiecesByColor[CurrentColor] &= ~Bitboard::sqBb[from];
    allPiecesByColor[CurrentColor] |= Bitboard::sqBb[to];
    allPiecesByColor[opColor] &= ~Bitboard::sqBb[to];
    allPieces = allPiecesByColor[WHITE] | allPiecesByColor[BLACK];

    //UpdateEnPassantField
    EnPassantField = 0;

    //UpdateCounters
    ++MoveCounter;
    FiftyMovesCounter = 0;

    // Change Color
    CurrentColor = CurrentColor == WHITE ? BLACK : WHITE;
}

void Position::MakeEnPassantMove(const Move::Move &move) {
    auto from = move.getFrom();
    auto to = move.getTo();
    auto opColor = CurrentColor ^ 1;
    auto capSq = CurrentColor == WHITE ? to - 8 : to + 8;

    // Update Pieces
    Pieces[CurrentColor][PAWN] &= ~Bitboard::sqBb[from];
    Pieces[CurrentColor][PAWN] |= Bitboard::sqBb[to];
    Pieces[opColor][PAWN] &= ~Bitboard::sqBb[capSq];

    //Update Types
    Types[from] = NONE;
    Types[to] = PAWN;
    Types[capSq] = NONE;

    // No need to Update Castle rights and kingSq

    //UpdateAllPieces
    allPiecesByColor[CurrentColor] &= ~Bitboard::sqBb[from];
    allPiecesByColor[CurrentColor] |= Bitboard::sqBb[to];
    allPiecesByColor[opColor] &= ~Bitboard::sqBb[capSq];
    allPieces = allPiecesByColor[WHITE] | allPiecesByColor[BLACK];

    //UpdateEnPassantField
    EnPassantField = 0;

    //UpdateCounters
    ++MoveCounter;
    FiftyMovesCounter = 0;

    // Change Color
    CurrentColor = CurrentColor == WHITE ? BLACK : WHITE;
}

void Position::MakePromotionMove(const Move::Move &move) {
    auto from = move.getFrom();
    auto to = move.getTo();
    auto promotedPieceType = move.getMovingPieceType();

    //Update Pieces
    Pieces[CurrentColor][PAWN] &= ~Bitboard::sqBb[from];
    Pieces[CurrentColor][promotedPieceType] |= Bitboard::sqBb[to];

    //Update types
    Types[from] = NONE;
    Types[to] = promotedPieceType;

    // No need to Update Castle rights and kingSq

    //UpdateAllPieces
    allPiecesByColor[CurrentColor] &= ~Bitboard::sqBb[from];
    allPiecesByColor[CurrentColor] |= Bitboard::sqBb[to];
    allPieces = allPiecesByColor[WHITE] | allPiecesByColor[BLACK];

    //UpdateEnPassantField
    EnPassantField = 0;

    //UpdateCounters
    ++MoveCounter;
    FiftyMovesCounter = 0;

    // Change Color
    CurrentColor = CurrentColor == WHITE ? BLACK : WHITE;
}

void Position::MakeCapturePromotionMove(const Move::Move &move) {
    auto from = move.getFrom();
    auto to = move.getTo();
    auto opColor = CurrentColor ^ 1;
    auto promotedPieceType = move.getMovingPieceType();
    auto capturedPieceType = move.getCapturedPieceType();

    //Update Pieces
    Pieces[CurrentColor][PAWN] &= ~Bitboard::sqBb[from];
    Pieces[CurrentColor][promotedPieceType] |= Bitboard::sqBb[to];
    Pieces[opColor][capturedPieceType] &= ~Bitboard::sqBb[to];

    //Update Types
    Types[from] = NONE;
    Types[to] = promotedPieceType;

    //Update castle rules (pawn can only capture op rook)
    castleRights &= ~castlingMasks[to];

    //UpdateAllPieces
    allPiecesByColor[CurrentColor] &= ~Bitboard::sqBb[from];
    allPiecesByColor[CurrentColor] |= Bitboard::sqBb[to];
    allPiecesByColor[opColor] &= ~Bitboard::sqBb[to];
    allPieces = allPiecesByColor[WHITE] | allPiecesByColor[BLACK];

    //UpdateEnPassantField
    EnPassantField = 0;

    //UpdateCounters
    ++MoveCounter;
    FiftyMovesCounter = 0;

    // Change Color
    CurrentColor = CurrentColor == WHITE ? BLACK : WHITE;
}

void Position::MakeQuietMove(const Move::Move &move) {
    auto from = move.getFrom();
    auto to = move.getTo();
    auto movingPieceType = move.getMovingPieceType();

    //Update Pieces
    Pieces[CurrentColor][movingPieceType] &= ~Bitboard::sqBb[from];
    Pieces[CurrentColor][movingPieceType] |= Bitboard::sqBb[to];

    //Update Types
    Types[from] = NONE;
    Types[to] = movingPieceType;

    //Update king sq
    if (movingPieceType == KING)
        kingsSq[CurrentColor] = to;

    //Update castle rules(can only move king or rook)
    castleRights &= ~castlingMasks[from];

    //UpdateAllPieces
    allPiecesByColor[CurrentColor] &= ~Bitboard::sqBb[from];
    allPiecesByColor[CurrentColor] |= Bitboard::sqBb[to];
    allPieces = allPiecesByColor[WHITE] | allPiecesByColor[BLACK];

    //UpdateEnPassantField
    if (movingPieceType == PAWN && std::abs((int)from - (int)to) == 16)            // if pawn double push
        EnPassantField = CurrentColor == WHITE ? to - 8 : to + 8;                     // set EnPassantField
    else EnPassantField = 0;                                                          // else reset field

    //UpdateCounters
    ++MoveCounter;
    if (movingPieceType == PAWN) FiftyMovesCounter = 0;
    else ++FiftyMovesCounter;

    // Change Color
    CurrentColor = CurrentColor == WHITE ? BLACK : WHITE;
}

void Position::UnMakeShortCastleMove(const Move::Move &move) {
    auto rookBeforeMoveSq = CurrentColor == WHITE ? Bitboard::H1 : Bitboard::H8;
    auto rookAfterMoveSq = CurrentColor == WHITE ? Bitboard::F1 : Bitboard::F8;
    UnMakeCastleMove(move,rookBeforeMoveSq,rookAfterMoveSq);
}

void Position::UnMakeLongCastleMove(const Move::Move &move) {
    auto rookBeforeMoveSq = CurrentColor == WHITE ? Bitboard::A1 : Bitboard::A8;
    auto rookAfterMoveSq = CurrentColor == WHITE ? Bitboard::D1 : Bitboard::D8;
    UnMakeCastleMove(move,rookBeforeMoveSq,rookAfterMoveSq);
}

void Position::UnMakeCastleMove(const Move::Move &move, Bitboard::Bitboard rookBeforeMoveSq,
    Bitboard::Bitboard rookAfterMoveSq) {
    auto from = move.getFrom();
    auto to = move.getTo();

    //Update CurrentColor
    CurrentColor = CurrentColor == WHITE ? BLACK : WHITE;

    //Update Counter
    --MoveCounter;

    //Update Pieces
    Pieces[CurrentColor][KING] &= ~Bitboard::sqBb[to];
    Pieces[CurrentColor][KING] |= Bitboard::sqBb[from];
    Pieces[CurrentColor][ROOK] &= ~Bitboard::sqBb[rookAfterMoveSq];
    Pieces[CurrentColor][ROOK] |= Bitboard::sqBb[rookBeforeMoveSq];

    //Update Types
    Types[to] = NONE;
    Types[from] = KING;
    Types[rookAfterMoveSq] = NONE;
    Types[rookBeforeMoveSq] = ROOK;

    //Update kingSq
    kingsSq[CurrentColor] = from;

    //Update allPeaces
    allPiecesByColor[CurrentColor] &= ~(Bitboard::sqBb[to] | Bitboard::sqBb[rookAfterMoveSq]);
    allPiecesByColor[CurrentColor] |= Bitboard::sqBb[from] | Bitboard::sqBb[rookBeforeMoveSq];
    allPieces = allPiecesByColor[WHITE] | allPiecesByColor[BLACK];
}

void Position::UnMakeCaptureMove(const Move::Move &move) {
    auto from = move.getFrom();
    auto to = move.getTo();
    auto movingPieceType = move.getMovingPieceType();
    auto capturedPieceType = move.getCapturedPieceType();
    auto opColor = CurrentColor;

    //Update CurrentColor
    CurrentColor = CurrentColor == WHITE ? BLACK : WHITE;

    //Update Counter
    --MoveCounter;

    //Update Pieces
    Pieces[CurrentColor][movingPieceType] &= ~Bitboard::sqBb[to];
    Pieces[CurrentColor][movingPieceType] |= Bitboard::sqBb[from];
    Pieces[opColor][capturedPieceType] |= Bitboard::sqBb[to];

    //Update Types
    Types[to] = capturedPieceType;
    Types[from] = movingPieceType;

    //Update kingSq
    if (movingPieceType == KING)
        kingsSq[CurrentColor] = from;

    //Update allPeaces
    allPiecesByColor[CurrentColor] &= ~Bitboard::sqBb[to];
    allPiecesByColor[CurrentColor] |= Bitboard::sqBb[from];
    allPiecesByColor[opColor] |= Bitboard::sqBb[to];
    allPieces = allPiecesByColor[WHITE] | allPiecesByColor[BLACK];
}

void Position::UnMakeEnPassantMove(const Move::Move &move) {
    auto from = move.getFrom();
    auto to = move.getTo();
    auto opColor = CurrentColor;

    //Update CurrentColor
    CurrentColor = CurrentColor == WHITE ? BLACK : WHITE;

    auto capSq = CurrentColor == WHITE ? to - 8 : to + 8;

    //Update Counter
    --MoveCounter;

    //Update Pieces
    Pieces[CurrentColor][PAWN] &= ~Bitboard::sqBb[to];
    Pieces[CurrentColor][PAWN] |= Bitboard::sqBb[from];
    Pieces[opColor][PAWN] |= Bitboard::sqBb[capSq];

    //Update Types
    Types[to] = NONE;
    Types[from] = PAWN;
    Types[capSq] = PAWN;

    //No need to update kingSq

    //Update allPeaces
    allPiecesByColor[CurrentColor] &= Bitboard::sqBb[to];
    allPiecesByColor[CurrentColor] |= Bitboard::sqBb[from];
    allPiecesByColor[opColor] |= Bitboard::sqBb[capSq];
    allPieces = allPiecesByColor[WHITE] | allPiecesByColor[BLACK];
}

void Position::UnMakePromotionMove(const Move::Move &move) {
    auto from = move.getFrom();
    auto to = move.getTo();
    auto promotionPieceType = move.getPromotionPieceType();

    //Update CurrentColor
    CurrentColor = CurrentColor == WHITE ? BLACK : WHITE;

    //Update Counter
    --MoveCounter;

    //Update Pieces
    Pieces[CurrentColor][promotionPieceType] &= ~Bitboard::sqBb[to];
    Pieces[CurrentColor][PAWN] |= Bitboard::sqBb[from];

    //Update Types
    Types[to] = NONE;
    Types[from] = PAWN;

    //No need to update kingSq

    //Update allPeaces
    allPiecesByColor[CurrentColor] &= ~Bitboard::sqBb[to];
    allPiecesByColor[CurrentColor] |= ~Bitboard::sqBb[from];
    allPieces = allPiecesByColor[WHITE] | allPiecesByColor[BLACK];
}

void Position::UnMakeCapturePromotionMove(const Move::Move &move) {
    auto from = move.getFrom();
    auto to = move.getTo();
    auto opColor = CurrentColor;
    auto promotionPieceType = move.getPromotionPieceType();
    auto capturedPieceType = move.getCapturedPieceType();

    //Update CurrentColor
    CurrentColor = CurrentColor == WHITE ? BLACK : WHITE;

    //Update Counter
    --MoveCounter;

    //Update Pieces
    Pieces[CurrentColor][promotionPieceType] &= ~Bitboard::sqBb[to];
    Pieces[CurrentColor][PAWN] |= Bitboard::sqBb[from];
    Pieces[opColor][capturedPieceType] |= Bitboard::sqBb[to];

    //Update Types
    Types[to] = capturedPieceType;
    Types[from] = PAWN;

    //No need to update kingSq

    //Update allPieces
    allPiecesByColor[CurrentColor] &= ~Bitboard::sqBb[to];
    allPiecesByColor[CurrentColor] |= Bitboard::sqBb[from];
    allPiecesByColor[opColor] |= Bitboard::sqBb[to];
    allPieces = allPiecesByColor[WHITE] | allPiecesByColor[BLACK];
}

void Position::UnMakeQuietMove(const Move::Move &move) {
    auto from = move.getFrom();
    auto to = move.getTo();
    auto movingPieceType = move.getMovingPieceType();

    //Update CurrentColor
    CurrentColor = CurrentColor == WHITE ? BLACK : WHITE;

    //Update Counter
    --MoveCounter;

    //Update Pieces
    Pieces[CurrentColor][movingPieceType] &= ~Bitboard::sqBb[to];
    Pieces[CurrentColor][movingPieceType] |= Bitboard::sqBb[from];

    //Update Types
    Types[to] = NONE;
    Types[from] = movingPieceType;

    //Update kingSq
    if (movingPieceType == KING)
        kingsSq[CurrentColor] = from;

    //Update allPieces
    allPiecesByColor[CurrentColor] &= ~Bitboard::sqBb[to];
    allPiecesByColor[CurrentColor] |= Bitboard::sqBb[from];
    allPieces = allPiecesByColor[WHITE] | allPiecesByColor[BLACK];
}

void Position::PushUndoInfo() {
    undoStack[currentPly++] = {
        castleRights,
        FiftyMovesCounter,
        EnPassantField,
    };
}

UndoInfo& Position::PopUndoInfo() {
    return undoStack[--currentPly];
}

Position::Position(const std::string &Fen) {
    std::istringstream iss(Fen);
    std::string piecePart;
    iss >> piecePart;

    // fill bitboards
    int currPos = 63;
    for (char ch: piecePart) {
        if (ch == '/') continue;
        if (isdigit(ch)) {
            currPos -= ch - '0';
        }
        else {
            PieceColor color = isupper(ch) ? WHITE : BLACK;
            PieceType type;
            ch = std::tolower(ch);
            switch (ch) {
                case 'k': type = KING; break;
                case 'q': type = QUEEN; break;
                case 'r': type = ROOK; break;
                case 'b': type = BISHOP; break;
                case 'n': type = KNIGHT; break;
                case 'p': type = PAWN; break;
            }
            Pieces[color][type] |= Bitboard::ONE << currPos;
            allPiecesByColor[color] |= Pieces[color][type];
            Types[currPos] = type;
            --currPos;
        }
    }
    allPieces = allPiecesByColor[WHITE] | allPiecesByColor[BLACK];

    kingsSq[WHITE] = Bitboard::BitScanForward(Pieces[WHITE][KING]);
    kingsSq[BLACK] = Bitboard::BitScanForward(Pieces[BLACK][KING]);

    // currColor
    std::string currColor;
    iss >> currColor;
    CurrentColor = currColor == "w"? WHITE : BLACK;

    // castles
    std::string castles;
    iss >> castles;
    if (castles.find('K') != std::string::npos) castleRights |= WK;
    if (castles.find('Q') != std::string::npos) castleRights |= WQ;
    if (castles.find('k') != std::string::npos) castleRights |= BK;
    if (castles.find('q') != std::string::npos) castleRights |= BQ;


    // enPassant
    std::string enPassant;
    iss >> enPassant;
    if (enPassant == "-") EnPassantField = 0;
    else {
        EnPassantField = enPassant[0] - 'a' + 1;
        EnPassantField += (enPassant[1] - '0' - 1) * 8;
    }

    // 50movesRuleCounter + MoveCounter
    iss >> FiftyMovesCounter;
    iss >> MoveCounter;

}

void Position::MakeMove(const Move::Move &move) {
    PushUndoInfo();
    switch (move.getFlags()) {
        case Move::ShortCastleFlag: {
            MakeShortCastleMove(move);
            break;
        }
        case Move::LongCastleFlag: {
            MakeLongCastleMove(move);
            break;
        }
        case Move::CaptureFlag: {
            MakeCaptureMove(move);
            break;
        }
        case Move::CaptureFlag | Move::EnPassantFlag: {
            MakeEnPassantMove(move);
            break;
        }
        case Move::PromotionFlag: {
            MakePromotionMove(move);
            break;
        }
        case Move::CaptureFlag | Move::PromotionFlag: {
            MakeCapturePromotionMove(move);
            break;
        }
        default: {
            MakeQuietMove(move);
            break;
        }
    }
}

void Position::UnMakeMove(const Move::Move &move) {
    // restore critical fields from stateStack
    auto undoInfo = PopUndoInfo();
    castleRights = undoInfo.castleRights;
    FiftyMovesCounter = undoInfo.fiftyMovesCounter;
    EnPassantField = undoInfo.enPassantField;
    switch (move.getFlags()) {
        case Move::ShortCastleFlag: {
            UnMakeShortCastleMove(move);
            break;
        }
        case Move::LongCastleFlag: {
            UnMakeLongCastleMove(move);
            break;
        }
        case Move::CaptureFlag: {
            UnMakeCaptureMove(move);
            break;
        }
        case Move::CaptureFlag | Move::EnPassantFlag: {
            UnMakeEnPassantMove(move);
            break;
        }
        case Move::PromotionFlag: {
            UnMakePromotionMove(move);
            break;
        }
        case Move::CaptureFlag | Move::PromotionFlag: {
            UnMakeCapturePromotionMove(move);
            break;
        }
        default: {
            UnMakeQuietMove(move);
            break;
        }
    }
}

bool Position::IsLegal(const Move::Move &move) {
    int kingSq = kingsSq[CurrentColor];
    return true;
}

bool Position::IsSquareUnderAttack(int sq) const {
    auto opColor = CurrentColor ^ 1;
    auto opQueens = Pieces[opColor][QUEEN];

    // pawnAttacks
    auto pawnAttacks = Cache::GetPawnAttacks(opColor,sq);
    auto opPawns = Pieces[opColor][PAWN];
    if (pawnAttacks & opPawns) return true;

    //knightAttacks
    auto knightAttacks = Cache::GetKnightMoves(sq);
    auto opKnights = Pieces[opColor][KNIGHT];
    if (knightAttacks & opKnights) return true;

    //bishopAttacks
    auto bishopAttacks = Cache::GetBishopMoves(sq,allPieces);
    auto opBishops = Pieces[opColor][BISHOP];
    if (bishopAttacks & (opBishops | opQueens)) return true;

    //rookAttacks
    auto rookAttacks = Cache::GetRookMoves(sq,allPieces);
    auto opRooks = Pieces[opColor][ROOK];
    if (rookAttacks & (opRooks | opQueens)) return true;

    //kingAttacks
    auto kingAttacks = Cache::GetKingMoves(sq);
    auto opKing = Pieces[opColor][KING];
    if (kingAttacks & opKing) return true;

    return false;
}

int Position::GetKingSq(int color) const {
    return kingsSq[color];
}
