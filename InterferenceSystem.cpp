#include "InterferenceSystem.h"
#include "SystemConstants.h"

#include <ctime>
#include <iostream>
#include <string>
#include <random>

#define _USE_MATH_DEFINES

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

void InterferenceSystem::fadeIt(std::vector<Complex> &dataOut, inData &configIn, std::vector<Complex> &dataIn) {
	std::cout << "Applying multipath fading with a radian delay of " << configIn.fade << std::endl; 

	std::vector<Complex> delayData; 

	double interp = configIn.fade / (2* 3.14159) ; 
	double temp = 0;

	for (int i = 0; i < dataIn.size()-1; i++) {
		temp = (1-interp) * (dataIn.at(i).real()) - interp*(dataIn.at(i + 1).real()); 
		delayData.push_back(temp); 
	}

	for (int i = 0; i < dataIn.size() - 1; i++) {
		dataOut.push_back(dataIn.at(i) + delayData.at(i)); 
	} 

}