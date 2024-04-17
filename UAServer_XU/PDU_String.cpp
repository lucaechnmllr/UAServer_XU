
#include "PDU_String.h"
#include <cstdio>
#include <cstring>
#include <stdexcept>
#include <iostream>
#include <string>


PDU::basePDU *PDU::StringtoPDU(const std::string &string, char separator) {
	basePDU *retval = NULL;


	// ------------------ check seperator --------------------------------------
	// -------------------------------------------------------------------------
	if (!ispunct(separator) || separator == '.') {
		std::ostringstream sstr;
		sstr << "The Character '" << separator << "' can not be used as "
			<< "separator.";
		throw std::invalid_argument(sstr.str());
	}

	// ------------------ check length -----------------------------------------
	// -------------------------------------------------------------------------
	if (string.size() < 15) {
		throw std::length_error("The input string is too short to contain "
			"all information of a PDU.");
	}

	// ------------------ create local copy of string --------------------------
	// -------------------------------------------------------------------------s
	char *localstring = new char[string.size() + 1];
	localstring[string.size()] = 0; // zero termination
	strcpy(localstring, string.c_str());

	// ------------------ replace and check separators -------------------------
	// -------------------------------------------------------------------------
	unsigned separators = 0;
	for (unsigned i = 0; i < strlen(localstring); i++) {
		if (string[i] == separator) {
			localstring[i] = ':';
			separators++;
		}
	}
	if (separators != 7) {
		delete[] localstring;
		throw std::invalid_argument("The string does not contain the proper number of separators.");
	}

	// temporary data
	timeval temp_time{};
	PDU::TYPE temp_type;
	int temp_origin;
	int temp_state;
	PDU::Value temp_value{};
	std::string temp_kks;

	char *ptr = localstring;

	// ------------------ convert seconds --------------------------------------
	// -------------------------------------------------------------------------
	if (!sscanf(ptr, "%ld:", &(temp_time.tv_sec))) {
		delete[] localstring;
		throw std::invalid_argument("Failed to read seconds.");
	}

	// move pointer to next separator in string
	do {
		ptr++;
		// check for end of string
		if (ptr[0] == '\0') {
			delete[] localstring;
			throw std::length_error("inputstring is to short. Aborted at "
				"'seconds'");
		}
	} while (ptr[0] != ':');

	if (!sscanf(ptr, ":%ld:", &(temp_time.tv_usec))) {
		delete[] localstring;
		throw std::invalid_argument("Failed to read microseconds.");
	}
	// move pointer to next separator in string
	do {
		ptr++;
		// check for end of string
		if (ptr[0] == '\0') {
			delete[] localstring;
			throw std::length_error("inputstring is to short: Aborted at "
				"'microseconds'");
		}
	} while (ptr[0] != ':');

	// ------------------ convert type -----------------------------------------
	// -------------------------------------------------------------------------
	switch (*(ptr + 1)) {
	case 'i':
		temp_type = PDU::TYPE::INT;
		break;
	case 'f':
		temp_type = PDU::TYPE::FLOAT;
		break;
	default:
		delete[] localstring;
		throw std::runtime_error("Valuetype is invalid");
	}
	do {
		ptr++;
		// check for end of string
		if (ptr[0] == '\0') {
			delete[] localstring;
			throw std::length_error("inputstring is to short: Aborted at "
				"'type'");
		}
	} while (ptr[0] != ':');

	// ------------------ convert value ----------------------------------------
	// -------------------------------------------------------------------------
	int value_return;
	switch (temp_type) {
	case PDU::TYPE::INT:
		int ival;
		value_return = sscanf(ptr, ":%i:", &ival);
		temp_value.i = ival;
		break;
	case PDU::TYPE::FLOAT:
		float fval;
		value_return = sscanf(ptr, ":%f:", &fval);
		temp_value.f = fval;
		break;
	default:
		delete[] localstring;
		throw std::runtime_error("Valuetype is invalid");
	}
	if (!value_return) {
		delete[] localstring;
		throw std::invalid_argument("Failed to read value.");
	}
	// move pointer to next separator in string
	do {
		ptr++;
		// check for end of string
		if (ptr[0] == '\0') {
			delete[] localstring;
			throw std::length_error("inputstring is to short: Aborted at "
				"'value'");
		}
	} while (ptr[0] != ':');

	// ------------------ convert state ----------------------------------------
	// -------------------------------------------------------------------------
	if (!sscanf(ptr, ":%d:", &(temp_state))) {
		delete[] localstring;
		throw std::invalid_argument("Failed to read state.");
	}
	// move pointer to next separator in string
	do {
		ptr++;
		// check for end of string
		if (ptr[0] == '\0') {
			delete[] localstring;
			throw std::length_error("inputstring is to short Aborted at "
				"'state'");
		}
	} while (ptr[0] != ':');

	// ------------------ convert origin ---------------------------------------
	// -------------------------------------------------------------------------
	if (!sscanf(ptr, ":%i:", &(temp_origin))) {
		delete[] localstring;
		throw std::invalid_argument("Failed to read origin.");
	}
	// move pointer to next separator in string
	do {
		ptr++;
		// check for end of string
		if (ptr[0] == '\0') {
			delete[] localstring;
			throw std::length_error("inputstring is to short: Aborted at "
				"'origin'");
		}
	} while (ptr[0] != ':');

	// ------------------ skip index -------------------------------------------
	// -------------------------------------------------------------------------
	do {
		ptr++;
		// check for end of string
		if (ptr[0] == '\0') {
			delete[] localstring;
			throw std::length_error("inputstring is to short Aborted at "
				"'index'");
		}
	} while (ptr[0] != ':');

	// ------------------ convert kks ------------------------------------------
	// -------------------------------------------------------------------------
	// 
	// additional increment do get first char of kks instead of separator
	temp_kks = ++ptr;

	if (temp_time.tv_sec <= 0 && temp_time.tv_usec <= 0) {
		//set Systemtime------------------------------------------------
#ifdef POSIX
		gettimeofday(&(temp_time), NULL);
		temp_state |= TIMADJ;
#else
		timeval *tp = &temp_time;
		struct timezone* tzp = NULL;

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
		temp_state |= TIMADJ;
#endif
	}


	switch (temp_type) {
	case INT:
		retval = new intPDU(temp_value.i, temp_kks, temp_time, temp_origin, temp_state);
		break;
	case FLOAT:
		retval = new floatPDU(temp_value.f, temp_kks, temp_time, temp_origin, temp_state);
	default:
		// Nothing exception would have been thrown earlier
		break;
	}

	delete[] localstring;



	return retval;
}





