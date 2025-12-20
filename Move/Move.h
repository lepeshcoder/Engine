    //
    // Created by nelep on 02.12.2025.
    //

    #ifndef ENGINE_MOVE_H
    #define ENGINE_MOVE_H

    #include <cstdint>
#include <string>

#include "../Types.h"


    // Format : | 000000  |       000          |       000       |        000        |      0      |      0       |       0       |      0      |     0     | 000000 | 000000 |
    // Goal     | unused  | promotionPieceType | movingPieceType | capturedPieceType | IsEnPassant | IsLongCastle | IsShortCastle | IsPromotion | IsCapture | FromSq |  ToSq  |
    // Size     |  6 bit  |       3 bit        |      3 bit      |       3 bit       |     1bit    |    1 bit     |     1 bit     |    1 bit    |   1 bit   | 6 bit  | 6 bit  |
    // Offset   | 26 bit  |       23 bit       |     20 bit      |       17 bit      |    16 bit   |    15 bit    |     14 bit    |    13 bit   |   12 bit  | 6 bit  | 0 bit  |
    // Union                                                                         |                            Flags                                     |
    namespace Move {

        constexpr uint32_t ToSqShift = 0;
        constexpr uint32_t FromSqShift = 6;
        constexpr uint32_t FlagsShift = 12;
        constexpr uint32_t CapturedPieceTypeShift = 17;
        constexpr uint32_t MovingPieceTypeShift = 20;
        constexpr uint32_t PromotionPieceTypeShift = 23;

        constexpr uint32_t OneBitMask = 0x1;
        constexpr uint32_t ThreeBitMask = 0x7;
        constexpr uint32_t FiveBitMask = 0x1F;
        constexpr uint32_t SixBitMask = 0x3F;

        enum Flags{ CaptureFlag = 1, PromotionFlag = 1 << 1, ShortCastleFlag = 1 << 2,
            LongCastleFlag = 1 << 3, EnPassantFlag = 1 << 4 };

        class Move {
        private:
            uint32_t move;
            std::string sq(int sq) const;

        public:
            Move(uint32_t from, uint32_t to, uint32_t flags,
                uint32_t movingPieceType, uint32_t capturedPieceType, uint32_t promotionPieceType);

            uint32_t getTo() const { return move & SixBitMask; }
            uint32_t getFrom() const { return move >> FromSqShift & SixBitMask; }
            uint32_t getFlags() const { return move >> FlagsShift & FiveBitMask; }

            PieceType getCapturedPieceType() const { return PieceType(move >> CapturedPieceTypeShift & ThreeBitMask); }
            PieceType getMovingPieceType() const { return PieceType(move >> MovingPieceTypeShift & ThreeBitMask); }
            PieceType getPromotionPieceType() const { return PieceType(move >> PromotionPieceTypeShift & ThreeBitMask); }

            bool isCapture() const { return getFlags() & CaptureFlag; }
            bool isPromotion() const { return getFlags() & PromotionFlag; }
            bool isShortCastle() const { return getFlags() & ShortCastleFlag; }
            bool isLongCastle() const { return getFlags() & LongCastleFlag; };
            bool isEnPassant() const { return getFlags() & EnPassantFlag; }

           void ShowMove() const;

        };
    }


    #endif //ENGINE_MOVE_H