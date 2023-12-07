/**
 * @file transition_test.cpp
 * @author Gabe Kaufman (gkaufman93@gmail.com)
 * @brief Test state transitions
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include <iostream>
#include <deque>
#include <types.h>
#include <evtol_sim.h>
#include <global_clk.h>
#include <charger.h>
#include <flight_sim.h>

FlightSim* sim_inst;
#define HR_PER_TICK (0.05)
#define NUM_CHARGERS (1)

void test_setup(int num_vtols)
{
  // If instance is not null, delete explicitly
  if (sim_inst != nullptr) delete sim_inst;

  // Initialize new flight sim module
  sim_inst = new FlightSim(num_vtols, NUM_CHARGERS, HR_PER_TICK);
}

void test_single_vehicle()
{
  int num_vtols = 1;
  test_setup(num_vtols);

  sim_inst->display_company_makeup();

  sim_inst->sim_flight(3.0);

  sim_inst->aggregate_company_stats();
}


int main(int argc, char *argv[])
{
  test_single_vehicle();
  return 0;
}