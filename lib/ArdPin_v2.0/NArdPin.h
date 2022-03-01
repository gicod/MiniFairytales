#pragma once

#include <ds_i_pin.h>
#include <ds_ard_direct.h>
#include <ds_pcf8575_adapter.h>

class ArdPin {
public:
	ArdPin(int pin, unsigned long requestDuration = 500);
	ArdPin(PCF8575 *pcf8575, int pin, unsigned long requestDuration = 500);
	void setCallbackOnChanged(void (*callbackOnChanged)(bool newState));
	bool getDirectValue();
	int getValue();	
	void check();

protected:
	virtual void onChanged(bool newState);

private:
	void _init(unsigned long requestDuration);
	IPin *_hw_controller;
	unsigned long _requestDuration;
	int _lastValue;

	bool isHighDetected;
	unsigned long valueHigh_ts;

	bool isLowDetected;
	unsigned long valueLow_ts;

	void (*_callbackOnChanged)(bool newState);
};