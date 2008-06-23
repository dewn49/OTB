#ifndef PositionVectorRecord_h
#define PositionVectorRecord_h

#include <iostream>
#include <ossim/imaging/RadarSat/RadarSatRecord.h>
/**
 * @ingroup PlatformPositionDataRecord
 * @brief This class is able to read a position vector record
 * @author Magellium, Pacome Dentraygues
 * @version 1.0
 * @date 23-11-07
 */
class PositionVectorRecord
{
public:
	/**
	 * @brief Constructor
	 */
	PositionVectorRecord();

	/**
	 * @brief Destructor
	 */
	~PositionVectorRecord();

	/**
	 * @brief Copy constructor
	 */
	PositionVectorRecord(const PositionVectorRecord& rhs);

	/**
	 * @brief Copy operator
	 */
	PositionVectorRecord& operator=(const PositionVectorRecord& rhs);
	/**
	 * @brief This function writes the PositionVectorRecord in a stream
	 */
	friend std::ostream& operator<<(std::ostream& os, const PositionVectorRecord& data);

	/**
	 * @brief This function reads a PositionVectorRecord from a stream
	 */
	friend std::istream& operator>>(std::istream& is, PositionVectorRecord& data);

	/**
	 * @brief Data point position (m)
	 */
	double* get_pos()
	{
		return _pos;
	};

	/** 
	 * @brief Data point velocity (mm/s)
	 */
	double* get_vel()
	{
		return _vel;
	};

protected:
	/**
	 * @brief Data point position (m)
	 */
	double _pos[3];
	/** 
	 * @brief Data point velocity (mm/s)
	 */
	double _vel[3];
private:


};

#endif