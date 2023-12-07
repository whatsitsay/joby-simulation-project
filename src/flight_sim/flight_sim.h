/**
 * Flight simulator and statistics aggregator
 */

#ifndef _FLIGHT_SIM_
#define _FLIGHT_SIM_

#include <iostream>
#include <deque>
#include <memory>
#include <types.h>
#include <evtol_sim.h>
#include <global_clk.h>
#include <charger.h>

using namespace std;

class FlightSim {

  private:
    float end_timestamp;

    // Main queue of VTOLs
    deque<eVTOL_Sim> evtol_q;

    // Array of queues for stats aggregation 
    deque<eVTOL_Sim *> evtol_companies[MAX_COMPANIES];

    // Charger instance
    shared_ptr<Charger> charger;

    // Global clock instance
    shared_ptr<GlobalClk> global_clk;
  
  public:
    /**
     * @brief Construct a new Flight Sim object
     * 
     * @param num_vtols - Number of eVTOLs to simulate
     * @param num_chargers - Number of chargers to simulate
     * @param tick_rate - Hours passed per tick
     */
    FlightSim(int num_vtols, int num_chargers, float tick_rate);

    /**
     * @brief Print the company makeup for all eVTOLs
     * 
     * I.e., print the number of eVTOLs for company Alpha, Braco, etc.
     * 
     */
    void display_company_makeup();

    /**
     * @brief Simulate flight for all VTOLs for the given amount of time
     * 
     * @param sim_time_hr - Simulation time in hours
     */
    void sim_flight(float sim_time_hr);

    /**
     * @brief Aggregate statistics per eVTOL company
     * 
     * Will output the following per company, as per the spec:
     * - Average time in flight (in hours)
     * - Average distance flown (in miles)
     * - Average charging time (in hours)
     * - Total faults across all instances
     * - Total passenger miles
     * 
     * NOTE: this assumes that the average flight time/distance and charging
     * time are for the *overall* simulation time, as these statistics are 
     * otherwise deterministic
     */
    void aggregate_company_stats();

    /**
     * @brief Force the global clock to the given timestamp
     * 
     * Should only be used in unit testing
     * 
     * @param timestamp - Desired new timestamp
     */
    void force_time(float timestamp);
};

#endif // _FLIGHT_SIM_