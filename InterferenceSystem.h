#ifndef InterferenceSystem_h
#define InterferenceSystem_h

#include "SystemConstants.h"

#include <vector>

class InterferenceSystem {

public:
	InterferenceSystem();
	void AWGN(std::vector<Complex> &dataOut, inData &configIn, std::vector<Complex> &dataIn);
private:

	double duration_;
	double numTotalDataPts_;
	int dataPointsPerSymbol_;
	int numSymbols_;

};

#endif

