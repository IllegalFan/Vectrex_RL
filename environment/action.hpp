#pragma once

#include <cstdint>

extern "C" 
{
    #include "periphery.h"
}

namespace vecx_rl {
/**
 * Simulating peripheral input to the vectrex
 */
class action {
public:
  action();
  ~action();

  /**
   * Set an input
   * @param peripheral: Choose peripherals to interact by the conjunction of the
   * peripherals
   * @param push: If true the peripheral gets pushed
   */
  void set_action(const uint8_t peripheral);

  inline uint8_t get_action() const { return event; }

public:
  uint8_t event;
};
} // namespace vecx_rl
