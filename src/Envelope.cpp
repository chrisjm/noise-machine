#include "Envelope.h"
#include <cmath>

EnvelopeGenerator::EnvelopeGenerator() : current_state(IDLE),
                                         current_level(0.0f),
                                         attack_increment(0.0f),
                                         decay_factor(0.0f),
                                         sample_rate(48000)
{
}

void EnvelopeGenerator::set_sample_rate(int rate)
{
  sample_rate = rate;

  // Let's make the attack 10 times slower (10ms instead of 1ms)
  attack_increment = 1.0f / (0.01f * sample_rate);
  // The decay time is probably fine, but we can make it a little longer too
  decay_factor = std::exp(-1.0f / (0.07f * sample_rate));

  // // Attack time: 1ms
  // attack_increment = 1.0f / (0.001f * sample_rate);
  // // Decay time: approx 50ms
  // decay_factor = std::exp(-1.0f / (0.05f * sample_rate));
}

void EnvelopeGenerator::trigger()
{
  current_state = ATTACK;
  current_level = 0.0f;
}

float EnvelopeGenerator::process()
{
  switch (current_state)
  {
  case ATTACK:
    current_level += attack_increment;
    if (current_level >= 1.0f)
    {
      current_level = 1.0f;
      current_state = DECAY;
    }
    break;
  case DECAY:
    current_level *= decay_factor;
    if (current_level < 0.00001f)
    { // Close enough to zero
      current_level = 0.0f;
      current_state = IDLE;
    }
    break;
  case IDLE:
    current_level = 0.0f;
    break;
  }
  return current_level;
}

bool EnvelopeGenerator::is_active() const
{
  return current_state != IDLE;
}
