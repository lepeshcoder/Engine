//
// Created by nelep on 20.12.2025.
//

#ifndef ENGINE_PERFT_H
#define ENGINE_PERFT_H
#include "../../MoveGeneration/Generators/LegalGenerator/LegalGenerator.h"
#include "../../Position/Position.h"
#include <cassert> 
#include<iostream>
#include<vector>
#include<stack>


struct PerftResult {
    uint64_t CapturesCount = 0;
    uint64_t EnPassantCount = 0;
    uint64_t CastlesCount = 0;
    uint64_t PromotionsCount = 0;
    uint64_t ChecksCount = 0;
    uint64_t TotalCount = 0;
    uint64_t DiscoveryCheckCount = 0;
    uint64_t DoubleCheckCount = 0;
    uint64_t CheckMateCount = 0;

    void Show() const {
        std::cout << "Total Positions: " << TotalCount << std::endl;
        std::cout << "Captures: "<<CapturesCount<<std::endl;
        std::cout << "EnPassant: "<<EnPassantCount<<std::endl;
        std::cout << "Castles: "<<CastlesCount<<std::endl;
        std::cout << "Promotions: "<<PromotionsCount<<std::endl;
        std::cout << "ChecksCount: "<<ChecksCount<<std::endl;
        std::cout << "DiscoveryChecksCount: "<< DiscoveryCheckCount <<std::endl;
        std::cout << "DoubleChecksCount: "<< DoubleCheckCount <<std::endl;
        std::cout << "CheckMateCount: "<<CheckMateCount<<std::endl;
    }
};

class Perft {

private:
    PositionInfo pi;
    PerftResult res;
    Position pos;
    std::stack<Move::Move> moveSequence;
    int depth = 0;
    bool isFullInfo = false;

    void myaso(int depth);

    void ShowMoveSequence();
public:
    Perft(const std::string& fen, int depth);

    PerftResult Execute(bool isFullinfo = false);

    void ExecuteDivide();
};


#endif //ENGINE_PERFT_H