#pragma once

#include "NoiseGenerator.h"
#include <random>

class BrownNoiseGenerator : public NoiseGenerator
{
private:
  using WhiteNoiseEngine = std::minstd_rand;
  WhiteNoiseEngine rng;
  float previous_output;

public:
  float leakage_factor;
  float gain;
  BrownNoiseGenerator();
  float next_float() override;
};
