//
// Created by nelep on 20.12.2025.
//

#ifndef ENGINE_PERFT_H
#define ENGINE_PERFT_H
#include "../../MoveGeneration/Generators/LegalGenerator/LegalGenerator.h"
#include "../../Position/Position.h"


class Perft {

private:
    LegalGenerator gen;
    Position* pos;
    int depth = 0;
    int positions = 0;

    void myaso(int depth);
public:
    Perft(int depth);

    ~Perft();

    int Execute();
};


#endif //ENGINE_PERFT_H