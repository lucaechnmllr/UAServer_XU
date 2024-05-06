
#ifndef PDU_STRING_H_
#define PDU_STRING_H_

#define PDU_STRING_HEADER_VERSION 11ul

#include "PDU.h"
#include "xu_pduobjecttype.h"
#include "xu_floatpduobjecttype.h"
#include "xu_intpduobjecttype.h"
#include <string>
#include <iomanip>
#include <limits>
#include <sstream>

namespace PDU {

	/** \brief Convert a String to a PDU
	*
	* Converts a string as output by the XUtoSTDOUT application into a process
	* data unit.
	*
	* Depending on the data type, either an intPDU or a floatPDU object will
	* be created.
	*
	* The memory for a PDU object is allocated with 'new'
	* --> Memory must be freed with delete by the caller.
	*
	* PDUs without a defined data type cannot be converted with this function!
	*
	* possible throws:
	* 		- std::invalid_argument
	* 		- std::runtime_error	    --> conversion failed (e.what() to log)
	* 		- std::length_error
	*
	*/

	/** \brief Convert an UaObject to a String
	*
	* Converts a PDUObject to a string. The string is formatted in
	* such a way that the Object can be transmitted to an XU using the STDINtoXU
	* application.
	*/
	template<typename T>
	std::string PDUObjecttoString(const XU::PDUObjectType& node,T value, char separator = ':')
	{
		// time_s:time_us:type:value:state:origin:index:kks

		std::ostringstream sstr;
		//!
		//! Set time to 0 
		//! 
		sstr << '0' << separator;	// time_s
		sstr << '0' << separator;	// time_us

		switch (node.getType())
		{
		case INT:
			sstr << 'i' << separator;
			sstr << value << separator;
			break;
		case FLOAT:
			sstr << 'f' << separator;
			sstr << std::scientific;
			sstr << std::setprecision(std::numeric_limits<float>::digits10 + 1);
			sstr << value << separator;
			break;
		default:
			sstr << "Node does not have a type. (" << node.getType() << ')';
			throw std::runtime_error(sstr.str());
			break;
		}

		//! Set State, Origin, Index and KKS
		sstr << node.getStateBitwise() << separator;	// State
		sstr << '0' << separator;	// Origin = 0
		sstr << '0' << separator;	// Index = 0


		sstr << node.nodeId().identifierString()->strContent;	// KKS

		sstr << '\n' << '!';

		return sstr.str();
	}


	basePDU *StringtoPDU(const std::string &string, char separator = ':');

	/*! \brief get the version of the header file
	*
	* only interesting if used as library.
	*/
	inline unsigned long getPDUStringHeaderVersion() { return PDU_STRING_HEADER_VERSION; }

	/*! \brief get the version of the source file
	*
	* only interesting if used as library.
	*/
	unsigned long getPDUStringLibVersion();
} // namespace PDU

//#ifndef __EXCEPTIONS
//static_assert(false, "Exceptions are mandatory.");
//#endif

#endif /* PDU_STRING_H_ */