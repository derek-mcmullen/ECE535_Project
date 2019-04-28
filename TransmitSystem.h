#ifndef TransmitSystem_h
#define TransmitSystem_h

#define _USE_MATH_DEFINES

#include "ConfigLoader.h"
#include "SystemConstants.h"

#include <cmath>   
#include <string>
#include <algorithm>
#include <vector>

class TransmitSystem {

public:
	TransmitSystem(); 
	void transmit(std::vector<Complex> &data, inData &in, std::vector<int> &message);
private:
	bool validateMessageData(const std::vector<int> &messageIn, const inData &configIn);
	void parseMessage(std::vector<int> &messageOut, inData &configIn, std::vector<int> &messageIn); 

	void transmitASK(std::vector<Complex> &carrierOut, inData &configIn, std::vector<int> &messageIn);
	void transmitFSK(std::vector<Complex> &carrierOut, inData &configIn, std::vector<int> &messageIn);
	void transmitPSK(std::vector<Complex> &carrierOut, inData &configIn, std::vector<int> &messageIn);

	double duration_; 
	double numTotalDataPts_; 
	int dataPointsPerSymbol_; 
}; 

#endif // !TransmitSystem_h

