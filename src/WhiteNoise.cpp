#include "WhiteNoise.h"

WhiteNoiseGenerator::WhiteNoiseGenerator() : gain(0.1f)
{
  rng.seed(std::random_device{}());
}

float WhiteNoiseGenerator::next_float()
{
  constexpr float min_val = static_cast<float>(WhiteNoiseEngine::min());
  constexpr float max_val = static_cast<float>(WhiteNoiseEngine::max());
  unsigned int raw_rand = rng();
  float normalized = (static_cast<float>(raw_rand) - min_val) / (max_val - min_val);
  return (normalized * 2.0f - 1.0f) * gain;
}
