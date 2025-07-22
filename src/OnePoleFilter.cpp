#include <OnePoleFilter.h>
#include <cmath>

OnePoleFilter::OnePoleFilter() : b1(0.0f), a0(1.0f), z1(0.0f) {}

void OnePoleFilter::set_cutoff(float frequency, int sample_rate)
{
  // A common formula for setting one-pole filter coefficients
  b1 = std::exp(-2.0f * M_PI * frequency / sample_rate);
  a0 = 1.0f - b1;
}

float OnePoleFilter::process(float input)
{
  // The core filter formula: y[n] = x[n]*a0 + y[n-1]*b1
  z1 = input * a0 + z1 * b1;
  return z1;
}
