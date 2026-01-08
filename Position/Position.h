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
#include <cassert> 
#include<iostream>


 constexpr int MAX_PLY = 256;
 constexpr int8_t WK = 1;
 constexpr int8_t WQ = 2;
 constexpr int8_t BK = 4;
 constexpr int8_t BQ = 8;


 constexpr int8_t castlingMasks[64] = {
    WK,0,0,WK | WQ,0,0,0,WQ,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
    BK,0,0,BK | BQ,0,0,0,BQ,
};

struct PositionInfo {

    PositionInfo() = default;

    //Field that copied when makemove
    Bitboard::Bitboard allPiecesByColor[2]{Bitboard::ZERO}; 
    Bitboard::Bitboard allPieces = Bitboard::ZERO;
    int8_t castleRights = 0;  
    int8_t FiftyMovesCounter = 0;
    int8_t EnPassantField = Bitboard::SQ_NONE; 
    EvalValue Material = 0;                      // material value of position
    Key key;                                     // zobrist hash value of position  
    // FIELDS THAT DONT COPY WHEN MAKEMOVE BEACUSE RECALCULATED
    
    Bitboard::Bitboard CheckSquares[6]{Bitboard::ZERO}; // array of Bitboards that represent from what square [pieceType] can do a check
    Bitboard::Bitboard Checkers = Bitboard::ZERO;  // bitboard that represent pieces that gives check in position
    Bitboard::Bitboard Pinners[2]{Bitboard::ZERO}; // bitboard that represent [color] pieces that pins [~color] pieces
    Bitboard::Bitboard KingBlockers[2]{Bitboard::ZERO}; // bitboard that represent pieces that are  blocks [color] king from [~color] pinners
    PositionInfo *next = nullptr , *previous = nullptr;
};



class Position {
private:
    // fields that not restore in UnmakeMove from PositionInfo struct
    Bitboard::Bitboard Pieces[2][6]{Bitboard::ZERO};     // stores bitbord representation of position for every color and piecetype
    int8_t Types[64]{ NONE }; // stores info about what piece type on this square
    PieceColor CurrentColor = WHITE;  
    int PlyFromNull = 0;
    int8_t kingsSq[2]{ 0 };

    PositionInfo* info;

    #pragma region PrivateMethods
    // for makeMove
    void MakeShortCastleMove(const Move::Move& move);
    void MakeLongCastleMove(const Move::Move& move);
    void MakeCastleMove(const Move::Move& move,uint8_t rookBeforeMoveSq, uint8_t rookAfterMoveSq);
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

    // checking
    void SetCheckSquares();
    void SetBlockersPinners(uint32_t color);
    void SetCheckers();

    //Hash
    void SetZobrist();

    // allPeaces of color that attacks sq
    Bitboard::Bitboard AttackersTo(uint32_t attackerColor, int sq);

    // for undoInfoStack
    void PushUndoInfo(PositionInfo* pi);
    void PopUndoInfo();


    #pragma endregion

public:

    Position();
    Position(const std::string &Fen, PositionInfo* pi);

    //Operators
    Position& operator=(const Position& other);
    bool operator==(const Position& other) const;
    bool operator!=(const Position& other) const;
  
    void SetPosition(const std::string& Fen, PositionInfo* pi);

    // MAKE/UNMAKE
    void MakeMove(const Move::Move& move, PositionInfo* pi);

    void UnMakeMove(const Move::Move& move);

    // ADDITIONAL
    bool IsCheck();

    bool IsLegal(const Move::Move& move) const;

    bool IsSquareUnderAttack(int sq, Bitboard::Bitboard newBlockers = Bitboard::ZERO) const;

    bool IsMoveGivesCheck(const Move::Move& move);




    // GETTERS
    PieceColor GetCurrentColor() const { return CurrentColor;}

    Bitboard::Bitboard GetPieceBitboard(int color, int pieceType) const { return Pieces[color][pieceType]; }

    Bitboard::Bitboard GetPiecesByColor(int color) const { return info->allPiecesByColor[color]; }

    Bitboard::Bitboard GetAllPieces() const { return info->allPieces;}

    uint8_t GetPieceTypeBySquare(int sq) const { return Types[sq]; };

    uint8_t GetEnPassantField()const { return this->info->EnPassantField; }

    bool IsShortCastleAvailable(int color) const {return color == WHITE ? info->castleRights & WK : info->castleRights & BK;}

    bool IsLongCastleAvailable(int color) const {return color == WHITE ? info->castleRights & WQ : info->castleRights & BQ;}

    uint8_t GetKingSq(int color) const {return kingsSq[color];}

    Bitboard::Bitboard GetCheckers() const { return info->Checkers; }

    Bitboard::Bitboard GetBlockers() const { return info->KingBlockers[CurrentColor]; }

    int32_t GetMaterial() const { return info->Material;}




    // print pos in console
    void ShowPos() const;



    //Constants
    static constexpr int8_t ShortCastleFieldsByColor[2][3] = {{Bitboard::E1,Bitboard::F1,Bitboard::G1},{Bitboard::E8,Bitboard::F8,Bitboard::G8}};
    static constexpr int8_t LongCastleFieldsByColor[2][3] = {{Bitboard::E1,Bitboard::D1,Bitboard::C1},{Bitboard::E8,Bitboard::D8,Bitboard::C8}};
};



#endif //ENGINE_POSITION_H