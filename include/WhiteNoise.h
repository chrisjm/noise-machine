#pragma once

#include "NoiseGenerator.h"
#include <random>

class WhiteNoiseGenerator : public NoiseGenerator
{
private:
  using WhiteNoiseEngine = std::minstd_rand;
  WhiteNoiseEngine rng;

public:
  float gain;
  WhiteNoiseGenerator();
  float next_float() override;
};
