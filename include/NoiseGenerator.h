#pragma once

class NoiseGenerator
{
public:
  virtual ~NoiseGenerator() = default;
  virtual float next_float() = 0;
};
