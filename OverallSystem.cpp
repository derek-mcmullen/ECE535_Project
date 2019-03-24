#define _USE_MATH_DEFINES

#include "ConfigLoader.h"
#include "InterferenceSystem.h"
#include "plot_tools.h"
#include "Sampler.h"
#include "TransmitSystem.h"

#include <iostream>  
#include <fstream>
#include <cmath>   
#include <string>
#include <algorithm>
#include <vector>

#define PI 3.14159265

using namespace std; 

int main()
{
	// Initilaize the config data, message data, and carrier data structures
	inData configInData; 
	vector<int> messageToTransmit = { 1,0,1,0,1,0,1,0 };
	vector<double> carrierData; 
	vector<int> messageReceived;

	// Load the config options from file
	loadSettings( configInData ); 

	// Simulate the modulation 
	transmit(carrierData, configInData, messageToTransmit); 

	// Simulate the additional interference
	// TODO

	// Simulate the demodulator
	// TODO

	// Plot relevant data
	// demo(PI);
	cout << "carrier is " << carrierData.size() << " elements large." << endl; 
	for (int i = 0; i < carrierData.size(); i++) {
		cout << "data points include: " << carrierData[i] << endl; 
	}


	system("pause"); 

}



