#define MINIAUDIO_IMPLEMENTATION
#include <miniaudio.h>

#include <iostream>
#include <random>
#include <array>
#include <atomic>
#include <unistd.h>
#include <termios.h>
#include <thread>

#include <NoiseGenerator.h>
#include <WhiteNoise.h>
#include <PinkNoise.h>
#include <BrownNoise.h>
#include <Raindrop.h>
#include <Mixer.h>

enum class PlayMode
{
  WHITE,
  PINK,
  BROWN,
  RAIN,
  QUIT
};

struct SharedState
{
  Mixer mixer;
};

void data_callback(ma_device *pDevice, void *pOutput, const void *pInput, ma_uint32 frameCount)
{
  (void)pInput;
  SharedState *pState = (SharedState *)pDevice->pUserData;
  float *pOutputF32 = (float *)pOutput;

  // The callback's only job is to ask the mixer for the next sample.
  for (ma_uint32 i = 0; i < frameCount; ++i)
  {
    float sample = pState->mixer.process_sample();
    pOutputF32[i * 2 + 0] = sample; // Left
    pOutputF32[i * 2 + 1] = sample; // Right
  }
}

void set_unbuffered_input()
{
  termios oldt, newt;
  tcgetattr(STDIN_FILENO, &oldt);
  newt = oldt;
  newt.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, &newt);
}

// This function runs on a separate thread and just waits for keyboard input.
void input_thread_func(std::atomic<char> &last_key)
{
  while (true)
  {
    char c = getchar();
    last_key.store(c);
    if (c == 'q')
    {
      break;
    }
  }
}

int main()
{
  ma_device_config deviceConfig;
  ma_device device;
  SharedState state;

  int channels = 2;
  int sample_rate = 48000;
  PlayMode current_mode = PlayMode::PINK;
  std::atomic<char> last_key_pressed(0);

  std::minstd_rand timing_rng(std::random_device{}());
  // Avg. 10 loops between drops, with a standard deviation of 4 loops
  std::normal_distribution<float> distribution(3.0f, 4.0f);
  int loops_until_next_drop = distribution(timing_rng);

  std::thread input_thread(input_thread_func, std::ref(last_key_pressed));

  // std::make_unique is the modern C++ way to create a unique_ptr.
  state.mixer.add_source(std::make_unique<PinkNoiseGenerator>());

  deviceConfig = ma_device_config_init(ma_device_type_playback);
  deviceConfig.playback.format = ma_format_f32;
  deviceConfig.playback.channels = channels;
  deviceConfig.sampleRate = sample_rate;
  deviceConfig.dataCallback = data_callback;
  deviceConfig.pUserData = &state;

  if (ma_device_init(NULL, &deviceConfig, &device) != MA_SUCCESS)
  {
    std::cerr << "Failed to initialize playback device." << std::endl;
    return -1;
  }

  if (ma_device_start(&device) != MA_SUCCESS)
  {
    std::cerr << "Failed to start playback device." << std::endl;
    return -1;
  }

  std::cout << "Noise Generator Started!" << std::endl;
  std::cout << "'w' white | 'p' pink | 'b' brown | 'r' rain | 'q' quit" << std::endl;
  std::cout << "Currently playing: Pink Noise" << std::endl;

  set_unbuffered_input();

  while (current_mode != PlayMode::QUIT)
  {
    char key = last_key_pressed.exchange(0); // Get the last key and reset it

    if (key)
    {
      if (key == 'w' && current_mode != PlayMode::WHITE)
      {
        current_mode = PlayMode::WHITE;
        state.mixer.clear_sources();
        state.mixer.add_source(std::make_unique<WhiteNoiseGenerator>());
        std::cout << "Switched to: White Noise" << std::endl;
      }
      else if (key == 'p' && current_mode != PlayMode::PINK)
      {
        current_mode = PlayMode::PINK;
        state.mixer.clear_sources();
        state.mixer.add_source(std::make_unique<PinkNoiseGenerator>());
        std::cout << "Switched to: Pink Noise" << std::endl;
      }
      else if (key == 'b' && current_mode != PlayMode::BROWN)
      {
        current_mode = PlayMode::BROWN;
        state.mixer.clear_sources();
        state.mixer.add_source(std::make_unique<BrownNoiseGenerator>());
        std::cout << "Switched to: Brown Noise" << std::endl;
      }
      else if (key == 'r' && current_mode != PlayMode::RAIN)
      {
        current_mode = PlayMode::RAIN;
        state.mixer.clear_sources();
        state.mixer.add_source(std::make_unique<PinkNoiseGenerator>());
        std::cout << "Switched to: Rain Storm" << std::endl;
      }
      else if (key == 'q')
      {
        current_mode = PlayMode::QUIT;
      }
    }

    if (current_mode == PlayMode::RAIN)
    {
      --loops_until_next_drop;

      if (loops_until_next_drop <= 0)
      {
        auto raindrop = std::make_unique<RaindropGenerator>(sample_rate);
        raindrop->gain = 0.25f;
        raindrop->trigger();
        state.mixer.add_source(std::move(raindrop));
        loops_until_next_drop = std::max(1, (int)distribution(timing_rng));
      }
      state.mixer.clean_inactive_sources();
    }
    usleep(10000);
  }

  std::cout << std::endl
            << "Quitting..." << std::endl;
  ma_device_uninit(&device);
  input_thread.join(); // Wait for the input thread to finish
  return 0;
}
