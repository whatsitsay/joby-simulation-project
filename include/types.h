#ifndef __EVTOL_TYPES__
#define __EVTOL_TYPES__

#include <map>
#include <string>

// eVTOL aircraft companies
typedef enum _VTOL_Comp {
  ALPHA,
  BRAVO,
  CHARLIE,
  DELTA,
  ECHO,
  MAX_COMPANIES
} VTOL_Comp_e;

// eVTOL state enum
typedef enum _VTOL_State {
  IN_FLIGHT,            // Flying at cruising speed
  CHARGING,             // Charging battery
  WAITING_TO_CHARGE,    // Waiting for charger to become available
  MAX_STATES
} VTOL_State_e;

// Structure for VTOL sim statistics
typedef struct _VTOLStats {
  float total_charge_time_hr;     // Total time spent charging (TODO: doc says to track time "per session", but does that mean altogether, or include the waiting time?)
  int num_faults;                 // Total faults accrued while in flight
  float vehicle_fly_time_hr;      // Total hours flown
  float vehicle_fly_distance_mi;  // Total number of miles flown for the vehicle
} VTOLStats_t;

// Structure for VTOL sim parameters
typedef struct _VTOLParams {
  int cruise_speed_mph;
  int battery_capacity_kwh;
  float chg_time_hr;
  float energy_use_kwh_per_mi;
  float fault_prob_per_hr;
} VTOLParams_t;

// Company-specific parameters
// Alpha
const VTOLParams_t ALPHA_PARAMS = {
  120, // Cruise speed MPH
  320, // Battery capacity kWh
  0.6, // Charge time hours
  1.6, // Energy use kWh/mi
  0.25 // Fault prob per hr
};
#define ALPHA_NUM_PASSENGERS (4)

// Bravo
const VTOLParams_t BRAVO_PARAMS = {
  100, // Cruise speed MPH
  100, // Battery capacity kWh
  0.2, // Charge time hours
  1.5, // Energy use kWh/mi
  0.10 // Fault prob per hr
};
#define BRAVO_NUM_PASSENGERS (5)

// Charlie
const VTOLParams_t CHARLIE_PARAMS = {
  160, // Cruise speed MPH
  220, // Battery capacity kWh
  0.8, // Charge time hours
  2.2, // Energy use kWh/mi
  0.05 // Fault prob per hr
};
#define CHARLIE_NUM_PASSENGERS (3)

// Delta
const VTOLParams_t DELTA_PARAMS = {
  90, // Cruise speed MPH
  120, // Battery capacity kWh
  0.62, // Charge time hours
  0.8, // Energy use kWh/mi
  0.22 // Fault prob per hr
};
#define DELTA_NUM_PASSENGERS (2)

// Echo
const VTOLParams_t ECHO_PARAMS = {
  90, // Cruise speed MPH
  120, // Battery capacity kWh
  0.62, // Charge time hours
  0.8, // Energy use kWh/mi
  0.22 // Fault prob per hr
};
#define ECHO_NUM_PASSENGERS (2)

// Map of company enum->params
// Using std::move as params should only be called from map
// NOTE: could be array, but this is a bit more dynamic
const std::map<VTOL_Comp_e, VTOLParams_t> COMP_MAP{
  {ALPHA,   std::move(ALPHA_PARAMS)},
  {BRAVO,   std::move(BRAVO_PARAMS)},
  {CHARLIE, std::move(CHARLIE_PARAMS)},
  {DELTA,   std::move(DELTA_PARAMS)},
  {ECHO,    std::move(ECHO_PARAMS)},
};

// Map of enums to company name strings
const std::map<VTOL_Comp_e, std::string> COMP_NAMES{
  {ALPHA,   "Alpha"},
  {BRAVO,   "Bravo"},
  {CHARLIE, "Charlie"},
  {DELTA,   "Delta"},
  {ECHO,    "Echo"},
};

// Map of num passengers per vehicle type
const std::map<VTOL_Comp_e, int> VTOL_PASSENGERS{
  {ALPHA,   ALPHA_NUM_PASSENGERS},  
  {BRAVO,   BRAVO_NUM_PASSENGERS},
  {CHARLIE, CHARLIE_NUM_PASSENGERS}, 
  {DELTA,   DELTA_NUM_PASSENGERS}, 
  {ECHO,    ECHO_NUM_PASSENGERS}, 
};

#endif // __EVTOL_TYPES__