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
    constexpr uint8_t rookBeforeMoveSq[2] = {Bitboard::A1 , Bitboard::A8};
    constexpr uint8_t rookAfterMoveSq[2] =  {Bitboard::D1 , Bitboard::D8};
    MakeCastleMove(move, rookBeforeMoveSq[CurrentColor], rookAfterMoveSq[CurrentColor]);
}

void Position::MakeCastleMove(const Move::Move &move, uint8_t rookBeforeMoveSq, uint8_t rookAfterMoveSq) {
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
    
    //Zobrist update Types
    info->key ^= Cache::GetZobristTable(CurrentColor, KING, from);
    info->key ^= Cache::GetZobristTable(CurrentColor, KING, to);
    info->key ^= Cache::GetZobristTable(CurrentColor, ROOK, rookBeforeMoveSq);
    info->key ^= Cache::GetZobristTable(CurrentColor, ROOK, rookAfterMoveSq);    

    // reset Old zobrist castle rights 
    info->key ^= Cache::GetZobristCastleRights(info->castleRights);

    //Update castleRules
    info->castleRights &= ~castlingMasks[from];
    info->castleRights &= ~castlingMasks[to];

    // set new zobrist castle rights
    info->key ^= Cache::GetZobristCastleRights(info->castleRights);
    
    //UpdateAllPieces
    info->allPiecesByColor[CurrentColor] &= ~(Bitboard::sqBb[from] | Bitboard::sqBb[rookBeforeMoveSq]);
    info->allPiecesByColor[CurrentColor] |=  Bitboard::sqBb[to] | Bitboard::sqBb[rookAfterMoveSq];
    info->allPieces = info->allPiecesByColor[WHITE] | info->allPiecesByColor[BLACK];

    // if enPassant field 
    if (info->EnPassantField != Bitboard::SQ_NONE)
    {
        // reset en passant zobrist
        info->key^=Cache::GetZobristEnPassant(info->EnPassantField % 8);
        //UpdateEnPassantField
        info->EnPassantField = Bitboard::SQ_NONE;
    }

    //UpdateCounters
    ++PlyFromNull;
    ++info->FiftyMovesCounter;

    //Change CurrentColor
    CurrentColor = ~CurrentColor;

    //Update zobrist curColor
    info->key ^= Cache::GetWhiteZobristSideMove();
}

void Position::MakeCaptureMove(const Move::Move &move) {
    auto from = move.getFrom();
    auto to = move.getTo();
    auto movingPieceType = Types[from];
    auto capturedPieceType = Types[to];
    auto opColor = CurrentColor ^ 1;

    //Update Pieces
    Pieces[CurrentColor][movingPieceType] &= ~Bitboard::sqBb[from];
    Pieces[CurrentColor][movingPieceType] |= Bitboard::sqBb[to];
    Pieces[opColor][capturedPieceType] &= ~Bitboard::sqBb[to];

    //Update Types
    Types[from] = NONE;
    Types[to] = movingPieceType;

    //Zobrist update Types
    info->key ^= Cache::GetZobristTable(CurrentColor, movingPieceType, from);
    info->key ^= Cache::GetZobristTable(CurrentColor, movingPieceType, to);
    info->key ^= Cache::GetZobristTable(opColor, capturedPieceType, to);

    // reset Old zobrist castle rights 
    info->key ^= Cache::GetZobristCastleRights(info->castleRights);

    //Update castle rules
    info->castleRights &= ~castlingMasks[from];
    info->castleRights &= ~castlingMasks[to];

    // set new zobrist castle rights
    info->key ^= Cache::GetZobristCastleRights(info->castleRights);

    //UpdateAllPieces
    info->allPiecesByColor[CurrentColor] &= ~Bitboard::sqBb[from];
    info->allPiecesByColor[CurrentColor] |= Bitboard::sqBb[to];
    info->allPiecesByColor[opColor] &= ~Bitboard::sqBb[to];
    info->allPieces = info->allPiecesByColor[WHITE] | info->allPiecesByColor[BLACK];


    // if enPassant field 
    if (info->EnPassantField != Bitboard::SQ_NONE)
    {
        // reset en passant zobrist
        info->key ^= Cache::GetZobristEnPassant(info->EnPassantField % 8);
        //UpdateEnPassantField
        info->EnPassantField = Bitboard::SQ_NONE;
    }


    //Update Material 
    info->Material += PiecesMaterial[CurrentColor][capturedPieceType];

    //UpdateCounters
    ++PlyFromNull;
    info->FiftyMovesCounter = 0;


    // Change Color
    CurrentColor = ~CurrentColor;

    //Update zobrist curColor
    info->key ^= Cache::GetWhiteZobristSideMove();
}

void Position::MakeEnPassantMove(const Move::Move &move) {
    auto from = move.getFrom();
    auto to = move.getTo();
    auto opColor = ~CurrentColor;
    auto capSq = CurrentColor == WHITE ? to - 8 : to + 8;

    // Update Pieces
    Pieces[CurrentColor][PAWN] &= ~Bitboard::sqBb[from];
    Pieces[CurrentColor][PAWN] |= Bitboard::sqBb[to];
    Pieces[opColor][PAWN] &= ~Bitboard::sqBb[capSq];

    //Update Types
    Types[from] = NONE;
    Types[to] = PAWN;
    Types[capSq] = NONE;

    //Zobrist update Types
    info->key ^= Cache::GetZobristTable(CurrentColor, PAWN, from);
    info->key ^= Cache::GetZobristTable(CurrentColor, PAWN, to);
    info->key ^= Cache::GetZobristTable(opColor, PAWN, capSq);

    // No need to Update Castle rights and kingSq

    //UpdateAllPieces
    info->allPiecesByColor[CurrentColor] &= ~Bitboard::sqBb[from];
    info->allPiecesByColor[CurrentColor] |= Bitboard::sqBb[to];
    info->allPiecesByColor[opColor] &= ~Bitboard::sqBb[capSq];
    info->allPieces = info->allPiecesByColor[WHITE] | info->allPiecesByColor[BLACK];

    //reset zobrist enpassant
    info->key ^= Cache::GetZobristEnPassant(to % 8);

    //UpdateEnPassantField
    info->EnPassantField = Bitboard::SQ_NONE;

    //update material
    info->Material += PiecesMaterial[CurrentColor][PAWN];

    //UpdateCounters
    ++PlyFromNull;
    info->FiftyMovesCounter = 0;

    // Change Color
    CurrentColor = ~CurrentColor;

    //Update zobrist curColor
    info->key ^= Cache::GetWhiteZobristSideMove();
}

void Position::MakePromotionMove(const Move::Move &move) {
    auto from = move.getFrom();
    auto to = move.getTo();
    auto promotedPieceType = move.getPromotionPieceType();

    //Update Pieces
    Pieces[CurrentColor][PAWN] &= ~Bitboard::sqBb[from];
    Pieces[CurrentColor][promotedPieceType] |= Bitboard::sqBb[to];

    //Update types
    Types[from] = NONE;
    Types[to] = promotedPieceType;

    //Zobrist update Types
    info->key ^= Cache::GetZobristTable(CurrentColor, PAWN, from);
    info->key ^= Cache::GetZobristTable(CurrentColor, promotedPieceType, to);

    // No need to Update Castle rights and kingSq

    //UpdateAllPieces
    info->allPiecesByColor[CurrentColor] &= ~Bitboard::sqBb[from];
    info->allPiecesByColor[CurrentColor] |= Bitboard::sqBb[to];
    info->allPieces = info->allPiecesByColor[WHITE] | info->allPiecesByColor[BLACK];

    // if enPassant field 
    if (info->EnPassantField != Bitboard::SQ_NONE)
    {
        // reset en passant zobrist
        info->key ^= Cache::GetZobristEnPassant(info->EnPassantField % 8);
        // UpdateEnPassantField
        info->EnPassantField = Bitboard::SQ_NONE;
    }
    

    //UpdateCounters
    ++PlyFromNull;
    info->FiftyMovesCounter = 0;

    //update material
    info->Material += (PiecesMaterial[CurrentColor][promotedPieceType] - PiecesMaterial[CurrentColor][PAWN]);

    // Change Color
    CurrentColor = ~CurrentColor;

    //Update zobrist curColor
    info->key ^= Cache::GetWhiteZobristSideMove();
}

void Position::MakeCapturePromotionMove(const Move::Move &move) {
    auto from = move.getFrom();
    auto to = move.getTo();
    auto opColor = ~CurrentColor;
    auto promotedPieceType = move.getPromotionPieceType();
    auto capturedPieceType = Types[to];

    //Update Pieces
    Pieces[CurrentColor][PAWN] &= ~Bitboard::sqBb[from];
    Pieces[CurrentColor][promotedPieceType] |= Bitboard::sqBb[to];
    Pieces[opColor][capturedPieceType] &= ~Bitboard::sqBb[to];

    //Update Types
    Types[from] = NONE;
    Types[to] = promotedPieceType;

    //Zobrist update Types
    info->key ^= Cache::GetZobristTable(CurrentColor, PAWN, from);
    info->key ^= Cache::GetZobristTable(CurrentColor, promotedPieceType, to);
    info->key ^= Cache::GetZobristTable(opColor, capturedPieceType, to);

    // reset Old zobrist castle rights 
    info->key ^= Cache::GetZobristCastleRights(info->castleRights);

    //Update castle rules (pawn can only capture op rook)
    info->castleRights &= ~castlingMasks[to];

    // set new zobrist castle rights 
    info->key ^= Cache::GetZobristCastleRights(info->castleRights);

    //UpdateAllPieces
    info->allPiecesByColor[CurrentColor] &= ~Bitboard::sqBb[from];
    info->allPiecesByColor[CurrentColor] |= Bitboard::sqBb[to];
    info->allPiecesByColor[opColor] &= ~Bitboard::sqBb[to];
    info->allPieces = info->allPiecesByColor[WHITE] | info->allPiecesByColor[BLACK];

    // if enPassant field 
    if (info->EnPassantField != Bitboard::SQ_NONE)
    {
        // reset en passant zobrist
        info->key ^= Cache::GetZobristEnPassant(info->EnPassantField % 8);
        //UpdateEnPassantField
        info->EnPassantField = Bitboard::SQ_NONE;
    }

    //update material
    info->Material += (PiecesMaterial[CurrentColor][promotedPieceType] + PiecesMaterial[CurrentColor][capturedPieceType] - PiecesMaterial[CurrentColor][PAWN]);

    //UpdateCounters
    ++PlyFromNull;
    info->FiftyMovesCounter = 0;

    // Change Color
    CurrentColor = ~CurrentColor;

    //Update zobrist curColor
    info->key ^= Cache::GetWhiteZobristSideMove();
}

void Position::MakeQuietMove(const Move::Move &move) {
    auto from = move.getFrom();
    auto to = move.getTo();
    auto movingPieceType = Types[from];
    auto opColor = ~CurrentColor;

    //Update Pieces
    Pieces[CurrentColor][movingPieceType] &= ~Bitboard::sqBb[from];
    Pieces[CurrentColor][movingPieceType] |= Bitboard::sqBb[to];

    //Update Types
    Types[from] = NONE;
    Types[to] = movingPieceType;

    //Zobrist update Types
    info->key ^= Cache::GetZobristTable(CurrentColor, movingPieceType, from);
    info->key ^= Cache::GetZobristTable(CurrentColor, movingPieceType, to);

    // reset Old zobrist castle rights 
    info->key ^= Cache::GetZobristCastleRights(info->castleRights);
    
    //Update castle rules(can only move king or rook)
    info->castleRights &= ~castlingMasks[from];

    // set new zobrist castle rights 
    info->key ^= Cache::GetZobristCastleRights(info->castleRights);

    //UpdateAllPieces
    info->allPiecesByColor[CurrentColor] &= ~Bitboard::sqBb[from];
    info->allPiecesByColor[CurrentColor] |= Bitboard::sqBb[to];
    info->allPieces = info->allPiecesByColor[WHITE] | info->allPiecesByColor[BLACK];

    //UpdateEnPassantField
    if (info->EnPassantField != Bitboard::SQ_NONE) {
        info->key ^= Cache::GetZobristEnPassant(info->EnPassantField % 8);
        info->EnPassantField = Bitboard::SQ_NONE;
    }
    if (movingPieceType == PAWN)
    {
        if (std::abs((int)from - (int)to) == 16)  // if pawn double push
        {
            uint8_t potentialEpSq = CurrentColor == WHITE ? to - 8 : to + 8;
            if(NonSlidingPieces::GetPawnAttacks(Pieces[opColor][PAWN],(PieceColor)opColor) & Bitboard::sqBb[potentialEpSq]) // IF en passant really possible
            { 
                info->EnPassantField = potentialEpSq;
                info->key ^= Cache::GetZobristEnPassant(potentialEpSq % 8);
            }
        }
    }

    //UpdateCounters
    ++PlyFromNull;
    if (movingPieceType == PAWN) info->FiftyMovesCounter = 0;
    else ++info->FiftyMovesCounter;

    // Change Color
    CurrentColor = ~CurrentColor;

    //Update zobrist curColor
    info->key ^= Cache::GetWhiteZobristSideMove();
}

void Position::UnMakeShortCastleMove(const Move::Move &move) {
    auto rookBeforeMoveSq = CurrentColor == WHITE ? Bitboard::H8 : Bitboard::H1;
    auto rookAfterMoveSq = CurrentColor == WHITE ? Bitboard::F8 : Bitboard::F1;
    UnMakeCastleMove(move,rookBeforeMoveSq,rookAfterMoveSq);
}

void Position::UnMakeLongCastleMove(const Move::Move &move) {
    auto rookBeforeMoveSq = CurrentColor == WHITE ? Bitboard::A8 : Bitboard::A1;
    auto rookAfterMoveSq = CurrentColor == WHITE ? Bitboard::D8 : Bitboard::D1;
    UnMakeCastleMove(move,rookBeforeMoveSq,rookAfterMoveSq);
}

void Position::UnMakeCastleMove(const Move::Move &move, Bitboard::Bitboard rookBeforeMoveSq,
    Bitboard::Bitboard rookAfterMoveSq) {
    auto from = move.getFrom();
    auto to = move.getTo();

    //Update CurrentColor
    CurrentColor = ~CurrentColor;

    //Update Counter
    --PlyFromNull;

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

}

void Position::UnMakeCaptureMove(const Move::Move &move) {
    auto from = move.getFrom();
    auto to = move.getTo();
    auto movingPieceType = Types[to];
    auto capturedPieceType = info->CapturedPiece;
    auto opColor = CurrentColor;

    //Update CurrentColor
    CurrentColor = ~CurrentColor;

    //Update Counter
    --PlyFromNull;

    //Update Pieces
    Pieces[CurrentColor][movingPieceType] &= ~Bitboard::sqBb[to];
    Pieces[CurrentColor][movingPieceType] |= Bitboard::sqBb[from];
    Pieces[opColor][capturedPieceType] |= Bitboard::sqBb[to];

    //Update Types
    Types[to] = capturedPieceType;
    Types[from] = movingPieceType;
}

void Position::UnMakeEnPassantMove(const Move::Move &move) {
    auto from = move.getFrom();
    auto to = move.getTo();
    auto opColor = CurrentColor;

    //Update CurrentColor
    CurrentColor = ~CurrentColor;

    auto capSq = CurrentColor == WHITE ? to - 8 : to + 8;

    //Update Counter
    --PlyFromNull;

    //Update Pieces
    Pieces[CurrentColor][PAWN] &= ~Bitboard::sqBb[to];
    Pieces[CurrentColor][PAWN] |= Bitboard::sqBb[from];
    Pieces[opColor][PAWN] |= Bitboard::sqBb[capSq];

    //Update Types
    Types[to] = NONE;
    Types[from] = PAWN;
    Types[capSq] = PAWN;
}

void Position::UnMakePromotionMove(const Move::Move &move) {
    auto from = move.getFrom();
    auto to = move.getTo();
    auto promotionPieceType = move.getPromotionPieceType();

    //Update CurrentColor
    CurrentColor = CurrentColor == WHITE ? BLACK : WHITE;

    //Update Counter
    --PlyFromNull;

    //Update Pieces
    Pieces[CurrentColor][promotionPieceType] &= ~Bitboard::sqBb[to];
    Pieces[CurrentColor][PAWN] |= Bitboard::sqBb[from];

    //Update Types
    Types[to] = NONE;
    Types[from] = PAWN;
}

void Position::UnMakeCapturePromotionMove(const Move::Move &move) {
    auto from = move.getFrom();
    auto to = move.getTo();
    auto opColor = CurrentColor;
    auto promotionPieceType = move.getPromotionPieceType();
    auto capturedPieceType = info->CapturedPiece;

    //Update CurrentColor
    CurrentColor = ~CurrentColor;

    //Update Counter
    --PlyFromNull;

    //Update Pieces
    Pieces[CurrentColor][promotionPieceType] &= ~Bitboard::sqBb[to];
    Pieces[CurrentColor][PAWN] |= Bitboard::sqBb[from];
    Pieces[opColor][capturedPieceType] |= Bitboard::sqBb[to];

    //Update Types
    Types[to] = capturedPieceType;
    Types[from] = PAWN;
}

void Position::UnMakeQuietMove(const Move::Move &move) {
    auto from = move.getFrom();
    auto to = move.getTo();
    auto movingPieceType = Types[to];

    //Update CurrentColor
    CurrentColor = ~CurrentColor;

    //Update Counter
    --PlyFromNull;

    //Update Pieces
    Pieces[CurrentColor][movingPieceType] &= ~Bitboard::sqBb[to];
    Pieces[CurrentColor][movingPieceType] |= Bitboard::sqBb[from];

    //Update Types
    Types[to] = NONE;
    Types[from] = movingPieceType;
}

void Position::SetCheckSquares()
{
    auto opColor = CurrentColor ^ 1;
    auto opKingSq = Bitboard::BitScanForward(Pieces[opColor][KING]);
    info->CheckSquares[PAWN] = Cache::GetPawnAttacks(CurrentColor,opKingSq);
    info->CheckSquares[KNIGHT] = Cache::GetKnightMoves(opKingSq);
    info->CheckSquares[BISHOP] = Cache::GetBishopMoves(opKingSq, info->allPieces);
    info->CheckSquares[ROOK] = Cache::GetRookMoves(opKingSq, info->allPieces);
    info->CheckSquares[QUEEN] = info->CheckSquares[BISHOP] | info->CheckSquares[ROOK];
    info->CheckSquares[KING] = Bitboard::ZERO;
}

void Position::SetBlockersPinners(uint32_t color)
{  
    auto opColor = color ^ 1;
    info->KingBlockers[color] = 0;
    info->Pinners[opColor] = 0;
    auto kingSq = GetKingSq(color);
    auto snipers = Cache::GetBishopMoves(kingSq,Bitboard::ZERO) & (Pieces[opColor][BISHOP] | Pieces[opColor][QUEEN]) |
                   Cache::GetRookMoves(kingSq,Bitboard::ZERO) & (Pieces[opColor][ROOK] | Pieces[opColor][QUEEN]);
    while (snipers) {
        auto sniperSq = Bitboard::BitScanForward(snipers);
        auto piecesBeetwenSniperAndKing = Cache::GetBetweenBb(kingSq,sniperSq) & info->allPieces;
        auto pieceCountOnLine = Bitboard::PopCount(piecesBeetwenSniperAndKing);
        if (pieceCountOnLine == 1)
        {
            info->KingBlockers[color] |= piecesBeetwenSniperAndKing;
            if(piecesBeetwenSniperAndKing & info->allPiecesByColor[color])
                info->Pinners[opColor] |= Bitboard::sqBb[sniperSq];
        }
        snipers &= snipers - 1;
    }
}

void Position::SetCheckers()
{
    info->Checkers = AttackersTo(~CurrentColor , GetKingSq(CurrentColor));
}

void Position::SetZobrist()
{
    for (int sq = 0; sq < 64; sq++)
    {
        auto type = Types[sq];
        auto color = Bitboard::sqBb[sq] & info->allPiecesByColor[WHITE] ? WHITE : BLACK;
        info->key ^= Cache::GetZobristTable(color,type,sq);
    }
    info->key ^= Cache::GetZobristCastleRights(info->castleRights);
    if (CurrentColor == WHITE) info->key ^= Cache::GetWhiteZobristSideMove();
    if(info->EnPassantField)
        info->key ^= Cache::GetZobristEnPassant(info->EnPassantField % 8);
}

Bitboard::Bitboard Position::AttackersTo(uint32_t attackerColor, int sq)
{
    auto opColor = attackerColor ^ 1;
    return (Cache::GetPawnAttacks(opColor, sq) & Pieces[attackerColor][PAWN]) |
           (Cache::GetKnightMoves(sq) & Pieces[attackerColor][KNIGHT]) |
           (Cache::GetBishopMoves(sq,info->allPieces) & (Pieces[attackerColor][BISHOP] | Pieces[attackerColor][QUEEN])) |
           (Cache::GetRookMoves(sq,info->allPieces) & (Pieces[attackerColor][ROOK] | Pieces[attackerColor][QUEEN])) |
           (Cache::GetKingMoves(sq) & Pieces[attackerColor][KING]);

}

void Position::PushUndoInfo(PositionInfo* pi) {
    std::memcpy(pi,info,offsetof(PositionInfo,CheckSquares));
    info->next = pi;
    pi->previous = info;
    info = pi;
}

void Position::PopUndoInfo() {
    info = info->previous;
}

Position::Position()
{
    // Очистка битбордов фигур
    for (int color = 0; color < 2; ++color)
        for (int piece = 0; piece < 6; ++piece)
            Pieces[color][piece] = Bitboard::ZERO;

    // Очистка типов фигур на клетках
    for (int sq = 0; sq < 64; ++sq)
        Types[sq] = NONE;

    CurrentColor = WHITE;
    PlyFromNull = 0;

    
    // Создаём базовый PositionInfo (корень undo-стека)
    info = nullptr; 
}

Position::Position(const std::string &Fen, PositionInfo* pi)
{
    SetPosition(Fen,pi);
    SetCheckSquares();
    SetBlockersPinners(WHITE);
    SetBlockersPinners(BLACK);
    SetCheckers();
    SetZobrist();
}

Position& Position::operator=(const Position& other) 
{
    if (this == &other)
        return *this;

    // Копируем простые поля
    CurrentColor = other.CurrentColor;
    PlyFromNull = other.PlyFromNull;

    // Копируем массивы
    std::memcpy(Pieces, other.Pieces, sizeof(Pieces));
    std::memcpy(Types, other.Types, sizeof(Types));
  
    // ВАЖНО: копируем указатель, а не данные
    info = other.info;

    return *this;
}

bool Position::operator==(const Position& other) const
{
    if (this == &other)
        return true;

    if (CurrentColor != other.CurrentColor)
        return false;

    if (PlyFromNull != other.PlyFromNull)
        return false;

    // Pieces
    for (int color = 0; color < 2; ++color)
        for (int piece = 0; piece < 6; ++piece)
            if (Pieces[color][piece] != other.Pieces[color][piece])
                return false;

    // Types
    for (int sq = 0; sq < 64; ++sq)
        if (Types[sq] != other.Types[sq])
            return false;

  
    // --- PositionInfo ---
    // сравниваем СОДЕРЖИМОЕ, а не адрес

    if (info->castleRights != other.info->castleRights)    return false;
    if (info->EnPassantField != other.info->EnPassantField)  return false;
    if (info->FiftyMovesCounter != other.info->FiftyMovesCounter) return false;

    if (info->allPieces != other.info->allPieces)
        return false;

    for (int c = 0; c < 2; ++c)
        if (info->allPiecesByColor[c] != other.info->allPiecesByColor[c])
            return false;

    return true;
}

bool Position::operator!=(const Position& other) const
{
    return !(*this == other);
}

void Position::SetPosition(const std::string& Fen, PositionInfo* pi)
{
    info = pi;
    std::istringstream iss(Fen);
    std::string piecePart;
    iss >> piecePart;

    // fill bitboards
    int currPos = 63;
    for (char ch : piecePart) {
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
            info->allPiecesByColor[color] |= Pieces[color][type];
            info->Material += PiecesMaterial[color][type];
            Types[currPos] = type;
            --currPos;
        }
    }
    info->allPieces = info->allPiecesByColor[WHITE] | info->allPiecesByColor[BLACK];

    // currColor
    std::string currColor;
    iss >> currColor;
    CurrentColor = currColor == "w" ? WHITE : BLACK;

    // castles
    std::string castles;
    iss >> castles;
    if (castles.find('K') != std::string::npos) info->castleRights |= WK;
    if (castles.find('Q') != std::string::npos) info->castleRights |= WQ;
    if (castles.find('k') != std::string::npos) info->castleRights |= BK;
    if (castles.find('q') != std::string::npos) info->castleRights |= BQ;


    // enPassant
    std::string enPassant;
    iss >> enPassant;
    if (enPassant == "-") info->EnPassantField = Bitboard::SQ_NONE;
    else {
        uint8_t file = 'h' - enPassant[0]; // h=0, g=1, ..., a=7
        uint8_t rank = enPassant[1] - '1'; // rank 1-8 → 0-7
        info->EnPassantField = rank * 8 + file;
    }

    // 50movesRuleCounter + MoveCounter
    iss >> info->FiftyMovesCounter;
    iss >> PlyFromNull;

    SetCheckSquares();
    SetBlockersPinners(WHITE);
    SetBlockersPinners(BLACK);
    SetCheckers();
    SetZobrist();
}

void Position::MakeMove(const Move::Move &move, PositionInfo* pi) {
    PushUndoInfo(pi);
    // TODO: FIX INNER FUNCTIONS (SWIPE TO ONLY 4 MOVETYPE)
    switch (move.GetType()) {
        case CASTLE: {
            MakeShortCastleMove(move);
            break;
        }
        case EN_PASSANT: {
            MakeEnPassantMove(move);
            break;
        }
        case PROMOTION: {
            MakePromotionMove(move);
            break;
        }
        default: {
            MakeQuietMove(move);
            break;
        }
    }
    SetCheckSquares();
    SetBlockersPinners(WHITE);
    SetBlockersPinners(BLACK);
    SetCheckers();
}

void Position::UnMakeMove(const Move::Move &move) {
    //TODO: FIX FOR 4 MOVETYPES
    PopUndoInfo();
    switch (move.GetType()) {
        case CASTLE: {
            UnMakeShortCastleMove(move);
            break;
        }
        case EN_PASSANT: {
            UnMakeEnPassantMove(move);
            break;
        }
        case PROMOTION: {
            UnMakePromotionMove(move);
            break;
        }
        default: {
            UnMakeQuietMove(move);
            break;
        }
    }
}

bool Position::IsCheck()
{
    return info->Checkers;
}

bool Position::IsLegal(const Move::Move &move) const {
    if (move.isShortCastle()) {
        return !(IsSquareUnderAttack(ShortCastleFieldsByColor[CurrentColor][0]) ||
                 IsSquareUnderAttack(ShortCastleFieldsByColor[CurrentColor][1]) ||
                 IsSquareUnderAttack(ShortCastleFieldsByColor[CurrentColor][2]));
    }
    if (move.isLongCastle())
    {
        return !(IsSquareUnderAttack(LongCastleFieldsByColor[CurrentColor][0]) ||
                 IsSquareUnderAttack(LongCastleFieldsByColor[CurrentColor][1]) ||
                 IsSquareUnderAttack(LongCastleFieldsByColor[CurrentColor][2]));
    }
    if (move.isEnPassant()) {
        auto kingSq = GetKingSq(CurrentColor);
        auto to = move.getTo();
        auto from = move.getFrom();
        auto capSq = CurrentColor == WHITE ? to - 8 : to + 8; 
        auto opColor = CurrentColor ^ 1;
        auto newBlockers = (info->allPieces & ~(Bitboard::sqBb[from] | Bitboard::sqBb[capSq])) | Bitboard::sqBb[to];
        return !(Cache::GetBishopMoves(kingSq,newBlockers) & (Pieces[opColor][QUEEN] | Pieces[opColor][BISHOP])) &&
               !(Cache::GetRookMoves(kingSq,newBlockers) & (Pieces[opColor][QUEEN] | Pieces[opColor][ROOK]));
    }
    if (Types[move.getFrom()] == KING) {
        return !IsSquareUnderAttack(move.getTo(), info->allPieces & ~Bitboard::sqBb[move.getFrom()] | Bitboard::sqBb[move.getTo()]);
    }
    bool isBlocker = Bitboard::sqBb[move.getFrom()] & info->KingBlockers[CurrentColor]; 
    if(!isBlocker) return true;
    return Bitboard::sqBb[GetKingSq(CurrentColor)] & Cache::GetLineBb(move.getFrom(),move.getTo());
    
}

bool Position::IsSquareUnderAttack(int sq, Bitboard::Bitboard newBlockers) const {
    auto blockers = newBlockers == Bitboard::ZERO ? info->allPieces : newBlockers;
    auto opColor = CurrentColor ^ 1;
    auto opQueens = Pieces[opColor][QUEEN];

    // pawnAttacks
    auto pawnAttacks = Cache::GetPawnAttacks(CurrentColor,sq);
    auto opPawns = Pieces[opColor][PAWN];
    if (pawnAttacks & opPawns) return true;

    //knightAttacks
    auto knightAttacks = Cache::GetKnightMoves(sq);
    auto opKnights = Pieces[opColor][KNIGHT];
    if (knightAttacks & opKnights) return true;

    //bishopAttacks
    auto bishopAttacks = Cache::GetBishopMoves(sq,blockers);
    auto opBishops = Pieces[opColor][BISHOP];
    if (bishopAttacks & (opBishops | opQueens)) return true;

    //rookAttacks
    auto rookAttacks = Cache::GetRookMoves(sq, blockers);
    auto opRooks = Pieces[opColor][ROOK];
    if (rookAttacks & (opRooks | opQueens)) return true;

    //kingAttacks
    auto kingAttacks = Cache::GetKingMoves(sq);
    auto opKing = Pieces[opColor][KING];
    if (kingAttacks & opKing) return true;

    return false;
}

bool Position::IsMoveGivesCheck(const Move::Move& move)
{
    constexpr Bitboard::Bitboard RookBbAfterShortCastleByColor[2] = {Bitboard::sqBb[Bitboard::F1],Bitboard::sqBb[Bitboard::F8]};
    constexpr Bitboard::Bitboard RookBbAfterLongCastleByColor[2] = {Bitboard::sqBb[Bitboard::D1],Bitboard::sqBb[Bitboard::D8]};
    auto from = move.getFrom();
    auto to = move.getTo();
    auto movingPieceType = Types[from];
    auto opColor = ~CurrentColor;
    auto opKingSq = GetKingSq(opColor);

    // if direct Check
    if(Bitboard::sqBb[to] & info->CheckSquares[movingPieceType]) return true;
    
    //if discovered Check (blocker go away from line between from and opkingsq )
    if( (Bitboard::sqBb[from] & info->KingBlockers[opColor]) && !(Cache::GetBetweenBb(from,opKingSq) & Bitboard::sqBb[to]) ) return true;

    // if captured pawn is blocker and our pawn dont become new blocker
    if (move.isEnPassant()) {
        auto capSq = CurrentColor == WHITE ? to - 8 : to + 8;
        if (Bitboard::sqBb[capSq] & info->KingBlockers[opColor])
            return !(Bitboard::sqBb[to] & Cache::GetBetweenBb(capSq,opKingSq));
    }

    // does a rook gives a check in castle?
    if (move.isShortCastle()) {
        return RookBbAfterShortCastleByColor[CurrentColor] & info->CheckSquares[ROOK];
    }

    if (move.isLongCastle()) {
        return RookBbAfterLongCastleByColor[CurrentColor] & info->CheckSquares[ROOK];
    }

    // if promoted pawn gives a check
    if (move.isPromotion()) {
        return Bitboard::sqBb[to] & info->CheckSquares[move.getPromotionPieceType()];
    }
}


void Position::ShowPos() const
{
    for (int i = 63; i >= 0; i--)
    {
        std::string currPiece = "";
        if (Types[i] == NONE)
        {
            currPiece = "___ ";
        }
        else {
            int color = Bitboard::sqBb[i] & Pieces[WHITE][Types[i]] ? WHITE : BLACK;
            if (color == WHITE) currPiece += "W_";
            else currPiece += "B_";
            switch (Types[i]) {
            case KING: currPiece += "K "; break;
            case QUEEN: currPiece += "Q "; break;
            case ROOK: currPiece += "R "; break;
            case BISHOP: currPiece += "B "; break;
            case KNIGHT: currPiece += "N "; break;
            case PAWN: currPiece += "P "; break;
            }
        } 
        std::cout<<currPiece;
        if(i % 8 == 0) std::cout<<"\n";
    }
}



