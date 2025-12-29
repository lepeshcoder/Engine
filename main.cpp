#include <iostream>
#include "Bitboard/Bitboard.h"
#include "MoveGeneration/Cache/Cache.h"
#include "MoveGeneration/Generators/LegalGenerator/LegalGenerator.h"
#include "Position/Position.h"
#include "Utils/Perft/Perft.h"
#include "Utils/Timer/Timer.h"



int main() {


   Cache::InitCache();

 /*  std::string request;
   
   while (std::getline(std::cin, request)) {
       if (request == "isready")
       {
            std::cout<<"uciok\n";
            std::cout.flush();
       }
   }*/




    std::string fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
    std::string pos1fen = "r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10";

    Timer timer;
    

    Perft perft(pos1fen,6);
    timer.start();
    perft.ExecuteDivide();
    //auto perftResult = perft.Execute(false);
    auto duration = timer.stop();
    std::cout<<"time: "<<duration / 1000 <<" ms.";
   // perftResult.Show();





  
    
   

    Cache::ClearCache();
}
