#include "PDU.h"
#include <iostream>

using std::cerr; using std::endl;


PDU::basePDU::basePDU(Value value, const std::string &kks, timeval _time, int origin, int state)
	: _value(value), _kks(kks), _time(_time), _origin(origin), _state(state) {}

PDU::basePDU::basePDU(Value value, const std::string &kks, int origin, int state)
	: _value(value), _kks(kks), _time{ 0 }, _origin(origin), _state(state) {}

PDU::basePDU::basePDU(Value value, const std::string &kks)
	: _value(value), _kks(kks), _time{ 0 }, _origin(PRESENT | EVENT), _state(VALID) {}

float PDU::basePDU::getFloat() const {
	throw std::runtime_error("Call of function 'getFloat()' is not allowed for"
		" this kind of object");
	//cerr << "Call of function 'getFloat()' is not allowed for this kind of object" << endl;
}

int PDU::basePDU::getInt() const {
	throw std::runtime_error("Call of function 'getInt()' is not allowed for"
		" this kind of object");
	//cerr << "Call of function 'getInt()' is not allowed for this kind of object" << endl;
}

void PDU::basePDU::setFloat(float value) {
	throw std::runtime_error("Call of function 'setFloat()' is not allowed for"
		" this kind of object");
	//cerr << "Call of function 'setFloat()' is not allowed for this kind of object" << endl;
	// Dummy to avoid error;
	if (value == 0) {
		;
	}
}

void PDU::basePDU::setInt(int value) {
	throw std::runtime_error("Call of fuction 'setInt()' is not allowed for"
		" this kind of object");
	//cerr << "Call of function 'setInt()' is not allowed for this kind of object" << endl;
	if (value == 0) {
		;
	}
}

std::string PDU::basePDU::getFolderName() const 
{
	return getKKS().substr(1, 3);
}

std::string PDU::basePDU::getSignalName() const
{
	return getKKS().substr(8, 2);
}


PDU::intPDU::intPDU(int value, const std::string &kks, timeval _time, int origin, int state)
	: basePDU({ 0 }, kks, _time, origin, state) {
	this->_value.i = value;
}

PDU::intPDU::intPDU(int value, const std::string &kks, int origin, int state) : basePDU({ 0 }, kks, _time, origin, state) {
	this->_value.i = value;
}

PDU::intPDU::intPDU(int value, const std::string &kks) : basePDU({ 0 }, kks) { this->_value.i = value; }

PDU::floatPDU::floatPDU(float value, const std::string &kks, timeval _time, int origin, int state)
	: basePDU({ 0 }, kks, _time, origin, state) {
	this->_value.f = value;
}

PDU::floatPDU::floatPDU(float value, const std::string &kks, int origin, int state)
	: basePDU({ 0 }, kks, _time, origin, state) {
	this->_value.f = value;
}

PDU::floatPDU::floatPDU(float value, const std::string &kks) : basePDU({ 0 }, kks) { this->_value.f = value; }

void PDU::basePDU::operator=(const basePDU &pdu) {
	this->_value = pdu._value;
	this->_time = pdu._time;
	this->_kks = pdu._kks;
	this->_origin = pdu._origin;
	this->_state = pdu._state;
}

PDU::basePDU::operator float() const {
	throw std::runtime_error("Call of function 'operator float()' is not allowed"
		" for this kind of object");
	//cerr << "Call of function 'operator float()' is not allowed for this kind of object" << endl;
}

PDU::basePDU::operator int() const {
	throw std::runtime_error("Call of function 'operator int()' is not allowed"
		" for this kind of object");
	//cerr << "Call of function 'operator int()' is not allowed for this kind of object" << endl;
}




unsigned long PDU::getPDULibVersion() { return PDU_HEADER_VERSION; }
