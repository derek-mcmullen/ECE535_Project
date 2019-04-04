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

using namespace std; 

void dumpToFile(const char* filename, vector<double> &data); 

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
	transmit( carrierData, configInData, messageToTransmit ); 
	dumpToFile("./data/transmitOut.dat", carrierData); 


	// Simulate the additional interference
	// TODO

	// Simulate the demodulator
	// TODO

	// Plot relevant data
	// demo(M_PI);
	cout << "carrier is " << carrierData.size() << " elements large." << endl; 

	//system("pause"); 


	plot_file(configInData.system_plots);

	system("pause");
}




void dumpToFile(const char* filename, vector<double> &data) {
	ofstream outFile;
	outFile.open(filename);

	for (int i = 0; i < data.size(); i++) {	
		outFile << data[i] << "\n";
	}
	outFile.close(); 
}