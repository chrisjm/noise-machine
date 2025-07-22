#pragma once

#include <NoiseGenerator.h>
#include <Envelope.h>
#include <ResonantFilter.h>
#include <random>

class RaindropGenerator : public NoiseGenerator
{
private:
  using WhiteNoiseEngine = std::minstd_rand;
  WhiteNoiseEngine rng;
  EnvelopeGenerator envelope;
  ResonantFilter filter;
  bool first_process = true;

public:
  float gain;
  RaindropGenerator(int rate);
  ~RaindropGenerator();
  float next_float() override;
  void trigger();
  bool is_active() const;
};
