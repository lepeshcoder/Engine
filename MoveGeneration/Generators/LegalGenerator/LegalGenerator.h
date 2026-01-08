//
// Created by nelep on 03.12.2025.
//

#ifndef ENGINE_LEGALGENERATOR_H
#define ENGINE_LEGALGENERATOR_H
#include "../../Cache/Cache.h"
#include "../../../Position/Position.h"
#include "../../../build/MoveList.h"






class LegalGenerator{

public:
   LegalGenerator() = delete;

   static void GenerateChecks(const Position& pos, MoveList& moveList);

   static void GenerateCaptures(const Position& pos, MoveList& moveList);
        
   static void GenerateLegalMoves(const Position& pos, MoveList& moveList);

    // Generate pseudolegal castles
   static void GenerateCastles(const Position& pos, MoveList& moveList);
    
    // generate all pawns pseudolegal moves
   static void GeneratePawnMoves(const Position& pos, MoveList& moveList, Bitboard::Bitboard target = Bitboard::ALLONE);

    // generate all pseudolegal given piece type moves (excludind pawns)
   static void GeneratePieceMove(const Position& pos, MoveList& moveList, uint8_t pieceType, Bitboard::Bitboard target = Bitboard::ALLONE);

    // generate all moves that erase check in position
   static void GenerateEvasions(const Position& pos, MoveList& moveList);
   
    // generate all pseudolegal moves( checks not handled)
   static void GenerateAllMoves(const Position& pos, MoveList& moveList);

   static void ComparePositions(const Position& a, const Position& b);
};


#endif //ENGINE_LEGALGENERATOR_H