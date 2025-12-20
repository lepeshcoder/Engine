//
// Created by nelep on 03.12.2025.
//

#ifndef ENGINE_LEGALGENERATOR_H
#define ENGINE_LEGALGENERATOR_H
#include "../IGenerator/IGenerator.h"
#include "../../Cache/Cache.h"


class LegalGenerator : IGenerator {

private:

public:

    std::vector<Move::Move> GenerateAllMoves(const Position &pos) override;

};


#endif //ENGINE_LEGALGENERATOR_H