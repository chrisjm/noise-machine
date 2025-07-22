#include "Raindrop.h"
#include <iostream>

RaindropGenerator::RaindropGenerator(int rate) : gain(0.5f)
{
  // std::cout << "  [+] Raindrop CREATED." << std::endl;
  rng.seed(std::random_device{}());
  envelope.set_sample_rate(rate);

  // --- Set the new filter parameters ---
  // A low cutoff frequency with high resonance gives a "plink" sound.
  float cutoff = 800.0f;   // The "tone" of the drop
  float resonance = 10.0f; // The "wetness" or "ring" of the drop
  filter.set_params(cutoff, resonance, rate);
}

RaindropGenerator::~RaindropGenerator()
{
  // std::cout << "  [-] Raindrop DESTROYED." << std::endl;
}

float RaindropGenerator::next_float()
{
  if (first_process)
  {
    // std::cout << "      [*] Raindrop PROCESSING SAMPLE." << std::endl;
    first_process = false;
  }
  float white_sample = (static_cast<float>(rng()) / rng.max()) * 2.0f - 1.0f;
  float filtered_sample = filter.process(white_sample);
  return filtered_sample * envelope.process() * gain;
}

void RaindropGenerator::trigger()
{
  // std::cout << "      [!] Raindrop TRIGGERED." << std::endl;
  envelope.trigger();
}

bool RaindropGenerator::is_active() const
{
  return envelope.is_active();
}
