#ifndef ReceiveSystem_h
#define ReceiveSystem_h

#define _USE_MATH_DEFINES

#include "ConfigLoader.h"

#include <cmath>   
#include <string>
#include <algorithm>
#include <vector>

class ReceiveSystem {

public:
	ReceiveSystem();
	void receive(std::vector<int> &messageOut, inData &configIn, std::vector<Complex> &rawDataIn);
private: 
	void receiveASK(std::vector<int> &messageOut, inData &configIn, std::vector<Complex> &rawDataIn);
	void receiveFSK(std::vector<int> &messageOut, inData &configIn, std::vector<Complex> &rawDataIn);
	void receivePSK(std::vector<int> &messageOut, inData &configIn, std::vector<Complex> &rawDataIn);

	void unparseMessage(std::vector<int> &messageOut, inData &configIn, std::vector<int> &symbols);
	int makeASKSymbolDetermination(double &val);
	int makeFSKSymbolDetermination(double &val, inData &configIn);

	double duration_;
	double numTotalDataPts_;
	int dataPointsPerSymbol_;
	int numSymbols_; 
	double askThreshold_; 

};


#endif // !ReceiveSystem_h
