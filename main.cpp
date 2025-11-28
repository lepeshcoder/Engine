#include <iostream>
#include "Bitboard/Bitboard.h"
#include "MoveGeneration/Cache/Cache.h"
#include "Utils/Timer/Timer.h"


int main() {


    Timer timer;
    timer.start();
    Cache pieceCache;




    int pieceSq = 26;
    std::cout<<"piece: \n"<<Bitboard::BitboardToString(Bitboard::ONE<<pieceSq);

    Bitboard::Bitboard blockers = 0x26922446363547;
    std::cout<<"blockers: \n"<<Bitboard::BitboardToString(blockers);

    Bitboard::Bitboard attacks = pieceCache.GetQueenMoves(pieceSq,blockers);
    std::cout<<"attacks: \n"<<Bitboard::BitboardToString(attacks);

    auto duration = timer.stop();
    std::cout<<"Elapsed: " << duration <<"microseconds\n";


    int a;
    std::cin>>a;










    /*
    std::string request;

    while (std::getline(std::cin, request)) {
        std::string response(request.rbegin(), request.rend());
        std::cout << response << std::endl;
        std::cout.flush();
    }*/
}
