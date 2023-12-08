#include <iostream>
#include <flight_sim.h>

// Simulation parameters
#define NUM_VTOLS              (20)
#define NUM_CHARGERS           (3)
#define FLIGHT_SIM_TIME_HR     (3.0)
#define FLIGHT_SIM_HR_PER_TICK (0.05)

using namespace std;

int main(int argc, char *argv[])
{
  cout << "Running eVTOL simulation with the following parameters: " << endl;
  cout << "\tNum eVTOLs:              " << NUM_VTOLS << endl;
  cout << "\tNum chargers:            " << NUM_CHARGERS << endl;
  cout << "\tSimulation time (hours): " << FLIGHT_SIM_TIME_HR << endl;

  // Instantiate flight sim class
  FlightSim sim_inst = FlightSim(NUM_VTOLS, NUM_CHARGERS, FLIGHT_SIM_HR_PER_TICK);
  sim_inst.display_company_makeup();

  // Simulate flight
  sim_inst.sim_flight(FLIGHT_SIM_TIME_HR);

  // Print stats
  sim_inst.aggregate_company_stats();

  return 0;
}
