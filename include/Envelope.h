#pragma once

class EnvelopeGenerator
{
private:
  enum State
  {
    IDLE,
    ATTACK,
    DECAY
  };

  State current_state;
  float current_level;
  float attack_increment;
  float decay_factor;
  int sample_rate;

public:
  EnvelopeGenerator();
  void set_sample_rate(int rate);
  void trigger();
  float process();
  bool is_active() const;
};
