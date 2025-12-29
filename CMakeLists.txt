cmake_minimum_required(VERSION 4.0)
project(Engine)

set(CMAKE_CXX_STANDARD 20)

add_executable(Engine main.cpp
        Bitboard/Bitboard.h
        Bitboard/Bitboard.cpp
        MoveGeneration/NonSlidingPieces/NonSlidingPieces.h
        MoveGeneration/NonSlidingPieces/NonSlidingPieces.cpp
        Position/Position.cpp
        Position/Position.h
        Types.h
        Rays/Rays.cpp
        Rays/Rays.h
        MoveGeneration/SlidingPieces/SlidingPieces.h
        MoveGeneration/SlidingPieces/SlidingPieces.cpp
        MoveGeneration/Cache/Cache.cpp
        MoveGeneration/Cache/Cache.h
        Utils/Timer/Timer.cpp
        Utils/Timer/Timer.h
        Move/Move.cpp
        Move/Move.h
        MoveGeneration/Generators/LegalGenerator/LegalGenerator.cpp
        MoveGeneration/Generators/LegalGenerator/LegalGenerator.h
        Utils/Perft/Perft.cpp
        Utils/Perft/Perft.h
)

target_compile_options(Engine PRIVATE -mbmi2)
target_link_options(Engine PRIVATE -mbmi2)
