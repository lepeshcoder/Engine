#include <iostream>
#include "Bitboard/Bitboard.h"
#include "MoveGeneration/Cache/Cache.h"
#include "MoveGeneration/Generators/LegalGenerator/LegalGenerator.h"
#include "Position/Position.h"
#include "Utils/Perft/Perft.h"
#include "Utils/Timer/Timer.h"
#include "build/Search.h"
#include "build/TranspositionTable.h"
#include "build/MoveList.h"



int main() {
    
    // TODO
    // iteraive depening
    // TT
    // NEGASCOUT
    // PV TABLES
    // QUICKSCENE
    // EVALUATE FUNC,
    // MOVE ORDERING
    // HISTORIES (#SKULL)(SKULL)(#SKULL)

    int a = 5;
    a = a++;
    std::cout<<a;
    
   Cache::InitCache();
   
   std::cout<<a;
   MoveList moveList;

   Move::Move move(0,0,0,0,0,0);
   Move::Move move1(Bitboard::H1,Bitboard::G1,0,1,1,0);
   Move::Move move2(Bitboard::H1,Bitboard::G2,0,1,1,0);
   Move::Move move3(Bitboard::H1, Bitboard::G1,0,1,1,0);
   moveList.Push(move1);
   moveList.Push(move2);
   moveList.Push(move3);

   for (auto move = moveList.Begin(); move != moveList.End(); move++)
   {
        move->ShowMove();
   }

  /*std::string request;
   
   while (std::getline(std::cin, request)) {
       if (request == "isready")
       {
            std::cout<<"uciok\n";
            std::cout.flush();
       }
   }*/


    

    std::string fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
    std::string pos1fen = "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1";


    
    
    Timer timer;                                                      // search
    int depth = 7;
    Search search(depth, fen);
    timer.start();
    auto score = search.StartSearch();
    auto duration = timer.stop();
    std::cout<<"score: "<<score<<"\n";
    std::cout << "time: " << duration / 1000 << " ms.\n";
    Move::Move* pv = search.GetPV();
    for (int i = 0; i < depth; i++)
    {
        std::cout<<"pv["<<i<<"]: \n";
        pv[i].ShowMove();
    }
    
    //Timer timer;                                                       // perft
    //Perft perft(pos1fen,5);
    //timer.start();
    //auto perftResult = perft.Execute(false);
    //auto duration = timer.stop();
    //std::cout << "time: " << duration / 1000 << " ms.";
    //perftResult.Show(); 
  
    Cache::ClearCache();
}
