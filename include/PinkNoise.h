#pragma once

#include "NoiseGenerator.h"
#include <random>
#include <array>

class PinkNoiseGenerator : public NoiseGenerator
{
private:
  using WhiteNoiseEngine = std::minstd_rand;
  static constexpr int NUM_SOURCES = 16;
  std::array<float, NUM_SOURCES> sources;
  WhiteNoiseEngine rng;
  int counter;

public:
  float gain;
  PinkNoiseGenerator();
  float next_float() override;
};
