#include "BrownNoise.h"

BrownNoiseGenerator::BrownNoiseGenerator() : previous_output(0.0f),
                                             leakage_factor(0.998f),
                                             gain(0.02f)
{
  rng.seed(std::random_device{}());
}

float BrownNoiseGenerator::next_float()
{
  constexpr float min_val = static_cast<float>(WhiteNoiseEngine::min());
  constexpr float max_val = static_cast<float>(WhiteNoiseEngine::max());
  float white_sample = ((static_cast<float>(rng()) - min_val) / (max_val - min_val) * 2.0f - 1.0f) * gain;

  float current_output = (previous_output * leakage_factor) + white_sample;
  previous_output = current_output;

  return current_output;
}
