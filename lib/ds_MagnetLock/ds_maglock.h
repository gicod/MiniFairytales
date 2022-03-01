#pragma once

#include <ds_i_pin.h>
#include <ds_ard_direct.h>
#include <ds_pcf8575_adapter.h>

/*!
* @class MagnetLock
* @brief decorator class to control a magnet lock
* @example
*         MagnetLock *exit_door = new MagnetLock(5);
*         exit_door->open();
*         ...
*         exit_door->close();
*/
class MagnetLock
{
public:
  explicit MagnetLock(const int &pin, const int &activation_on = HIGH);
  explicit MagnetLock(
    PCF8575 *pcf8575,
    const int &pin,
    const int &activation_on = HIGH
  );
  void lock();
  void unlock();

  ~MagnetLock();

  MagnetLock(const MagnetLock&)             = delete;
  MagnetLock(MagnetLock&&)                  = delete;
  MagnetLock& operator=(const MagnetLock&)  = delete;
  MagnetLock& operator=(MagnetLock&&)       = delete;
protected:
  IPin *_hw_controller;
  int _active_level;
};