#include "TransmitSystem.h"

#include <iostream>

TransmitSystem::TransmitSystem() :
	duration_(10),
	numTotalDataPts_(100),
	dataPointsPerSymbol_(10)
{}; 

void TransmitSystem::transmit(std::vector<double> &carrierOut, inData &configIn, std::vector<int> &messageIn) {
	
	duration_ = (1 / (double)configIn.symbol_rate) * messageIn.size();		// in seconds
	numTotalDataPts_ = (duration_ / (1.0 / configIn.carrier_freq));

	dataPointsPerSymbol_ = (numTotalDataPts_ / messageIn.size());

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


bool TransmitSystem::validateMessageData(const std::vector<int>& messageIn, const inData & configIn)
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

void TransmitSystem::transmitASK(std::vector<double> &carrierOut, inData &configIn, std::vector<int> &messageIn) {

	// prep the carrier data points
	for (int i = 0; i < numTotalDataPts_; i++) {
		carrierOut.push_back(sin(2 * configIn.carrier_freq * i));
	}

	// key the amplitudes
	int offset = 0;
	for (int i = 0; i < messageIn.size(); i++) {
		offset = i * dataPointsPerSymbol_;
		for (int j = 0; j < dataPointsPerSymbol_; j++) {
			carrierOut[offset + j] *= messageIn[i];
		}
	}

}

void TransmitSystem::transmitFSK(std::vector<double> &carrierOut, inData &configIn, std::vector<int> &messageIn) {
	double freqStep = configIn.freq_dev * 2 / configIn.n_ary; 

	double modulatedFreq = configIn.carrier_freq; 

	for (int i = 0; i < messageIn.size(); i++) {
		switch (messageIn[i]) {
		case 0:
			modulatedFreq = configIn.carrier_freq - (1 * freqStep);
			break;
		case 1:
			modulatedFreq = configIn.carrier_freq + (1 * freqStep);
			break;
		case 2:
			modulatedFreq = configIn.carrier_freq - (2 * freqStep);
			break;
		case 3:
			modulatedFreq = configIn.carrier_freq + (2 * freqStep);
			break;
		case 4:
			modulatedFreq = configIn.carrier_freq - (3 * freqStep);
			break;
		case 5:
			modulatedFreq = configIn.carrier_freq + (3 * freqStep);
			break;
		case 6:
			modulatedFreq = configIn.carrier_freq - (4 * freqStep);
			break;
		case 7:
			modulatedFreq = configIn.carrier_freq + (4 * freqStep);
			break;
		case 8:
			modulatedFreq = configIn.carrier_freq - (5 * freqStep);
			break;
		case 9:
			modulatedFreq = configIn.carrier_freq + (5 * freqStep);
			break;
		case 10:
			modulatedFreq = configIn.carrier_freq - (6 * freqStep);
			break;
		case 11:
			modulatedFreq = configIn.carrier_freq + (6 * freqStep);
			break;
		case 12:
			modulatedFreq = configIn.carrier_freq - (7 * freqStep);
			break;
		case 13:
			modulatedFreq = configIn.carrier_freq + (7 * freqStep);
			break;
		case 14:
			modulatedFreq = configIn.carrier_freq - (8 * freqStep);
			break;
		case 15:
			modulatedFreq = configIn.carrier_freq + (8 * freqStep);
			break;
		}
		for (int j = 0; j < dataPointsPerSymbol_; j++) {
			carrierOut.push_back(sin(2 * modulatedFreq * i));
		}
	}

}

void TransmitSystem::transmitPSK(std::vector<double> &carrierOut, inData &configIn, std::vector<int> &messageIn) {

}