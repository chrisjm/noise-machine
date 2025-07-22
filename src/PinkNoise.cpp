#include "PinkNoise.h"

PinkNoiseGenerator::PinkNoiseGenerator() : counter(0), gain(0.2f)
{
  sources.fill(0.0f);
  rng.seed(std::random_device{}());
}

float PinkNoiseGenerator::next_float()
{
  // This is a compiler intrinsic, so it doesn't need a special header with clang/gcc
  int trailing_zeros = __builtin_ctz(++counter);
  sources[trailing_zeros % NUM_SOURCES] = (static_cast<float>(rng()) / rng.max()) * 2.0f - 1.0f;

  float sum = 0.0f;
  for (float s : sources)
  {
    sum += s;
  }

  return (sum / static_cast<float>(NUM_SOURCES)) * gain;
}
