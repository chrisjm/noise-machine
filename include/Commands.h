#pragma once

enum class AppState
{
  WHITE,
  PINK,
  BROWN,
  RAIN,
  QUIT
};

struct Command
{
  AppState state;
};
