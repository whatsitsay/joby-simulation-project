#ifndef _GLOBAL_CLK_
#define _GLOBAL_CLK_

// Simple class for synchronizing all sim instances
class GlobalClk {
  private:
    float curr_timestamp;           // Current timestamp
    float hr_per_tick;              // Hour incremented per tick
  
  public:
    // Constructor
    GlobalClk(float start_time, float hr_per_tick);

    /**
     * @brief Increment timestamp by tick amount, defined in constructor
     */
    void tick();

    /**
     * @brief Get the current timestamp
     * 
     * @return float - timestamp value (in hours)
     */
    float get_timestamp();

    /**
     * @brief Get the number of hours incremented per tick
     * 
     * @return float - Hours passed per call to tick()
     */
    float get_hr_per_tick();

    /**
     * @brief Force the timestamp to the given value.
     * 
     * Should only really be used in testing
     * 
     * @param timestamp - Desired timestamp value
     */
    void set_timestamp(float timestamp);
};

#endif // _GLOBAL_CLK_