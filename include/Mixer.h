#pragma once

#include <vector>
#include <memory>
#include <mutex>

// Forward-declare NoiseGenerator to avoid circular includes
class NoiseGenerator;

class Mixer
{
private:
  std::vector<std::unique_ptr<NoiseGenerator>> sources;
  mutable std::mutex sources_mutex;

public:
  Mixer();
  void add_source(std::unique_ptr<NoiseGenerator> source);
  float process_sample();
  void clean_inactive_sources();
  void clear_sources();
  size_t get_source_count() const;
};
