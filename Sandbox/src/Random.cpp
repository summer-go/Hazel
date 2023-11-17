//
// Created by ByteDance on 2023/11/16.
//

#include "Random.h"

std::mt19937 Random::s_RandomEngine;
std::uniform_int_distribution<std::mt19937::result_type> Random::s_Distribution;
