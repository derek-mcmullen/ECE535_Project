#include "TransmitSystem.h"

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


void transmitASK(std::vector<double> &carrierOut, inData &configIn, std::vector<int> &messageIn) {
	
	int ratio = configIn.carrier_freq / configIn.sample_rate;
	int numTotalDataPts = configIn.samples_per_bit * messageIn.size() * ratio;

	// prep the carrier data points
	for (int i = 0; i < numTotalDataPts; i++) {
		carrierOut.push_back( sin( 2*M_PI*i/configIn.carrier_freq) ); 
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

void transmitFSK(std::vector<double> &carrierOut, inData &configIn, std::vector<int> &messageIn) {

}

void transmitPSK(std::vector<double> &carrierOut, inData &configIn, std::vector<int> &messageIn) {

}