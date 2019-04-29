#include "TransmitSystem.h"

#include <iostream>

TransmitSystem::TransmitSystem() :
	duration_(10),
	numTotalDataPts_(100),
	dataPointsPerSymbol_(10)
{}; 

void TransmitSystem::transmit(std::vector<Complex> &carrierOut, inData &configIn, std::vector<int> &messageIn) {
	
	// prep data for TX by creating symbols
	std::vector<int> parsedData;
	parseMessage(parsedData, configIn, messageIn);

	// compute parameters for amount of symbols
	duration_ = (1 / (double)configIn.symbol_rate) * parsedData.size();		// in seconds
	// numTotalDataPts_ = (duration_ / (1.0 / configIn.carrier_freq)) * configIn.sample_rate;
	numTotalDataPts_ = (duration_ / (1.0 / configIn.sample_rate));
	dataPointsPerSymbol_ = (numTotalDataPts_ / parsedData.size());

	// transmission details
	//std::cout << "TRANSMIT: total data pts =" << numTotalDataPts_ << std::endl;
	//std::cout << "TRANSMIT: duration =" << parsedData.size() << std::endl;
	//std::cout << "TRANSMIT: num data pts /sym =" << dataPointsPerSymbol_ << std::endl;

	// validate message 
	if ( validateMessageData(parsedData, configIn) ) {
		if (strcmp(configIn.mod_type.c_str(), "ask") == 0) {
			transmitASK(carrierOut, configIn, parsedData);
		} 
		else if (strcmp(configIn.mod_type.c_str(), "fsk") == 0) {
			transmitFSK(carrierOut, configIn, parsedData);
		}
		else if (strcmp(configIn.mod_type.c_str(), "psk") == 0) {
			transmitPSK(carrierOut, configIn, parsedData);
		}
	}
	else {
		std::cout << "Invalid message data provided. Will not transmit" << std::endl;
	}

	return;
}


void TransmitSystem::parseMessage(std::vector<int> &messageOut, inData &configIn, std::vector<int> &messageIn) {
	// upper limit of alphabet for n_ary scheme
	int maxAlpha = configIn.n_ary;
	std::cout << "parsing binary data into appropriate symbols..." << std::endl; 

	int i = 0;
	while (i < messageIn.size()) {
		int encodedSymbol = 0;
		if (configIn.n_ary == 2) {
			for (int j = 0; j < messageIn.size(); j++) {
				messageOut.push_back(messageIn.at(j));
			}
			break;
		}
		else if (configIn.n_ary == 4) {
			try {
				encodedSymbol = 2 * messageIn.at(i);
				encodedSymbol += messageIn.at(i + 1);
			}
			catch (std::out_of_range& err) {
				messageOut.push_back(encodedSymbol);
				break;
			}
			i += 2;
		}
		else if (configIn.n_ary == 8) {
			try {
				encodedSymbol = 4 * messageIn.at(i);
				encodedSymbol += 2 * messageIn.at(i + 1);
				encodedSymbol += messageIn.at(i + 2);
			}
			catch (std::out_of_range& err) {
				messageOut.push_back(encodedSymbol);
				break;
			}
			i += 3;
		}
		else if (configIn.n_ary == 16) {
			try {
				encodedSymbol = 8 * messageIn.at(i);
				encodedSymbol += 4 * messageIn.at(i + 1);
				encodedSymbol += 2 * messageIn.at(i + 2);
				encodedSymbol += messageIn.at(i + 3);
			}
			catch (std::out_of_range& err) {
				messageOut.push_back(encodedSymbol);
				break;
			}
			i += 4;
		}
		messageOut.push_back(encodedSymbol);
		// std::cout << "parsed: " << encodedSymbol << std::endl; 
	}
}

bool TransmitSystem::validateMessageData(const std::vector<int>& messageIn, const inData& configIn) {
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

void TransmitSystem::transmitASK(std::vector<Complex> &carrierOut, inData &configIn, std::vector<int> &messageIn) {

	double timeStep = 1 / (double)configIn.sample_rate;

	// prep the carrier data points
	for (int i = 0; i < numTotalDataPts_; i++) {
		carrierOut.push_back(sin(2 * configIn.carrier_freq * (double)i*timeStep));
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

void TransmitSystem::transmitFSK(std::vector<Complex> &carrierOut, inData &configIn, std::vector<int> &messageIn) {
	double freqStep = configIn.freq_dev * 2 / configIn.n_ary; 


	double modulatedFreq = configIn.carrier_freq; 
	double carrierFreq = configIn.carrier_freq; 

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
		double timeStep = 1/(double)configIn.sample_rate; 
		for (int j = 0; j < dataPointsPerSymbol_; j++) {
//			carrierOut.push_back(sin( carrierFreq * M_PI / 180 * j*0.0001 ));
			carrierOut.push_back( sin( (2*M_PI*modulatedFreq) * (double)j* timeStep) );
		}
		// std::cout << "modulated freq = " << modulatedFreq << std::endl; 
	}
}

void TransmitSystem::transmitPSK(std::vector<Complex> &carrierOut, inData &configIn, std::vector<int> &messageIn) {
	double phaseOffset = M_PI / configIn.n_ary;
	double timeStep = 1 / (double)configIn.sample_rate;

	std::cout << "PSK Modulation..." << std::endl; 
	std::cout << "Phase offset per symbol: " << phaseOffset << std::endl;

	for (int i = 0; i < messageIn.size(); i++) {
		if (configIn.n_ary == 2) {

			for (int j = 0; j < dataPointsPerSymbol_; j++) {
				carrierOut.push_back(sin((2 * M_PI *configIn.carrier_freq) * (double)j* timeStep + ((double)messageIn[i]*phaseOffset)));
			}
		}

		if (configIn.n_ary == 4) {
			for (int j = 0; j < dataPointsPerSymbol_; j++) {
				carrierOut.push_back(sin((2 * M_PI*configIn.carrier_freq) * (double)j* timeStep + ((2 * ((double)messageIn[i]+1) - 1) * phaseOffset)));
			}
		}

		if (configIn.n_ary == 8) {
			for (int j = 0; j < dataPointsPerSymbol_; j++) {
				carrierOut.push_back(sin((2 * M_PI*configIn.carrier_freq) * (double)j* timeStep + ((2 * ((double)messageIn[i] + 1) - 1) * phaseOffset)));
			}
		}

		if (configIn.n_ary == 16) {
			for (int j = 0; j < dataPointsPerSymbol_; j++) {
				carrierOut.push_back(sin((2 * M_PI*configIn.carrier_freq) * (double)j* timeStep + ((2 * ((double)messageIn[i] + 1) - 1) * phaseOffset)));
			}
		}
	}

}
