//
// Created by nelep on 03.12.2025.
//

#ifndef ENGINE_LEGALGENERATOR_H
#define ENGINE_LEGALGENERATOR_H
#include "../../Cache/Cache.h"
#include "../../../Position/Position.h"
#include <unordered_map>


struct MoveList {
    static constexpr uint8_t maxSize = 255;   
    Move::Move moves[maxSize];
    uint8_t size = 0;
    

    inline void Push(const Move::Move& move) {
        moves[size++] = move;
    }
    inline Move::Move* End() {
        return &moves[size];
    }
};


class LegalGenerator{

private:
    

public:
    
    struct M {
        uint8_t from;
        uint8_t to;
    };

    uint16_t map[64][64]{0};

    uint64_t count = 0;
    //
    void GenerateLegalMoves(const Position& pos, MoveList& moveList);

    // Generate pseudolegal castles
    void GenerateCastles(const Position& pos, MoveList& moveList);
    
    // generate all pawns pseudolegal moves
    void GeneratePawnMoves(const Position& pos, MoveList& moveList, Bitboard::Bitboard target = Bitboard::ALLONE);

    // generate all pseudolegal given piece type moves (excludind pawns)
    void GeneratePieceMove(const Position& pos, MoveList& moveList, uint32_t pieceType, Bitboard::Bitboard target = Bitboard::ALLONE);

    // generate all moves that erase check in position
    void GenerateEvasions(const Position& pos, MoveList& moveList);

    
    // generate all pseudolegal moves( checks not handled)
    void GenerateAllMoves(const Position& pos, MoveList& moveList);

    void ComparePositions(const Position& a, const Position& b);
};


#endif //ENGINE_LEGALGENERATOR_H