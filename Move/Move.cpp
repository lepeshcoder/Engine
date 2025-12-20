//
// Created by nelep on 02.12.2025.
//

#include "Move.h"

#include <iostream>
#include <istream>


std::string Move::Move::sq(int sq) const {
    const std::string rows[8] = {"1","2","3","4","5","6","7","8"};
    const std::string cols[8] = {"h", "g","f","e","d","c","b","a"};
    return cols[sq % 8] + rows[sq / 8];
}

Move::Move::Move(uint32_t from, uint32_t to, uint32_t flags, uint32_t movingPieceType, uint32_t capturedPieceType,
                 uint32_t promotionPieceType) {
    move = from << FromSqShift | to << ToSqShift | flags << FlagsShift |
        movingPieceType  << MovingPieceTypeShift |
            capturedPieceType  << CapturedPieceTypeShift |
                promotionPieceType  << PromotionPieceTypeShift;
}

void Move::Move::ShowMove() const {
    const std::string types[7] = {"PAWN", "KNIGHT", "BISHOP", "ROOK", "QUEEN", "KING", "NONE"};


    std::cout<<"From: "<<sq(getFrom())<<std::endl;
    std::cout<<"To: "<<sq(getTo())<<std::endl;
    std::cout<<"IsCapture: "<<isCapture()<<std::endl;
    std::cout<<"IsPromotion: "<<isPromotion()<<std::endl;
    std::cout<<"IsLongCastle: "<<isLongCastle()<<std::endl;
    std::cout<<"IsShortCastle: "<<isShortCastle()<<std::endl;
    std::cout<<"IsEnPassant: "<<isEnPassant()<<std::endl;
    std::cout<<"CapturedPieceType: "<<types[getCapturedPieceType()]<<std::endl;
    std::cout<<"MovingPieceType: "<<types[getMovingPieceType()]<<std::endl;
    std::cout<<"PromotedPieceType: "<<types[getPromotionPieceType()]<<std::endl<<std::endl;
}


