//
// Created by nelep on 25.11.2025.
//

#ifndef ENGINE_POSITION_H
#define ENGINE_POSITION_H
#include <string>

#include "../Types.h"
#include "../Bitboard/Bitboard.h"
#include <sstream>
#include"../Move/Move.h"
#include "../MoveGeneration/Cache/Cache.h"

struct UndoInfo {
    int8_t castleRights;
    int8_t fiftyMovesCounter;
    int8_t enPassantField;
};

class Position {
private:

    Bitboard::Bitboard Pieces[2][6]{};     // stores bitbord representation of position for every color and piecetype

    int Types[64]{NONE}; // stores info about what piece type on this square

    int kingsSq[2];
    int8_t castleRights = 0;

    Bitboard::Bitboard allPiecesByColor[2]{Bitboard::ZERO};
    Bitboard::Bitboard allPieces = Bitboard::ZERO;

    PieceColor CurrentColor;

    int MoveCounter;
    int8_t FiftyMovesCounter;
    int8_t EnPassantField;

    static constexpr int MAX_PLY = 256;
    UndoInfo undoStack[MAX_PLY];
    int currentPly = 0;

    static constexpr int8_t WK = 1;
    static constexpr int8_t WQ = 2;
    static constexpr int8_t BK = 4;
    static constexpr int8_t BQ = 8;

    static constexpr int8_t castlingMasks[64] = {
        WK,0,0,WK|WQ,0,0,0,WQ,
        0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,
        BK,0,0,BK|BQ,0,0,0,BQ,
    };
    #pragma region PrivateMethods
    // for makeMove
    void MakeShortCastleMove(const Move::Move& move);
    void MakeLongCastleMove(const Move::Move& move);
    void MakeCastleMove(const Move::Move& move, Bitboard::Bitboard rookBeforeMoveSq, Bitboard::Bitboard rookAfterMoveSq);
    void MakeCaptureMove(const Move::Move& move);
    void MakeEnPassantMove(const Move::Move& move);
    void MakePromotionMove(const Move::Move& move);
    void MakeCapturePromotionMove(const Move::Move& move);
    void MakeQuietMove(const Move::Move& move);

    // for unMakeMove
    void UnMakeShortCastleMove(const Move::Move& move);
    void UnMakeLongCastleMove(const Move::Move& move);
    void UnMakeCastleMove(const Move::Move& move, Bitboard::Bitboard rookBeforeMoveSq, Bitboard::Bitboard rookAfterMoveSq);
    void UnMakeCaptureMove(const Move::Move& move);
    void UnMakeEnPassantMove(const Move::Move& move);
    void UnMakePromotionMove(const Move::Move& move);
    void UnMakeCapturePromotionMove(const Move::Move& move);
    void UnMakeQuietMove(const Move::Move& move);

    // for undoInfoStack
    void PushUndoInfo();
    UndoInfo& PopUndoInfo();

    #pragma endregion

public:

    Position(const std::string &Fen);

    // MAKE/UNMAKE
    void MakeMove(const Move::Move& move);

    void UnMakeMove(const Move::Move& move);

    // ADDITIONAL
    bool IsLegal(const Move::Move& move);

    bool IsSquareUnderAttack(int sq) const;

    // GETTERS
    PieceColor GetCurrentColor() const { return CurrentColor;}

    Bitboard::Bitboard GetPieceBitboard(int color, int pieceType) const { return Pieces[color][pieceType]; }

    Bitboard::Bitboard GetPiecesByColor(int color) const { return allPiecesByColor[color]; }

    Bitboard::Bitboard GetAllPieces() const { return allPieces;}

    int GetPieceTypeBySquare(int sq) const { return Types[sq]; };

    int GetEnPassantField()const { return EnPassantField; }

    bool IsShortCastleAvailable(int color) const {return color == WHITE ? castleRights & WK : castleRights & BK;}

    bool IsLongCastleAvailable(int color) const {return color == WHITE ? castleRights & WQ : castleRights & BQ;}

    int GetKingSq(int color) const;


};


#endif //ENGINE_POSITION_H