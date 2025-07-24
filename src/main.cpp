#define MINIAUDIO_IMPLEMENTATION
#include <miniaudio.h>
#include <MPMCQueue.h>

#include <iostream>
#include <random>
#include <array>
#include <atomic>
#include <unistd.h>
#include <termios.h>
#include <thread>

#include <Commands.h>
#include <NoiseGenerator.h>
#include <WhiteNoise.h>
#include <PinkNoise.h>
#include <BrownNoise.h>
#include <Raindrop.h>
#include <Mixer.h>

struct SharedState
{
  rigtorp::MPMCQueue<Command> command_queue{128};
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

void input_thread_func(rigtorp::MPMCQueue<Command> &queue)
{
  while (true)
  {
    char c = getchar();
    if (c == 'w')
      queue.push({AppState::WHITE});
    else if (c == 'p')
      queue.push({AppState::PINK});
    else if (c == 'b')
      queue.push({AppState::BROWN});
    else if (c == 'r')
      queue.push({AppState::RAIN});
    else if (c == 'q')
    {
      queue.push({AppState::QUIT});
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
  AppState current_mode = AppState::PINK;
  std::atomic<char> last_key_pressed(0);

  std::minstd_rand timing_rng(std::random_device{}());
  // Avg. 10 loops between drops, with a standard deviation of 4 loops
  std::normal_distribution<float> distribution(3.0f, 4.0f);

  std::thread input_thread(input_thread_func, std::ref(state.command_queue));

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

  int loops_until_next_drop = distribution(timing_rng);

  Command cmd;
  while (current_mode != AppState::QUIT)
  {
    if (state.command_queue.try_pop(cmd))
    {
      if (cmd.state != current_mode)
      {
        current_mode = cmd.state;
        state.mixer.clear_sources();

        switch (current_mode)
        {
        case AppState::WHITE:
          state.mixer.add_source(std::make_unique<WhiteNoiseGenerator>());
          std::cout << "Switched to: White Noise" << std::endl;
          break;
        case AppState::PINK:
          state.mixer.add_source(std::make_unique<PinkNoiseGenerator>());
          std::cout << "Switched to: Pink Noise" << std::endl;
          break;
        case AppState::BROWN:
          state.mixer.add_source(std::make_unique<BrownNoiseGenerator>());
          std::cout << "Switched to: Brown Noise" << std::endl;
          break;
        case AppState::RAIN:
          state.mixer.add_source(std::make_unique<PinkNoiseGenerator>());
          std::cout << "Switched to: Rain Storm" << std::endl;
          break;
        case AppState::QUIT:
          break;
        }
      }
    }

    if (current_mode == AppState::RAIN)
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
