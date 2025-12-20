#include <iostream>
#include "Bitboard/Bitboard.h"
#include "MoveGeneration/Cache/Cache.h"
#include "MoveGeneration/Generators/LegalGenerator/LegalGenerator.h"
#include "Position/Position.h"
#include "Utils/Perft/Perft.h"
#include "Utils/Timer/Timer.h"


int main() {

    Cache::InitCache();

    Timer timer;

    Perft perft(5);

    timer.start();
    int positions = perft.Execute();
    auto duration = timer.stop();
    std::cout<<"positions: "<<positions <<"\n"<<"time: "<<duration<<" mcs.";





    // for (int sq = 0; sq < 64; sq++) {
    //     std::cout<<"sq: "<<sq<<":\n"<<Bitboard::BitboardToString(Bitboard::ONE << sq);
    //     std::cout<<"attacks To Sq:\n"<<Bitboard::BitboardToString(pos.AttacksTo[sq]);
    //     std::cout<<"attacks from Sq:\n"<<Bitboard::BitboardToString(pos.AttacksFrom[sq]);
    // }

    //std::cout<<Bitboard::BitScanForward(pos.GetPieceBitboard(WHITE,QUEEN));


    /*
    std::string request;

    while (std::getline(std::cin, request)) {
        std::string response(request.rbegin(), request.rend());
        std::cout << response << std::endl;
        std::cout.flush();
    }*/

    Cache::ClearCache();
}
