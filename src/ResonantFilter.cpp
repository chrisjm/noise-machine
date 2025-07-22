#include <ResonantFilter.h>
#include <algorithm>

ResonantFilter::ResonantFilter()
{
  a0 = 1.0;
  a1 = a2 = b1 = b2 = 0.0;
  x1 = x2 = y1 = y2 = 0.0;
}

void ResonantFilter::set_params(float cutoff, float resonance, int sample_rate)
{
  resonance = std::max(0.707f, resonance);
  float omega = 2.0f * M_PI * cutoff / sample_rate;
  float alpha = std::sin(omega) / (2.0f * resonance);
  float cos_omega = std::cos(omega);

  b1 = -2.0f * cos_omega;
  b2 = 1.0f - alpha;
  a0 = (1.0f - cos_omega) / 2.0f;
  a1 = 1.0f - cos_omega;
  a2 = a0;
}

float ResonantFilter::process(float input)
{
  float output = input * a0 + x1;
  x1 = input * a1 - output * b1 + x2;
  x2 = input * a2 - output * b2;
  return output;
}
