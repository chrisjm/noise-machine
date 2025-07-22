#include "Mixer.h"
#include "NoiseGenerator.h"
#include "Raindrop.h"
#include <algorithm>

Mixer::Mixer() {}

void Mixer::add_source(std::unique_ptr<NoiseGenerator> source)
{
  std::lock_guard<std::mutex> lock(sources_mutex);
  // std::move transfers ownership of the source into our vector
  sources.push_back(std::move(source));
}

float Mixer::process_sample()
{
  std::lock_guard<std::mutex> lock(sources_mutex);
  float mixed_sample = 0.0f;

  for (const auto &source : sources)
  {
    mixed_sample += source->next_float();
  }
  return mixed_sample;
}

void Mixer::clean_inactive_sources()
{
  std::lock_guard<std::mutex> lock(sources_mutex);
  // erase-remove idiom
  sources.erase(
      std::remove_if(sources.begin(), sources.end(), [](const auto &source)
                     {
            // Try to cast the source to a RaindropGenerator
            if (auto* raindrop = dynamic_cast<RaindropGenerator*>(source.get())) {
                // If it's a raindrop and it's no longer active, mark it for removal
                return !raindrop->is_active();
            }
            // Don't remove non-raindrop sources (like our background noise)
            return false; }),
      sources.end());
}

void Mixer::clear_sources()
{
  std::lock_guard<std::mutex> lock(sources_mutex);
  sources.clear();
}

size_t Mixer::get_source_count() const
{
  std::lock_guard<std::mutex> lock(sources_mutex);
  return sources.size();
}
