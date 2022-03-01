#include "NArdPin.h"

ArdPin::ArdPin(int pin, unsigned long requestDuration):
	_hw_controller(new ArdDirect(pin))
{
	this->_init(requestDuration);	
}

ArdPin::ArdPin(PCF8575 *pcf8575, int pin, unsigned long requestDuration):
	_hw_controller(new PCF8575Adapter(pcf8575, pin))
{
	this->_init(requestDuration);
}

void ArdPin::setCallbackOnChanged(void (*callbackOnChanged)(bool newState)) {
  _callbackOnChanged = callbackOnChanged;
}

bool ArdPin::getDirectValue() {
  return this->_hw_controller->digital_read() == HIGH;
}

/*!
	@todo: DRY
*/
void ArdPin::check() {
	auto is_timeout = [this] (unsigned long value_ts) -> bool {
		return millis() - value_ts > this->_requestDuration;
	};
	const bool currentValue = getDirectValue();

	if(currentValue) { // check HIGH

		if(this->_lastValue == LOW) { //  check new HIGH
			this->valueHigh_ts = millis();
		}

		//  check after delay (once)
		if(this->isHighDetected == false && is_timeout(this->valueHigh_ts)) {
			this->isHighDetected = true;
			onChanged(HIGH);
		}
		this->isLowDetected = false; //  clear flag
	} else {//  check LOW  	

		if(this->_lastValue == HIGH) { //  check new LOW
			this->valueLow_ts = millis();
		}

		//  check after delay (once)
		if(this->isLowDetected == false && is_timeout(this->valueLow_ts)) {
			this->isLowDetected = true;
			this->onChanged(LOW);
		}		
		this->isHighDetected = false; //  clear flag
	}
	this->_lastValue = currentValue;
}

void ArdPin::onChanged(bool newState) {
	if(_callbackOnChanged != nullptr) {
		_callbackOnChanged(newState);
	}
}

int ArdPin::getValue() {
	if(isHighDetected) {
		return 1;
	}
	else if(isLowDetected) {
		return 0;
	}
	else {
		return -1;
	}
}

/*!
	@brief puts default values into fields and calls pinMode for _hw_controller
*/
void ArdPin::_init(unsigned long requestDuration)
{
	this->_requestDuration = requestDuration;
	this->_lastValue = -1;
	this->isHighDetected = false;
	this->valueHigh_ts = 0;
	this->isLowDetected = false;
	this->valueLow_ts = 0;
	this->_callbackOnChanged = nullptr;
	this->_hw_controller->setup(INPUT_PULLUP);
}