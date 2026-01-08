//
// Created by nelep on 25.11.2025.
//

#ifndef ENGINE_TYPES_H
#define ENGINE_TYPES_H

enum PieceColor{WHITE, BLACK};

enum PieceType{ PAWN, KNIGHT, BISHOP, ROOK, QUEEN, KING, NONE};


typedef uint64_t Key;
typedef uint64_t EvalValue;

constexpr int8_t PAWN_COST = 1;
constexpr int8_t KNIGHT_COST = 3;
constexpr int8_t BISHOP_COST = 3;
constexpr int8_t ROOK_COST = 5;
constexpr int8_t QUEEN_COST = 9;
constexpr int8_t KING_COST = 100;

constexpr int8_t PiecesMaterial[2][6] = {{PAWN_COST,KNIGHT_COST,BISHOP_COST,ROOK_COST,QUEEN_COST,KING_COST},
										 {-PAWN_COST,-KNIGHT_COST,-BISHOP_COST,-ROOK_COST,-QUEEN_COST,-KING_COST}};
										 

#endif //ENGINE_TYPES_H