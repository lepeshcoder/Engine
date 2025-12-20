//
// Created by nelep on 20.12.2025.
//

#include "Perft.h"

void Perft::myaso(int depth) {
    if (depth == 0) {
        ++positions;
        return;
    }
    auto moves = gen.GenerateAllMoves(*pos);
    for (auto &move : moves) {
        pos->MakeMove(move);
        myaso(depth - 1);
        pos->UnMakeMove(move);
    }
}

Perft::Perft(int depth) {
    this->depth = depth;
    pos = new Position("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
}

Perft::~Perft() {
    delete pos;
}

int Perft::Execute() {
    myaso(depth);
    return positions;
}
