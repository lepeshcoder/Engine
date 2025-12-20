//
// Created by nelep on 02.12.2025.
//

#ifndef ENGINE_IGENERATOR_H
#define ENGINE_IGENERATOR_H

#include <vector>

#include"../../../Position/Position.h"
#include"../../../Move/Move.h"

class IGenerator {
    public:

    virtual std::vector<Move::Move> GenerateAllMoves(const Position& pos);

};


#endif //ENGINE_IGENERATOR_H