    //
    // Created by nelep on 02.12.2025.
    //

    #ifndef ENGINE_MOVE_H
    #define ENGINE_MOVE_H

    #include <cstdint>
    #include <string>
    #include "../Types.h"


    // Format : |            00              |        00          | 000000 | 000000 |
    // Goal     | Special flags for moveType | PromotionPieceType | FromSq |  ToSq  |
    // Size     |           2 bit            |       2 bit        | 6 bit  | 6 bit  |
    namespace Move {

        constexpr uint16_t ToSqShift = 0;
        constexpr uint16_t FromSqShift = 6;
        constexpr uint16_t PromotionPieceTypeShift = 12;
        constexpr uint16_t FlagsShift = 14;

        constexpr uint16_t TwoBitMask = 0x3;
        constexpr uint16_t SixBitMask = 0x3F;

        constexpr uint16_t QuietMove = 0;
        constexpr uint16_t PromotionMove = 0b01 << FlagsShift;
        constexpr uint16_t EnPassantMove = 0b10 << FlagsShift;
        constexpr uint16_t CastleMove = 0b11 << FlagsShift;
        constexpr uint16_t FlagsMask = CastleMove;

        class Move{
        protected:
            uint16_t move;
            
        public:
            Move() = default;
            constexpr explicit Move(uint16_t d) : move(d) {}

           
            constexpr Square getTo() const noexcept { return Square(move & SixBitMask); }
            constexpr Square getFrom() const noexcept { return Square((move >> FromSqShift) & SixBitMask); }
            constexpr PieceType getPromotionPieceType() const noexcept { return PieceType(((move >> PromotionPieceTypeShift) & TwoBitMask) + KNIGHT); }
            
            constexpr MoveType GetType() const noexcept { return MoveType(move >> FlagsShift);}
            constexpr bool isPromotion() const noexcept { return (move & FlagsMask) == PromotionMove; }
            constexpr bool isCastle() const noexcept { return (move & FlagsMask) == CastleMove; }
            constexpr bool isShortCastle() const noexcept { return isCastle() && FileOf(getTo()) == G ; }
            constexpr bool isLongCastle() const noexcept { return isCastle() && FileOf(getTo()) == C; }
            constexpr bool isEnPassant() const noexcept { return (move & FlagsMask) == EnPassantMove; }
            
            constexpr uint16_t GetRaw() const noexcept { return move; }

            static constexpr Move MakeMove(Square from, Square to) noexcept { return Move(to | from << FromSqShift); };
            static constexpr Move MakePromotion(Square from, Square to, PieceType promotionType) noexcept { return Move(to | from << FromSqShift | ((promotionType - KNIGHT) << PromotionPieceTypeShift) | PromotionMove); };
            static constexpr Move MakeEnPassant(Square from, Square to) noexcept { return Move(to | from << FromSqShift | EnPassantMove); };
            static constexpr Move MakeCastle(Square from, Square to) noexcept { return Move(to | from << FromSqShift | CastleMove); };


            void ShowMove() const;
            static std::string sq(int sq);
        };

        class ExtMove : public  Move {
        private:
            EvalValue score = 0;
        public:
            inline EvalValue GetScore() const {return score;}
            inline void operator=(Move move) { this->move = move.GetRaw(); }
        };
        inline bool operator< (const ExtMove& a, const ExtMove& b) { return a.GetScore() < b.GetScore(); }
    }


    #endif //ENGINE_MOVE_H