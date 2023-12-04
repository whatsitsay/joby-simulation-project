#include <iostream>
#include <random>
#include <types.h>
#include <global_clk.h>
#include <charger.h>

#include "evtol_sim.h"

eVTOL_Sim::eVTOL_Sim(VTOL_Comp_e company, std::shared_ptr<GlobalClk> clk) {
  // Copy in parameters using company enum
  this->company = company;
  memcpy(&this->params, &(comp_map[company]), sizeof(VTOLParams_t));
  // Assign local clock pointer
  this->clk = clk;

  // Initialize statistics to all zeros
  stats = {0, 0, 0};

  // Initialize fault bernoulli distribution to fault per hour x hour per tick
  // Assumes uniform probability distribution per hour and hr_per_tick <= 1
  // TODO: is this a correct assumption? Could also be a Poisson distribution
  fault_dist = std::bernoulli_distribution(this->params.fault_prob_per_hr * this->clk->get_hr_per_tick());

  // Start a flight
  start_flight(this->clk->get_timestamp());
}

void eVTOL_Sim::start_flight(float timestamp) {
  // Flight time will be hours to battery depletion, or
  // battery capacity / cruise power draw 
  // Where cruise power draw is energy draw per mile x cruise speed (mph)
  float cruise_power_draw_kw = params.energy_use_kwh_per_mi * params.cruise_speed_mph;
  float flight_time_hr = params.battery_capacity_kwh / cruise_power_draw_kw;
  // Set flight time end as timestamp + flight time hr
  flight_end_timestamp = timestamp + flight_time_hr;

  // Change state
  curr_state = IN_FLIGHT;
}

void eVTOL_Sim::start_charge(float timestamp) {
  // Set charge end time to timestamp + charge time
  charge_end_timestamp = timestamp + params.chg_time_hr;
  // Set state to CHARGING
  curr_state = CHARGING;
}

void eVTOL_Sim::_check_fault() {
  // Use bernoulli distribution to increment counter
  if (fault_dist(rand_gen)) ++stats.num_faults;
}

bool eVTOL_Sim::is_blocked() {
  // Return whether VTOL is waiting or not
  return (curr_state == WAITING_TO_CHARGE);
}

void eVTOL_Sim::tick() {
  // Start by getting the current timestamp and hr_per_tick
  float curr_timestamp = clk->get_timestamp();
  float hr_per_tick    = clk->get_hr_per_tick();

  // Enter state machine
  switch (curr_state) {
    IN_FLIGHT: {
      // Get hour per tick
      // Increment miles flown by tick time
      stats.vehicle_fly_distance_mi += params.cruise_speed_mph * hr_per_tick;

      // Check for fault
      // TODO: Doesn't account for flight end scenario, which will likely be
      // within a tick instead of a tick boundary
      _check_fault();

      // If haven't reached flight end, move on
      if (curr_timestamp < flight_end_timestamp) break;
      
      // Otherwise, reached flight end!
      // Subtract difference between current timestamp and flight end
      // NOTE: this could just be done at the end with the total time, but allows
      // for mid-sim checking of distance flown (if desired)
      float timestamp_diff           = curr_timestamp - flight_end_timestamp;
      stats.vehicle_fly_distance_mi -= timestamp_diff * hr_per_tick;

      // Try to get charger key, passing pointer to this instance
      if (Charger::try_get_charger(this)) {
        // Successfully got key! Start charging using flight end timestamp as start
        start_charge(flight_end_timestamp);
      } else {
        // Chargers are accounted for, change to "waiting"
        curr_state = WAITING_TO_CHARGE;
      }

      break;
    }
    CHARGING: {
      // Increment total charging time
      stats.total_charge_time_hr += hr_per_tick;

      // Check timestamp. If not at endpoint, move on
      if (curr_timestamp < charge_end_timestamp) break;

      // Else, charging complete
      // Subtract difference between current timestamp and charge end
      // NOTE: this could just be done at the end with the total time, but allows
      // for mid-sim checking of charge time (if desired)
      float timestamp_diff = curr_timestamp - charge_end_timestamp;
      stats.total_charge_time_hr -= timestamp_diff;

      // Go directly into flight, using charge end as timestamp
      start_flight(charge_end_timestamp);

      // Release charger, passing the charge end timestamp
      Charger::release_charger(charge_end_timestamp);

      break;
    }
    WAITING_TO_CHARGE: {
      // Should not reach this point, throw error
      // Instead, should be unblocked by calling "start_charge" from within Charger instance
      throw std::runtime_error("Should not process tick while WAITING!");
    }
  }
}