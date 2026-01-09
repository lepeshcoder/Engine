//
// Created by nelep on 25.11.2025.
//

#ifndef ENGINE_TYPES_H
#define ENGINE_TYPES_H

enum PieceColor{ WHITE, BLACK};

inline PieceColor operator~(PieceColor color) noexcept {
	return PieceColor((int)color ^ 1);
}



enum PieceType{ PAWN, KNIGHT, BISHOP, ROOK, QUEEN, KING, NONE};

enum Square : int32_t {
	H1,G1,F1,E1,D1,C1,B1,A1,
	H2,G2,F2,E2,D2,C2,B2,A2,
	H3,G3,F3,E3,D3,C3,B3,A3,
	H4,G4,F4,E4,D4,C4,B4,A4,
	H5,G5,F5,E5,D5,C5,B5,A5,
	H6,G6,F6,E6,D6,C6,B6,A6,
	H7,G7,F7,E7,D7,C7,B7,A7,
	H8,G8,F8,E8,D8,C8,B8,A8,
	SQ_NONE
};

enum File{ H,G,F,E,D,C,B,A};
enum Rank{ Rank1,Rank2,Rank3,Rank4,Rank5,Rank6,Rank7,Rank8};

inline File FileOf(Square sq) noexcept { return File((int)sq % 8); }
inline Rank RankOf(Square sq) noexcept { return Rank((int)sq / 8 ); }

enum MoveType : int {
	QUIET,
	PROMOTION,
	EN_PASSANT,
	CASTLE
};

typedef uint64_t Key;
typedef uint16_t EvalValue;

constexpr EvalValue PAWN_COST = 1;
constexpr EvalValue KNIGHT_COST = 3;
constexpr EvalValue BISHOP_COST = 3;
constexpr EvalValue ROOK_COST = 5;
constexpr EvalValue QUEEN_COST = 9;
constexpr EvalValue KING_COST = 100;

constexpr EvalValue PiecesMaterial[2][6] = {{PAWN_COST,KNIGHT_COST,BISHOP_COST,ROOK_COST,QUEEN_COST,KING_COST},
										 {-PAWN_COST,-KNIGHT_COST,-BISHOP_COST,-ROOK_COST,-QUEEN_COST,-KING_COST}};


										 

#endif //ENGINE_TYPES_H