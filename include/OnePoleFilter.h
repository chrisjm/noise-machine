#pragma once

class OnePoleFilter
{
private:
    float b1;
    float a0;
    float z1;

public:
    OnePoleFilter();
    void set_cutoff(float frequency, int sample_rate);
    float process(float input);
};
