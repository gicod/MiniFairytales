#pragma once

#include <ds_i_pin.h>
#include <ds_ard_direct.h>
#include <ds_pcf8575_adapter.h>

/*!
* @class SimpleLed
* @brief decorator class to control a led on transistor
* @example
*         SimpleLed *upper_light = new SimpleLed(5);
*         upper_light->open();
*         ...
*         upper_light->close();
*/
class SimpleLed
{
public:
  explicit SimpleLed(const int &pin, const int &activation_on = HIGH);
  explicit SimpleLed(
    PCF8575 *pcf8575,
    const int &pin,
    const int &activation_on = HIGH
  );
  void on();
  void off();

  ~SimpleLed();

  SimpleLed(const SimpleLed&)             = delete;
  SimpleLed(SimpleLed&&)                  = delete;
  SimpleLed& operator=(const SimpleLed&)  = delete;
  SimpleLed& operator=(SimpleLed&&)       = delete;
protected:
  IPin *_hw_controller;
  int _active_level;
};