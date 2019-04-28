#include "InterferenceSystem.h"

#include <ctime>
#include <iostream>
#include <string>
#include <random>

InterferenceSystem::InterferenceSystem() :
	duration_(10),
	numTotalDataPts_(100),
	dataPointsPerSymbol_(10),
	numSymbols_(0)
{};


void InterferenceSystem::AWGN(std::vector<Complex> &dataOut, inData &configIn, std::vector<Complex> &dataIn) {

	// set a random seed based on current system time
	std::default_random_engine generator;
	generator.seed(time(NULL)); 
	std::normal_distribution<double> distribution(0.0,NOISE_VARIANCE);

	// apply the noise to each piece of transmit data
	for (int i = 0; i < dataIn.size(); i++) {
		dataOut.push_back( dataIn[i] + distribution(generator) ); 
	}
	std::cout << "1: " << distribution(generator) << std::endl;
	std::cout << "2: " << distribution(generator) << std::endl;

	
	std::cout << "AWGN with a variance of " << NOISE_VARIANCE << " has been added to the signal" << std::endl; 
}