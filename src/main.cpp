#include "stamp.h"
#include "merlin.h"
#include "rainbow.h"
#include "color_rings.h"
#include "digits.h"
#include "coil.h"
#include "codes.h"
#include "hints.h"
#include "mc.h"

#include "hw_devices.h"

void setup()
{
  common_routine_init();

  stamp_init();
  merlin_init();
  rainbow_init();
  color_rings_init();
  digits_init();
  coil_init();
  codes_init();
  hints_init();
  mc_init();

  devices_init();

  *console << CIRCUIT_NAME << F(": init done") << endl;
}

void loop()
{
  common_routine();

  stamp_routine();
  merlin_routine();
  rainbow_routine();
  color_rings_routine();
  digits_routine();
  coil_routine();
  codes_routine();
  hints_routine();
  mc_routine();

  devices_routine();
}
