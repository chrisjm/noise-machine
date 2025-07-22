#pragma once

class ResonantFilter
{
private:
  float a0, a1, a2, b1, b2;
  float x1, x2, y1, y2;

public:
  ResonantFilter();
  void set_params(float cutoff, float resonance, int sample_rate);
  float process(float input);
};
