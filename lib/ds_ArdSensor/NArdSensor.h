//
//  v1.1 - Added isWasPressed  //
//  v1.2 - delay replaced by request duration counter  //
//  v2.0 - used ArdPin_2.0  //
//  v2.1 - added callbacks tagged  //
//

#ifndef ArdSensor_h
#define ArdSensor_h

#include "Arduino.h"

#include "../ArdPin_v2.0/NArdPin.h"

class ArdSensor : public ArdPin {
	
  private:		
	void onChanged(bool newState);
	
	bool _activateValue = LOW;

  public:
	ArdSensor(int pin, 
			  bool activateValue = LOW, 
			  unsigned long requestDuration_ms = 500);
	
	ArdSensor(PCF8575 *pcf8575,
			  int pin, 
			  bool activateValue = LOW, 
			  unsigned long requestDuration_ms = 500);
	
    bool isActivating();

    void check();
	
	//
	//  Callbacks  //
	//
	
  private:
	void init_(bool activateValue);
	void (*_callbackOnActivated)();
	void (*_callbackOnDeactivated)();
	
  public:
	void setCallbackOnActivated(void (*callbackOnActivated)());
	void setCallbackOnDeactivated(void (*callbackOnDeactivated)());
	
	//
	//  Callbacks tags  //
	//
	
  private:
	int _tagActivated;
	void (*_callbackOnActivatedTagged)(int);
	
	int _tagDeactivated;
	void (*_callbackOnDeactivatedTagged)(int);
	
  public:
	void setCallbackOnActivatedTagged(void (*callbackOnActivatedTagged)(int), int tagActivated);
	void setCallbackOnDeactivatedTagged(void (*callbackOnDeactivatedTagged)(int), int tagDeactivated);
};

#endif