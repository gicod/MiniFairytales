#include "NArdSensor.h"

ArdSensor::ArdSensor(int pin, 
					 bool activateValue, 
					 unsigned long requestDuration_ms) 
					 : ArdPin(pin, 
							  requestDuration_ms) {
	this->init_(activateValue);
}

ArdSensor::ArdSensor(PCF8575 *pcf8575,
					int pin,
					bool activateValue,
					unsigned long requestDuration_ms):
	ArdPin(pcf8575, pin, requestDuration_ms) {
	this->init_(activateValue);
}

void ArdSensor::check() {
	ArdPin::check();
}

void ArdSensor::onChanged(bool newState) {
	ArdPin::onChanged(newState);
	
	//  check on pressed  //
	if(newState == _activateValue) {
		//  emit onPressed  //
		if(_callbackOnActivated != NULL) _callbackOnActivated();
		if(_callbackOnActivatedTagged != NULL) _callbackOnActivatedTagged(_tagActivated);
	} 
	
	//  check on released  //
	else {
		//  emit onReleased  //
		if(_callbackOnDeactivated != NULL) _callbackOnDeactivated();
		if(_callbackOnDeactivatedTagged != NULL) _callbackOnDeactivatedTagged(_tagDeactivated);		
	}
}

bool ArdSensor::isActivating() {
	return getValue() == _activateValue;
}

void ArdSensor::init_(bool activateValue) {
	_activateValue = activateValue;
	
	_callbackOnActivated = NULL;
	_callbackOnDeactivated = NULL;
	
	_tagActivated = 0;
	_callbackOnActivatedTagged = NULL;
	
	_tagDeactivated = 0;
	_callbackOnDeactivatedTagged = NULL;
}

void ArdSensor::setCallbackOnActivated(void (*callbackOnActivated)()) {  
	_callbackOnActivated = callbackOnActivated;
}

void ArdSensor::setCallbackOnDeactivated(void (*callbackOnDeactivated)()) {  
	_callbackOnDeactivated = callbackOnDeactivated;
}

void ArdSensor::setCallbackOnActivatedTagged(void (*callbackOnActivatedTagged)(int), int tagActivated) {  
	_callbackOnActivatedTagged = callbackOnActivatedTagged;
	_tagActivated = tagActivated;
}

void ArdSensor::setCallbackOnDeactivatedTagged(void (*callbackOnDeactivatedTagged)(int), int tagDeactivated) {  
	_callbackOnDeactivatedTagged = callbackOnDeactivatedTagged;
	_tagDeactivated = tagDeactivated;
}
