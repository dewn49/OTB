#ifndef MisregistrationRecord_h
#define MisregistrationRecord_h

#include <iostream>

#include <ossim/imaging/RadarSat/RadarSatRecord.h>
/**
 * @ingroup DataQualitySummaryRecord
 * @brief This class is able to read a misregistration record
 * @author Magellium, Pacome Dentraygues
 * @version 1.0
 * @date 23-11-07
 */
class MisregistrationRecord
{
public:
	/**
	 * @brief Constructor
	 */
	MisregistrationRecord();

	/**
	 * @brief Destructor
	 */
	~MisregistrationRecord();

	/**
	 * @brief Copy constructor
	 */
	MisregistrationRecord(const MisregistrationRecord& rhs);

	/**
	 * @brief Copy operator
	 */
	MisregistrationRecord& operator=(const MisregistrationRecord& rhs);
	/**
	 * @brief This function write the MisregistrationRecord in a stream
	 */
	friend std::ostream& operator<<(std::ostream& os, const MisregistrationRecord& data);

	/**
	 * @brief This function read a MisregistrationRecord from a stream
	 */
	friend std::istream& operator>>(std::istream& is, MisregistrationRecord& data);

	/**
	 * @brief Nominal along track misregistration
	 */
	double   get_alt_m()
	{
		return _alt_m;
	};
    /**
	 * @brief Nominal cross track misregistration 
	 */
	double   get_crt_m()
	{
		return _crt_m;
	};
protected:

	/**
	 * @brief Nominal along track misregistration
	 */
	double   _alt_m;
    /**
	 * @brief Nominal cross track misregistration 
	 */
	double   _crt_m;
private:


};

#endif