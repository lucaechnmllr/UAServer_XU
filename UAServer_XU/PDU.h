

#ifndef BASEPDU_H_
#define BASEPDU_H_
#define PDU_HEADER_VERSION 11ul

#include <stdexcept>
#include <string>
#ifdef POSIX
#include <sys/time.h>
#else
#pragma warning(push,2)
#include <WinSock2.h>
#endif
#pragma warning(pop)


namespace PDU {
//! Data type of the PDU value
enum TYPE {
	NONE, //!< No type defined, cannot be returned by any function
	INT,  //!< PDU contains an integer value
	FLOAT //!< PDU contains a float value
};

//! State of the value
enum STATE {
	VALID = 0,          //!< Value is valid
	UNINIT = (1 << 0),  //!< Value is uninitialized
	INVALID = (1 << 1), //!< Value is invalid
	REPVAL = (1 << 2),  //!< It is a replacement value
	SIMULAT = (1 << 3), //!< The value is simulated
	DOUBT = (1 << 4),   //!< State of the signal is doubtful
	HWFAIL = (1 << 5),  //!< There is a hardware error
	SERVICE = (1 << 6), //!< The system is currently maintained
	TIMADJ = (1 << 7)   //!< The time was adjusted
};

//! Signal origin
enum ORIGIN {
	PAST = (1 << 0), //!< from the past
					 /** \brief From the transition between past and present.
					 *
					 * At the time of the recording by the XU it was still present, but at
					 * the time of the transmission to the caller it is already past.
					 * The interpretation is up to the caller. (xu_fss.h)
					 */
	TRANSIT = (1 << 1),
	PRESENT = (1 << 2), //!< from the present
	IMAGE = (1 << 3),   //!< from an image
	EVENT = (1 << 4)    //!< from an event
};

//! PDU value
union Value {
	int i;   //!< Value is an integer
	float f; //!< value is an float
};

#ifndef POSIX
//! PDU time
typedef struct timeval {
	long tv_sec;
	long tv_usec;
} timeval;
#endif

/** \brief abstract PDU base class
*
*  Contains all data of a PDU, but cannot be used independently because
*  the data type of the value is not defined.
*
*  superclass of PDU::intPDU and PDU::floatPDU
*/
class basePDU {
public:
	/** \brief full constructor
	*
	* Initializes the PDU object with all the existing data.
	*/
	basePDU(Value value, const std::string &kks, timeval time, int origin, int state = 0);
	/** \brief simplified constructor
	*
	* Initializes the PDU object with all existing data, except the
	* time.
	*
	* Time is set to the actual systemtime.
	*/
	basePDU(Value value, const std::string &kks, int origin, int state = 0);
	/** \brief minimal Constructor
	*
	* Initializes the PDU object only with the absolutely required
	* values.
	*
	* State: valid
	*
	* Origin: present event
	*
	* Time: actual systemtime
	*/
	basePDU(Value value, const std::string &kks);

	//! Destructor of class basePDU
	virtual ~basePDU() {}

	//! Data type of the value
	virtual TYPE getType() const = 0;

	//! Get value as float
	virtual float getFloat() const;

	//! Get value as int.
	virtual int getInt() const;

	//! Set value as float
	virtual void setFloat(float value);

	//! Set value as int
	virtual void setInt(int value);

	//! Set state
	void setState(int state) { this->_state = state; }

	//! Set origin
	void setOrigin(int origin) { this->_origin = origin; }

	//! Get kks
	const std::string &getKKS() const { return _kks; }

	//! Get time
	virtual const timeval &getTime() const { return _time; }

	//! Get state of value
	int getState() const { return _state; }

	//! get origin of value
	int getOrigin() const { return _origin; }


	/*! \brief Get the system name
	*
	*  Returns the system-folder name for the OPC UA Address Space (Character 2,3,4 from KKS)
	*/
	std::string getFolderName() const;

	/*! \brief Get the signal name
	*
	*  Returns the signal-folder name for the OPC UA Address Space (Character 9,10 from KKS) 
	*/
	std::string getSignalName() const;


	//! set time to actual systemtime
	void actTime() {
		//set Systemtime------------------------------------------------
#ifdef POSIX
		gettimeofday(&_time, NULL);
#else
		timeval *tp = &_time;
		//struct timezone* tzp = NULL;

		SYSTEMTIME  system_time;
		FILETIME    file_time;
		uint64_t    time;
		static const uint64_t EPOCH = ((uint64_t)116444736000000000ULL);

		GetSystemTime(&system_time);
		SystemTimeToFileTime(&system_time, &file_time);
		time = ((uint64_t)file_time.dwLowDateTime);
		time += ((uint64_t)file_time.dwHighDateTime) << 32;

		tp->tv_sec = (long)((time - EPOCH) / 10000000L);
		tp->tv_usec = (long)(system_time.wMilliseconds * 1000);
		//---------------------------------------------------------------
#endif
	}
	

	//! += operator int value
	virtual void operator+=(int i) = 0;
	//! += operator float value
	virtual void operator+=(float f) = 0;
	//< -= operator int value
	virtual void operator-=(int i) = 0;
	//< -= operator float value
	virtual void operator-=(float f) = 0;
	//< /= operator int value
	virtual void operator/=(int i) = 0;
	//< /= operator float value
	virtual void operator/=(float f) = 0;
	//< *= operator int value
	virtual void operator*=(int i) = 0;
	//< *= operator float value
	virtual void operator*=(float f) = 0;

	//! operator = to copy a pdu
	virtual void operator=(const basePDU &pdu);
	//! operator = to set a int value
	virtual void operator=(int i) = 0;
	//! operator = to set a float value
	virtual void operator=(float f) = 0;

	//! compare int value (equal)
	virtual bool operator==(int i) const = 0;
	//! compare float value (equal)
	virtual bool operator==(float f) const = 0;
	//! compare int value (equal)
	virtual bool operator!=(int i) const = 0;
	//! compare float value (equal)
	virtual bool operator!=(float f) const = 0;
	//! compare int value (greater or equal)
	virtual bool operator>=(int i) const = 0;
	//! compare float value (greater or equal)
	virtual bool operator>=(float f) const = 0;
	//! compare int value (less or equal)
	virtual bool operator<=(int i) const = 0;
	//! compare float value (less or equal)
	virtual bool operator<=(float f) const = 0;
	//! compare int value (greater)
	virtual bool operator>(int i) const = 0;
	//! compare float value (greater)
	virtual bool operator>(float f) const = 0;
	//! compare int value (less)
	virtual bool operator<(int i) const = 0;
	//! compare float value (less)
	virtual bool operator<(float f) const = 0;

	//! Add an int value
	virtual int operator+(int i) const = 0;
	//! Subtract an int value
	virtual int operator-(int i) const = 0;
	//! Multiply an int value
	virtual int operator*(int i) const = 0;
	//! Divide an int value
	virtual int operator/(int i) const = 0;

	//! Add a float value
	virtual float operator+(float f) const = 0;
	//! Subtract a float value
	virtual float operator-(float f) const = 0;
	//! Multiply a float value
	virtual float operator*(float f) const = 0;
	//! Divide a float value
	virtual float operator/(float f) const = 0;

	//! conversion to int
	virtual operator int() const;

	//! conversion to float
	virtual operator float() const;

protected:
	//! Data of the PDU object
	Value _value;
	//! Signal identifier
	std::string _kks;
	//! Signal time
	timeval _time;
	//! Origin of value
	int _origin;
	//! state of value
	int _state;
};

/** \brief PDU with integer value
*
* Subclass of PDU::basePDU
*/
class intPDU : public basePDU {
public:
	/**\brief full constructor
	*
	* Same as in basePDU, but with a defined data type.
	*/
	intPDU(int value, const std::string &kks, timeval _time, int origin, int state = 0);
	/**\brief simplified constructor
	*
	* Same as in basePDU, but with a defined data type.
	*/
	intPDU(int value, const std::string &kks, int origin, int state = 0);
	/**\brief minimal constructor
	*
	* Same as in basePDU, but with a defined data type.
	*/
	intPDU(int value, const std::string &kks);
	//! Destructor of class intPDU
	virtual ~intPDU() {}
	//! Data type of the value
	virtual TYPE getType() const { return INT; }
	//! Get value as int.
	virtual int getInt() const { return _value.i; };
	//! Set value as int.
	virtual void setInt(int value) { this->_value.i = value; }

	//! += operator int value
	virtual void operator+=(int i) { _value.i += i; }
	//! += operator float value
	virtual void operator+=(float f) { _value.i += static_cast<int>(f); }
	//! -= operator int value
	virtual void operator-=(int i) { _value.i -= i; }
	//! -= operator float value
	virtual void operator-=(float f) { _value.i -= static_cast<int>(f); };
	//! /= operator int value
	virtual void operator/=(int i) { _value.i /= i; }
	//< /= operator float value
	virtual void operator/=(float f) { _value.i /= static_cast<int>(f); }
	//! *= operator int value
	virtual void operator*=(int i) { _value.i *= i; }
	//! *= operator float value
	virtual void operator*=(float f) { _value.i *= static_cast<int>(f); }

	// operator !
	int operator!() const { return !_value.i; };

	//! operator = int value
	virtual void operator=(int i) { _value.i = i; }
	//! operator = float value
	virtual void operator=(float f) { _value.i = static_cast<int>(f); }

	//! compare int value (equal)
	virtual bool operator==(int i) const { return _value.i == i; }
	//! compare float value (equal)
	virtual bool operator==(float f) const { return static_cast<float>(_value.i) == f; }
	//! compare int value (equal)
	virtual bool operator!=(int i) const { return _value.i != i; }
	//! compare float value (equal)
	virtual bool operator!=(float f) const { return static_cast<float>(_value.i) != f; }
	//! compare int value (greater or equal)
	virtual bool operator>=(int i) const { return _value.i >= i; }
	//! compare float value (greater or equal)
	virtual bool operator>=(float f) const { return static_cast<float>(_value.i) >= f; }
	//! compare int value (less or equal)
	virtual bool operator<=(int i) const { return _value.i <= i; }
	//! compare float value (less or equal)
	virtual bool operator<=(float f) const { return static_cast<float>(_value.i) <= f; }
	//! compare int value (greater)
	virtual bool operator>(int i) const { return _value.i > i; }
	//! compare float value (greater)
	virtual bool operator>(float f) const { return static_cast<float>(_value.i) > f; }
	//! compare int value (less)
	virtual bool operator<(int i) const { return _value.i < i; }
	//! compare float value (less)
	virtual bool operator<(float f) const { return static_cast<float>(_value.i) < f; }

	//! Add an int value
	virtual int operator+(int i) const { return _value.i + i; }
	//! Subtract an int value
	virtual int operator-(int i) const { return _value.i - i; }
	//! Multiply an int value
	virtual int operator*(int i) const { return _value.i * i; }
	//! Divide an int value
	virtual int operator/(int i) const { return _value.i / i; }

	//! Add a float value
	virtual float operator+(float f) const { return static_cast<float>(_value.i) + f; }
	//! Subtract a float value
	virtual float operator-(float f) const { return static_cast<float>(_value.i) - f; }
	//! Multiply a float value
	virtual float operator*(float f) const { return static_cast<float>(_value.i) * f; }
	//! Divide a float value
	virtual float operator/(float f) const { return static_cast<float>(_value.i) / f; }

	//! conversion to int
	virtual operator int() const { return _value.i; };
};

/** \brief PDU with float value
*
* Subclass of PDU::basePDU
*/
class floatPDU : public basePDU {
public:
	/**\brief full constructor
	*
	* Same as in basePDU, but with a defined data type.
	*/
	floatPDU(float value, const std::string &kks, timeval _time, int origin, int state = 0);
	/**\brief simplified constructor
	*
	* Same as in basePDU, but with a defined data type.
	*/
	floatPDU(float value, const std::string &kks, int origin, int state = 0);
	/**\brief minimal constructor
	*
	* Same as in basePDU, but with a defined data type.
	*/
	floatPDU(float value, const std::string &kks);
	//! Destructor of class floatPDU
	virtual ~floatPDU() {};
	//! Data type of the value
	virtual TYPE getType() const { return FLOAT; }
	//! Get value as float.
	virtual float getFloat() const { return _value.f; }
	//! Set value as float.
	virtual void setFloat(float value) { this->_value.f = value; }

	//! += operator int value
	virtual void operator+=(int i) { _value.f += static_cast<float>(i); }
	//! += operator float value
	virtual void operator+=(float f) { _value.f += f; }
	//< -= operator int value
	virtual void operator-=(int i) { _value.f -= static_cast<float>(i); }
	//< -= operator float value
	virtual void operator-=(float f) { _value.f -= f; };
	//< /= operator int value
	virtual void operator/=(int i) { _value.f /= static_cast<float>(i); };
	//< /= operator float value
	virtual void operator/=(float f) { _value.f /= f; }
	//< *= operator int value
	virtual void operator*=(int i) { _value.f *= static_cast<float>(i); }
	//< *= operator float value
	virtual void operator*=(float f) { _value.f /= f; }

	//! operator = int value
	virtual void operator=(int i) { _value.f = static_cast<float>(i); }
	//! operator = float value
	virtual void operator=(float f) { _value.f = f; };

	//! compare int value (equal)
	virtual bool operator==(int i) const { return _value.f == static_cast<float>(i); }
	//! compare float value (equal)
	virtual bool operator==(float f) const { return _value.f == f; }
	//! compare int value (equal)
	virtual bool operator!=(int i) const { return _value.f != static_cast<float>(i); }
	//! compare float value (equal)
	virtual bool operator!=(float f) const { return _value.f != f; }
	//! compare int value (greater or equal)
	virtual bool operator>=(int i) const { return _value.f >= static_cast<float>(i); }
	//! compare float value (greater or equal)
	virtual bool operator>=(float f) const { return _value.f >= f; }
	//! compare int value (less or equal)
	virtual bool operator<=(int i) const { return _value.f <= static_cast<float>(i); }
	//! compare float value (less or equal)
	virtual bool operator<=(float f) const { return _value.f <= f; }
	//! compare int value (greater)
	virtual bool operator>(int i) const { return _value.f > static_cast<float>(i); }
	//! compare float value (greater)
	virtual bool operator>(float f) const { return _value.f > f; }
	//! compare int value (less)
	virtual bool operator<(int i) const { return _value.f < static_cast<float>(i); }
	//! compare float value (less)
	virtual bool operator<(float f) const { return _value.f < f; }

	//! Add an int value
	virtual int operator+(int i) const { return static_cast<int>(_value.f) + i; }
	//! Subtract an int value
	virtual int operator-(int i) const { return static_cast<int>(_value.f) - i; }
	//! Multiply an int value
	virtual int operator*(int i) const { return static_cast<int>(_value.f) * i; }
	//! Divide an int value
	virtual int operator/(int i) const { return static_cast<int>(_value.f) / i; }

	//! Add a float value
	virtual float operator+(float f) const { return _value.f + f; }
	//! Subtract a float value
	virtual float operator-(float f) const { return _value.f - f; }
	//! Multiply a float value
	virtual float operator*(float f) const { return _value.f * f; }
	//! Divide a float value
	virtual float operator/(float f) const { return _value.f / f; }

	//! conversion to int
	virtual operator int() const { return static_cast<int>(_value.f); }

	//! conversion to float
	virtual operator float() const { return _value.f; };
};

/*! \brief get the version of the header file
*
* only interesting if used as library.
*/
inline unsigned long getPDUHeaderVersion() { return PDU_HEADER_VERSION; }

/*! \brief get the version of the source file
*
* only interesting if used as library.
*/
unsigned long getPDULibVersion();
} // namespace PDU



//#ifndef __EXCEPTIONS
//static_assert(false, "Exceptions are mandatory.");
//#endif

static_assert(sizeof(int) == sizeof(float), "int must be the same size as float");

#endif /* BASEPDU_H_ */