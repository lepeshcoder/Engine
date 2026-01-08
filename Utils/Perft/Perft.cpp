//
// Created by nelep on 20.12.2025.
//

#include "Perft.h"

void Perft::myaso(int depth) {
    if (depth == 0) {
        ++res.TotalCount;
        return;
    }
    MoveList moves;
    LegalGenerator::GenerateLegalMoves(pos,moves);
    for (auto move = moves.Begin() ; move != moves.End(); move++)
    {
        // делаем ход
        PositionInfo pi;
      
        pos.MakeMove(*move, &pi);

        //if (isFullInfo && depth == 1) {                                                 // после этого хода            
        //    if (move.isCapture()) ++res.CapturesCount;                    // увеличиваем счётчики ходов
        //    if (move.isPromotion()) ++res.PromotionsCount;
        //    if (move.isLongCastle() || move.isShortCastle()) ++res.CastlesCount;
        //    if (move.isEnPassant()) ++res.EnPassantCount;
        //    if (pos.IsCheck())
        //    {
        //        ++res.ChecksCount;
        //        if (Bitboard::PopCount(pos.GetCheckers()) == 2)
        //        {
        //            ++res.DoubleCheckCount;
        //        }
        //        else {
        //            if(!(Bitboard::sqBb[move.getTo()] & pos.GetCheckers())) ++res.DiscoveryCheckCount;
        //        }
        //        MoveList movesInCheck{};
        //        gen.GenerateLegalMoves(pos,movesInCheck);
        //        if (movesInCheck.size == 0)
        //        {
        //            ++res.CheckMateCount;
        //        }
        //    }
        //}

        myaso(depth - 1);
        pos.UnMakeMove(*move);    
    }
}

void Perft::ShowMoveSequence()
{
    for (auto& move : moveSequence._Get_container())
    {
        move.ShowMove();
    }
}

Perft::Perft(const std::string& fen, const int depth) {
    pos.SetPosition(fen,&pi);
    this->depth = depth;
}

PerftResult Perft::Execute(bool isFullinfo) {
    this->isFullInfo = isFullinfo;
    myaso(depth);
    return res;
}

void Perft::ExecuteDivide()
{
    MoveList moves;
    uint64_t totalCount = 0;
    LegalGenerator::GenerateLegalMoves(pos,moves);
    for (auto move = moves.Begin(); move != moves.End(); move++)
    {
        PositionInfo pi;
        std::cout<<Move::Move::sq(move->getFrom()) <<"->" << Move::Move::sq(move->getTo())<<" : ";
        pos.MakeMove(*move,&pi);
        myaso(depth - 1);
        pos.UnMakeMove(*move);
        std::cout<<res.TotalCount<<"\n";
        totalCount+=res.TotalCount;
        res.TotalCount = 0;
    }
    std::cout<<"total count: "<<totalCount<<"\n";
}
