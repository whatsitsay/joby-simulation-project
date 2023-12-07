#ifndef __VTOL_SIM__
#define __VTOL_SIM__

#include <types.h>
#include <global_clk.h>
#include <random>

// Class prototype
class eVTOL_Sim {
  private:
    // Company-specific parameters
    VTOL_Comp_e  company;
    VTOLParams_t params;

    // Fault randomization
    std::default_random_engine rand_gen;
    std::bernoulli_distribution fault_dist;

    // Statistics
    VTOLStats_t stats;

    // FSM state
    VTOL_State_e curr_state;
    float flight_end_timestamp; // Flight is complete
    float charge_end_timestamp; // Charging is complete
    bool blocked; // VTOL currently blocked (waiting on charger)

    // Clock pointer
    std::shared_ptr<GlobalClk> clk;

    // Internal methods

    /**
     * Check if a fault has occurred in the last hour. Should only be called if tick count has hit an hour mark
     */
    void _check_fault();
  
  public:
    // Constructor
    eVTOL_Sim(VTOL_Comp_e company, std::shared_ptr<GlobalClk> clk);

    /**
     * @brief Start flight
     * 
     * Set state and set flight end timestamp
     * 
     * @param timestamp - Time (in hr) when flight begins
     */
    void start_flight(float timestamp);

    /**
     * @brief Enter charging state

     * @param timestamp - Time (in hr) when charging begins
     */
    void start_charge(float timestamp);

    /**
     * @brief Indicate if VTOL is blocked (waiting for a charger)
     * 
     * @return true - VTOL is in WAITING state 
     * @return false - VTOL is not in WAITING state
     */
    bool is_blocked();

    /**
     * @brief Set internal blocked flag
     * 
     * Separated from FSM to have check occur after FSM processing, decoupling
     * it from Charger queuing
     */
    void check_blocked();

    /**
     * @brief: Process tick
     * 
     * Uses the global clock's current timestamp to progress the state machine if needed
     */
    void tick();

    /**
     * @brief Get the eVTOL company for this instance
     * 
     * @return VTOL_Comp_e - eVTOL company enum
     */
    VTOL_Comp_e get_company();

    /**
     * @brief Get pointer to this eVTOL's running statistics block
     * 
     * @return VTOLStats_t* - Pointer to eVTOL sim statistics
     */
    VTOLStats_t* get_stats_ptr();
};

#endif // __VTOL_SIM__