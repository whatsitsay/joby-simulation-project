#include <iostream>
#include <deque>
#include <types.h>
#include <evtol_sim.h>
#include <global_clk.h>
#include <charger.h>
#include "flight_sim.h"

using namespace std;

FlightSim::FlightSim(int num_vtols, int num_chargers, float tick_rate)
{
  // Instantiate clock
  global_clk = make_shared<GlobalClk>(0, tick_rate);
  // Instantiate charger
  charger = make_shared<Charger>(num_chargers);

  // Iterate over and instantiate eVTOL sims
  for (int i = 0; i < num_vtols; i++) {
    // Randomize company. Just use rand() as this is a very simple randomization
    VTOL_Comp_e company = (VTOL_Comp_e)(rand() % MAX_COMPANIES);

    // Instantiate sim instance and push into main queue
    evtol_arr.emplace_back(company, global_clk, charger);

    // Push into relevant queue for stats aggregation
    evtol_companies[company].push_back(&evtol_arr.back());
  }
}

void FlightSim::display_company_makeup()
{
  cout << "For " << evtol_arr.size() << " eVTOLs instantiated, the breakdown is as follows:" << endl;
  cout << "\tAlpha:   " << evtol_companies[ALPHA].size() << endl;
  cout << "\tBravo:   " << evtol_companies[BRAVO].size() << endl;
  cout << "\tCharlie: " << evtol_companies[CHARLIE].size() << endl;
  cout << "\tDelta:   " << evtol_companies[DELTA].size() << endl;
  cout << "\tEcho:    " << evtol_companies[ECHO].size() << endl;
}

void FlightSim::sim_flight(float sim_time_hr)
{
  cout << "Beginning Flight Sim, simulated duration: " << sim_time_hr << " hours" << endl;
  float start_timestamp = global_clk->get_timestamp();
  end_timestamp = start_timestamp + sim_time_hr;

  int pct_complete = 0;

  while(global_clk->get_timestamp() < end_timestamp)
  {
    // Start by ticking clock
    global_clk->tick();

    // Next iterate through all VTOLs
    vector<eVTOL_Sim>::iterator vtol;
    for (vtol = evtol_arr.begin(); vtol != evtol_arr.end(); ++vtol) {
        // If particular VTOL is blocked, skip
        if (vtol->is_blocked()) continue;

        // Activate tick
        vtol->tick();
    }

    // TODO: add means to track per-tick stats here

    // Iterate again, checking 
    // This is done separately to avoid ticking instances twice
    // but still update based on charger availability
    for (vtol = evtol_arr.begin(); vtol != evtol_arr.end(); ++vtol) {
      vtol->check_blocked();
    }

    // Calc percentage complete via timestamp
    float time_complete_hr = global_clk->get_timestamp() - start_timestamp;
    pct_complete = (int)roundf(100.0 * (time_complete_hr / sim_time_hr));
    cout << pct_complete << "%% complete @ " << global_clk->get_timestamp()
         << ", end time " << end_timestamp << endl;
  }

  cout << "Completed " << sim_time_hr << " hour simulation!" << endl;
}

void FlightSim::aggregate_company_stats() {
  cout << "Company Statistics:\n" << endl;
  for (int company = ALPHA; company != MAX_COMPANIES; company++)
  {
    VTOL_Comp_e comp_enum = static_cast<VTOL_Comp_e>(company);
    // If queue for company is empty, indicate as such and skip
    if (evtol_companies[company].empty())
    {
      cout << "No eVTOLs instantiated for " << COMP_NAMES.at(comp_enum) << " company.\n" << endl;
      continue;
    }

    // Otherwise, proceed with stats aggregation
    // Instantiate running statistics for company
    float avg_flight_time       = 0;
    float avg_flight_distance   = 0;
    float avg_charging_time     = 0;
    int total_faults            = 0;
    float total_passenger_miles = 0;

    // For averages
    float num_vtols = evtol_companies[company].size();

    // Iterate over all eVTOLs of company make
    for (eVTOL_Sim* vtol_p : evtol_companies[company])
    {
      VTOLStats_t* stats_p = vtol_p->get_stats_ptr();
      // Averages
      avg_flight_time += stats_p->vehicle_fly_time_hr / num_vtols;
      avg_flight_distance += stats_p->vehicle_fly_distance_mi / num_vtols;
      avg_charging_time += stats_p->total_charge_time_hr / num_vtols;
      // Totals
      total_faults += stats_p->num_faults;
      total_passenger_miles += stats_p->vehicle_fly_distance_mi * VTOL_PASSENGERS.at(comp_enum);
    }

    cout << COMP_NAMES.at(comp_enum) << " Statistics:" << endl;
    cout << "\tAvg. Flight Time:      " << avg_flight_time << " hours" << endl;
    cout << "\tAvg. Flight Distance:  " << avg_flight_distance << " miles" << endl;
    cout << "\tAvg. Charging Time:    " << avg_charging_time << " hours" << endl;
    cout << "\tTotal Faults:          " << total_faults << endl;
    cout << "\tTotal Passenger Miles: " << total_passenger_miles;
    cout << endl; // Extra line break between company stats blocks
  }
}

void FlightSim::force_time(float timestamp)
{
  global_clk->set_timestamp(timestamp);
}

