#define _USE_MATH_DEFINES

#include "ConfigLoader.h"
#include "InterferenceSystem.h"
#include "MathStuff.h"
#include "plot_tools.h"
#include "ReceiveSystem.h"
#include "Sampler.h"
#include "SystemConstants.h"
#include "TransmitSystem.h"

#include <iostream>  
#include <fstream>
#include <cmath>   
#include <string>
#include <algorithm>
#include <vector>
#include <complex>
#include <valarray>

using namespace std; 

void dumpToFile(const char* filename, vector<Complex> &data);
void dumpToFile(const char* filename, vector<int> &data);

int main()
{
	// Initilaize the config data, message data, and carrier data structures
	inData configInData; 
	//  vector<int> messageToTransmit = { 0,1,0,1,0,1,0,1,0,1 };							// 2-ary example
	// vector<int> messageToTransmit = { 0,0, 0,1, 1,0, 1,1, 0,0, 0,1, 1,0, 1,1 };				// 4-ary example
	 vector<int> messageToTransmit = { 0,0,0, 0,0,1, 0,1,0, 0,1,1, 1,0,0, 1,0,1, 1,1,0 ,1,1,1 };		// 8-ary
	// vector<int> messageToTransmit = { 0,0,0,1, 0,0,1,0, 0,0,1,1, 0,1,0,0, 0,1,0,1, 0,1,1,0, 0,1,1,1, 1,0,0,0, 1,0,0,1, 1,0,1,0, 1,0,1,1, 1,1,0,0, 1,1,0,1, 1,1,1,0, 1,1,1,1 };

	vector<Complex> carrierData; 
	vector<Complex> noisyCarrier; 
	vector<Complex> fadedCarrier; 
	vector<int> messageReceived;


	// Load the config options from file
	loadSettings(configInData);

	// Print the message to be sent (set above)
	std::cout << "Prepared Message Details" << std::endl; 
	std::cout << "----------------------------" << endl; 
	std::cout << "bits to transmit: {"; 
	for (int i = 0; i < messageToTransmit.size(); i++) {
		std::cout << messageToTransmit.at(i);
		if (i != messageToTransmit.size() - 1) {
			std::cout << ","; 
		}
	}
	std::cout << "}" << std::endl; 
	std::cout << "carrier freq:  " << configInData.carrier_freq << std::endl;
	std::cout << "sampling freq: " << configInData.sample_rate << std::endl;
	std::cout << "n-ary level:   " << configInData.n_ary << std::endl;
	std::cout << "symbol rate:   " << configInData.symbol_rate << std::endl; 

	// Transmission details
	std::cout << std::endl;
	std::cout << "Transmission details" << std::endl;
	std::cout << "----------------------------" << endl;

	// Simulate the modulation 
	TransmitSystem TS; 
	TS.transmit( carrierData, configInData, messageToTransmit ); 
	dumpToFile("./data/transmitOut.dat", carrierData); 


	// Simulate any additional interference / noise
	InterferenceSystem IS;
	IS.AWGN(noisyCarrier, configInData, carrierData);
	IS.fadeIt(fadedCarrier, configInData, noisyCarrier); 
	dumpToFile("./data/noisyOut.dat", noisyCarrier);
	dumpToFile("./data/fadedOut.dat", fadedCarrier);


	// Simulate the demodulator
	std::cout << std::endl;
	std::cout << "Received message details" << std::endl;
	std::cout << "----------------------------" << endl;
	ReceiveSystem RS; 
	RS.receive(messageReceived, configInData, noisyCarrier);

	// Print the demodulated output
	std::cout << "demodulate output data: {";
	for (int i = 0; i < messageReceived.size(); i++) {
		std::cout << messageReceived.at(i);
		if (i != messageReceived.size() - 1) {
			std::cout << ",";
		}
	}
	std::cout << "}" << std::endl;
	dumpToFile("./data/messageOut.dat", messageReceived);

	// Compute number or error bits
	int errorCount = 0; 
	for (int i = 0; i < messageReceived.size(); i++) {
		if (messageReceived[i] != messageToTransmit[i]) {
			errorCount++; 
		}
	}
	std::cout << "There were " << errorCount << " bits received in error!" << std::endl; 

	if (errorCount == 0) {
		std::cout << "Perfect tranmission!" << std::endl; 
		std::cout << "        _______________                       | *\\_/*|________  " << std::endl; 
		std::cout << "       |  ___________  |     .-.     .-.      ||_/-\\_|______  | " << std::endl;
		std::cout << "       | |           | |    .****. .****.     | |           | | " << std::endl;
		std::cout << "       | |   0   0   | |    .*****.*****.     | |   0   0   | | " << std::endl;
		std::cout << "       | |     -     | |     .*********.      | |     -     | | " << std::endl;
		std::cout << "       | |   \\___/   | |      .*******.       | |   \\___/   | | " << std::endl;
		std::cout << "       | |___     ___| |       .*****.        | |___________| | " << std::endl;
		std::cout << "       |_____|\\_/|_____|        .***.         |_______________| " << std::endl;
		std::cout << "         _|__|/ \\|_|_.............*.............._|________|_ " << std::endl;
		std::cout << "        / ********** \\                          / ********** \\ " << std::endl;
		std::cout << "      /  ************  \\                      /  ************  \\ " << std::endl;
		std::cout << "     --------------------                    -------------------- " << std::endl;
	}
	else {
		std::cout << "        _______________    " << std::endl;
		std::cout << "       |  ___________  |  " << std::endl;
		std::cout << "       | |           | |  " << std::endl;
		std::cout << "       | |   0   0   | |   " << std::endl;
		std::cout << "       | |     -     | |  " << std::endl;
		std::cout << "       | |    ___    | | " << std::endl;
		std::cout << "       | |___/   \\___| |  " << std::endl;
		std::cout << "       |_____|\\_/|_____| " << std::endl;
		std::cout << "         _|__|/ \\|_|_.............XXX........." << std::endl;
		std::cout << "        / ********** \\    " << std::endl;
		std::cout << "      /  ************  \\   " << std::endl;
		std::cout << "     --------------------   " << std::endl;
	}


	// Plot relevant data
	plot_file("transmitOut.dat"); 
	plot_file("noisyOut.dat");
	plot_file("fadedOut.dat"); 
	if (strcmp(configInData.mod_type.c_str(), "fsk") == 0) {
		plot_fft("fftOut.dat");
	} 
	plot_file("messageOut.dat"); 
	system("pause"); 

}


void dumpToFile(const char* filename, vector<Complex> &data) {
	ofstream outFile;
	outFile.open(filename);

	for (int i = 0; i < data.size(); i++) {
		outFile << data[i].real() << "\n";
	}
	outFile.close();
}


void dumpToFile(const char* filename, vector<int> &data) {
	ofstream outFile;
	outFile.open(filename);

	for (int i = 0; i < data.size(); i++) {
		outFile << data[i] << "\n";
	}
	outFile.close();
}

