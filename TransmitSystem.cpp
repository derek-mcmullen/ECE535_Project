#include "TransmitSystem.h"

#include <iostream>

void transmit(std::vector<double> &carrierOut, inData &configIn, std::vector<int> &messageIn) {
	
	// validate message 
	if ( validateMessageData(messageIn, configIn) ) {
		if (strcmp(configIn.mod_type.c_str(), "ask") == 0) {
			transmitASK(carrierOut, configIn, messageIn);
		} 
		else if (strcmp(configIn.mod_type.c_str(), "fsk") == 0) {
			transmitFSK(carrierOut, configIn, messageIn);
		}
		else if (strcmp(configIn.mod_type.c_str(), "psk") == 0) {
			transmitPSK(carrierOut, configIn, messageIn);
		}
	}
	else {
		std::cout << "Invalid message data provided. Will not transmit" << std::endl;
	}

	return;
}


bool validateMessageData(const std::vector<int>& messageIn, const inData & configIn)
{
	// upper limit of alphabet for n_ary scheme
	int maxAlpha = configIn.n_ary;

	// check each message symbol
	for (int i = 0; i < messageIn.size(); i++) {
		if (messageIn[i] < 0 || messageIn[i] > maxAlpha) {
			return false; 
		}
	}

	return true;
}

void transmitASK(std::vector<double> &carrierOut, inData &configIn, std::vector<int> &messageIn) {
	
	double duration = ( 1 / (double)configIn.symbol_rate ) * messageIn.size();		// in seconds
	double numTotalDataPts = ( duration / (1.0/configIn.carrier_freq) );

	int dataPointsPerSymbol = ( numTotalDataPts / messageIn.size() ); 

	// prep the carrier data points
	for (int i = 0; i < numTotalDataPts; i++) {
		carrierOut.push_back(sin(2 * configIn.carrier_freq * i));
	}

	// key the amplitudes
	int offset = 0;
	for (int i = 0; i < messageIn.size(); i++) {
		offset = i * dataPointsPerSymbol;
		for (int j = 0; j < dataPointsPerSymbol; j++) {
			carrierOut[offset + j] *= messageIn[i];
		}
	}

}

void transmitFSK(std::vector<double> &carrierOut, inData &configIn, std::vector<int> &messageIn) {

}

void transmitPSK(std::vector<double> &carrierOut, inData &configIn, std::vector<int> &messageIn) {

}