#include "charger.h"
#include <evtol_sim.h>
#include <deque>

Charger::Charger(int num_chargers)
{
  // Instantiate number of chargers
  chargers_available = num_chargers;
}

static bool Charger::try_get_charger(eVTOL_Sim* vtol_ptr)
{
  // Act based on number of chargers available
  if (chargers_available > 0)
  {
    // Decrement count and return true
    --chargers_available;
    return true;
  }

  // Else, store in queue and return false
  wait_q.push_back(vtol_ptr);
  return false;
}

static void Charger::release_charger(float timestamp)
{
  if (wait_q.empty())
  {
    // Increment number of chargers available
    ++chargers_available;
  }
  else
  {
    // Pop a waiting VTOL from the wait queue
    eVTOL_Sim* vtol = wait_q.pop_front();
    // Unblock VTOL by calling it's "start_charge" method
    vtol->start_charge(timestamp);
  }
}