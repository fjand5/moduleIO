#ifndef PINDEVICE 
#define PINDEVICE
#include "sysFunc.h"
class pinDevice
{
private:
	String _pin;
	String _state;
	uint32_t _beginTime;
	uint32_t _endTime;
public:
	pinDevice(){};
	pinDevice(String pin, String state,uint32_t beginTime,uint32_t endTime){
		_pin=pin;
		_state=state;
		_beginTime=beginTime;
		_endTime=endTime;
		pinMode(mapPin(_pin),OUTPUT);
	};

	void setState(String state){
		digitalWrite(mapPin(_pin),mapState(state));
	}
	bool getState(){
		return digitalRead(mapPin(_pin));
	}
	String getPin(){
		return _pin;
	}
	void setEndTime(uint32_t endTime){
		_endTime=endTime;
	}
	void setbeginTime(uint32_t beginTime){
		_beginTime=beginTime;
	}
	uint32_t getEndTime(){
		return _endTime;
	}
	uint32_t getbeginTime(){
		return _beginTime;
	}

};
#endif 