#include "TransmitSystem.h"

#include <iostream>

void transmit(std::vector<double> &carrierOut, inData &configIn, std::vector<int> &messageIn) {
	

	if (strcmp(configIn.mod_type.c_str(), "ask") == 0) {
		transmitASK(carrierOut, configIn, messageIn);
	} 
	else if (strcmp(configIn.mod_type.c_str(), "fsk") == 0) {
		transmitFSK(carrierOut, configIn, messageIn);
	}
	else if (strcmp(configIn.mod_type.c_str(), "psk") == 0) {
		transmitPSK(carrierOut, configIn, messageIn);
	}
	
	return;
}


bool validateMessageData(const std::vector<int>& messageIn, const inData & configIn)
{
	// upper limit of alphabet for n_ary scheme
	int maxAlpha = 1;
	if (configIn.n_ary == 4) {
		maxAlpha = 2; 
	} 
	else if (configIn.n_ary == 8) {
		maxAlpha = 3;
	}
	else if (configIn.n_ary == 16) {
		maxAlpha = 4;
	}

	// check each message symbol
	for (int i = 0; i < messageIn.size(); i++) {
		if (messageIn[i] < 0 || messageIn[i] > maxAlpha) {
			return false; 
		}
	}

	return true;
}

void transmitASK(std::vector<double> &carrierOut, inData &configIn, std::vector<int> &messageIn) {
	
	// validate message 
	if (validateMessageData(messageIn, configIn) ) {

		int ratio = configIn.carrier_freq / configIn.sample_rate;
		int numTotalDataPts = configIn.samples_per_bit * messageIn.size() * ratio;

		// prep the carrier data points
		for (int i = 0; i < numTotalDataPts; i++) {
			carrierOut.push_back(sin(2 * M_PI * i / configIn.sample_rate));
		}

		// scale the amplitudes
		int offset = 0;
		for (int i = 0; i < messageIn.size(); i++) {
			offset = i * configIn.samples_per_bit*ratio;
			for (int j = 0; j < (configIn.samples_per_bit*ratio); j++) {
				carrierOut[offset + j] *= messageIn[i];
			}
		}
	}
	else {
		std::cout << "Invalid message data provided. Will not transmit" << std::endl; 
	}
}

void transmitFSK(std::vector<double> &carrierOut, inData &configIn, std::vector<int> &messageIn) {

}

void transmitPSK(std::vector<double> &carrierOut, inData &configIn, std::vector<int> &messageIn) {

}