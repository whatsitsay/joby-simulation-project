#include "global_clk.h"

GlobalClk::GlobalClk(float start_time, float hr_per_tick)
{
  // Set local variables and counters
  curr_timestamp = start_time;
  hr_per_tick    = hr_per_tick;
}

void GlobalClk::tick()
{
  // Increment timestamp by hr_per_tick
  curr_timestamp += hr_per_tick;
}

float GlobalClk::get_timestamp()
{
  return curr_timestamp;
}

float GlobalClk::get_hr_per_tick()
{
  return hr_per_tick;
}

void GlobalClk::set_timestamp(float timestamp)
{
  // Reset current timestamp and current tick to new value
  curr_timestamp = timestamp;
}