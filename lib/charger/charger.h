/**
 * @brief Charger class, modeled after a counting semaphore
 * 
 * Allows for different VTOL to attempt to request and release chargers.
 * 
 */

#ifndef __CHARGER__
#define __CHARGER__

#include <deque>
#include <evtol_sim.h>

class Charger {
  private:
    static int chargers_available; // Counter for number of chargers available
    static std::deque<eVTOL_Sim *> wait_q; // Wait queue for VTOLs

  public:
    /**
     * @brief Construct a new Charger object
     * 
     * @param num_chargers - Number of chargers available
     */
    Charger(int num_chargers);
    
    /**
     * @brief Attempt to get a charger
     * 
     * @param vtol_ptr - Pointer to requesting VTOL instance
     * @return true - If charger is available
     * @return false - If charger is not available. Also added to internal wait queue
     */
    static bool try_get_charger(eVTOL_Sim* vtol_ptr);

    /**
     * @brief Release a charger
     * 
     * If a VTOL is waiting for a charger, it will be notified and unblocked
     * 
     * @param timestamp - Timestamp of charger release, passed from releasing VTOL
     */
    static void release_charger(float timestamp);
}

#endif // __CHARGER__