#include "ReceiveSystem.h"

#include "MathStuff.h"

#include <fstream>
#include <iostream>
#include <vector>


ReceiveSystem::ReceiveSystem() :
	duration_(10),
	numTotalDataPts_(100),
	dataPointsPerSymbol_(10),
	numSymbols_(0), 
	askThreshold_(0.636411)
{};


void ReceiveSystem::receive(std::vector<int> &messageOut, inData &configIn, std::vector<Complex> &rawDataIn) {
	// receiver constants based on received samples
	numTotalDataPts_ = rawDataIn.size();
	dataPointsPerSymbol_ = configIn.sample_rate / configIn.symbol_rate;
	numSymbols_ = numTotalDataPts_ / dataPointsPerSymbol_; 

	// Receiver details
	std::cout << "Sampling has resulted in "<< numTotalDataPts_ << " total data pts" << std::endl;
	std::cout << "For configured symbol rate, this is "<< dataPointsPerSymbol_ << " data points per symbol" << std::endl;

	// initialize the output message
	std::vector<int> symbols;

	// choose the appropraite receiver type
	if (strcmp(configIn.mod_type.c_str(), "ask") == 0) {
		receiveASK(symbols, configIn, rawDataIn);
	}
	else if (strcmp(configIn.mod_type.c_str(), "fsk") == 0) {
		receiveFSK(symbols, configIn, rawDataIn);
	}
	else if (strcmp(configIn.mod_type.c_str(), "psk") == 0) {
		receivePSK(symbols, configIn, rawDataIn);
	}

	unparseMessage(messageOut, configIn, symbols); 
}

void ReceiveSystem::receiveASK(std::vector<int> &messageOut, inData &configIn, std::vector<Complex> &rawDataIn) {
	// implementing a basic LPF through a moving average filter to act as an envelope detector
	for (int i = 0; i < numSymbols_; i++) {
		double sum = 0; 
		for (int j = 0; j < dataPointsPerSymbol_; j++) {
			sum += abs(rawDataIn.at(i*dataPointsPerSymbol_ + j)); 
		}
		double undeterminedSymbol = sum / dataPointsPerSymbol_; 

		// populated the symbols after determination
		messageOut.push_back(makeASKSymbolDetermination(undeterminedSymbol) );
	}
}

void ReceiveSystem::receiveFSK(std::vector<int> &messageOut, inData &configIn, std::vector<Complex> &rawDataIn) {
	
	std::vector<double> symbolFFTs; 
	int fftSize = 0;
	for (int i = 0; i < numSymbols_; i++) {
		
		std::vector<Complex> buffer; 
		std::vector<double> undeterminedSymbols; 

		// pad zeros up to a power of 2 samples
		fftSize = 1; 
		while (fftSize < dataPointsPerSymbol_) {
			fftSize *= 2; 
		}
		int zerosNeeded = fftSize - dataPointsPerSymbol_;

		for (int j = 0; j < dataPointsPerSymbol_; j++) {
			buffer.push_back( rawDataIn.at(i*dataPointsPerSymbol_ + j) ); 
		}
		for (int j = 0; j < zerosNeeded; j++) {
			buffer.push_back(0); 
		}
		Complex* samples = &buffer[0];
		CArray data(samples, fftSize);

		// std::cout << "zeros added: " << zerosNeeded << "  total size: " << data.size() << std::endl;

		// forward fft to find the freq
		fft(data);
		int maxIndex = 0;
		double max = 0;

		// window the samples for a single symbol
		for (int k = 0; k < data.size(); ++k)
		{
			double check = abs(data[k]);
			symbolFFTs.push_back(check);
			if (check > max) {
				max = check;
				maxIndex = k;
			}
		}
		// check if the fft bin wrapped around
		if (maxIndex > fftSize / 2) {
			maxIndex = abs(maxIndex - fftSize); 
		}

		// determine the freq of the max fft bin
		double freq = (maxIndex * configIn.sample_rate) / (fftSize);

		// std::cout << "maxIndex: " << maxIndex << std::endl;
		// std::cout << "samp rate: " << configIn.sample_rate << std::endl;
		// std::cout << "fft size: " << fftSize << std::endl;


		messageOut.push_back(makeFSKSymbolDetermination(freq, configIn) ); 
	}

	//std::ofstream outfile;
	//outfile.open("data/transmitout.dat");

	//for (int i = 0; i < fftsize; i++) {
	//	outfile << symbolffts[i] << "\n";
	//}
	//outfile.close();


}

void ReceiveSystem::receivePSK(std::vector<int> &messageOut, inData &configIn, std::vector<Complex> &rawDataIn) {
	double timeStep = 1 / (double)configIn.sample_rate;

	for (int i = 0; i < numSymbols_; i++) {
		// initialize the constellation point error rates
		double deltaVals[] = { 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0 }; 
		for (int j = 0; j < 16; j++) {
			deltaVals[j] = 100000000;
		}
		for (int j = 0; j < configIn.n_ary; j++) {
			deltaVals[j] = 0;
		}
		// implement decision regions symmetrically by identifying which constellation points are minimum error
		for (int j = 0; j < dataPointsPerSymbol_; j++) {
			if (configIn.n_ary == 2) {
				deltaVals[0] += abs(sin(2 * M_PI*configIn.carrier_freq*(double)j*timeStep) - rawDataIn[i*dataPointsPerSymbol_ + j]);
				deltaVals[1] += abs(sin(2 * M_PI*configIn.carrier_freq*(double)j*timeStep+M_PI/2) - rawDataIn[i*dataPointsPerSymbol_ + j]);
			}

			if (configIn.n_ary == 4) {
				deltaVals[0] += abs(sin(2 * M_PI*configIn.carrier_freq*(double)j*timeStep + M_PI/4) - rawDataIn[i*dataPointsPerSymbol_ + j]);
				deltaVals[1] += abs(sin(2 * M_PI*configIn.carrier_freq*(double)j*timeStep + 3 * M_PI / 4) - rawDataIn[i*dataPointsPerSymbol_ + j]);
				deltaVals[2] += abs(sin(2 * M_PI*configIn.carrier_freq*(double)j*timeStep + 5 * M_PI / 4) - rawDataIn[i*dataPointsPerSymbol_ + j]);
				deltaVals[3] += abs(sin(2 * M_PI*configIn.carrier_freq*(double)j*timeStep + 7 * M_PI / 4) - rawDataIn[i*dataPointsPerSymbol_ + j]);
			}
			if (configIn.n_ary == 8) {
				deltaVals[0] += abs(sin(2 * M_PI*configIn.carrier_freq*(double)j*timeStep + M_PI / 8) - rawDataIn[i*dataPointsPerSymbol_ + j]);
				deltaVals[1] += abs(sin(2 * M_PI*configIn.carrier_freq*(double)j*timeStep + 3 * M_PI / 8) - rawDataIn[i*dataPointsPerSymbol_ + j]);
				deltaVals[2] += abs(sin(2 * M_PI*configIn.carrier_freq*(double)j*timeStep + 5 * M_PI / 8) - rawDataIn[i*dataPointsPerSymbol_ + j]);
				deltaVals[3] += abs(sin(2 * M_PI*configIn.carrier_freq*(double)j*timeStep + 7 * M_PI / 8) - rawDataIn[i*dataPointsPerSymbol_ + j]);
				deltaVals[4] += abs(sin(2 * M_PI*configIn.carrier_freq*(double)j*timeStep + 9 * M_PI / 8) - rawDataIn[i*dataPointsPerSymbol_ + j]);
				deltaVals[5] += abs(sin(2 * M_PI*configIn.carrier_freq*(double)j*timeStep + 11 * M_PI / 8) - rawDataIn[i*dataPointsPerSymbol_ + j]);
				deltaVals[6] += abs(sin(2 * M_PI*configIn.carrier_freq*(double)j*timeStep + 13 * M_PI / 8) - rawDataIn[i*dataPointsPerSymbol_ + j]);
				deltaVals[7] += abs(sin(2 * M_PI*configIn.carrier_freq*(double)j*timeStep + 15 * M_PI / 8) - rawDataIn[i*dataPointsPerSymbol_ + j]);
			}

			if (configIn.n_ary == 16) {
				deltaVals[0] += abs(sin(2 * M_PI*configIn.carrier_freq*(double)j*timeStep + M_PI / 16) - rawDataIn[i*dataPointsPerSymbol_ + j]);
				deltaVals[1] += abs(sin(2 * M_PI*configIn.carrier_freq*(double)j*timeStep + 3 * M_PI / 16) - rawDataIn[i*dataPointsPerSymbol_ + j]);
				deltaVals[2] += abs(sin(2 * M_PI*configIn.carrier_freq*(double)j*timeStep + 5 * M_PI / 16) - rawDataIn[i*dataPointsPerSymbol_ + j]);
				deltaVals[3] += abs(sin(2 * M_PI*configIn.carrier_freq*(double)j*timeStep + 7 * M_PI / 16) - rawDataIn[i*dataPointsPerSymbol_ + j]);
				deltaVals[4] += abs(sin(2 * M_PI*configIn.carrier_freq*(double)j*timeStep + 9 * M_PI / 16) - rawDataIn[i*dataPointsPerSymbol_ + j]);
				deltaVals[5] += abs(sin(2 * M_PI*configIn.carrier_freq*(double)j*timeStep + 11 * M_PI / 16) - rawDataIn[i*dataPointsPerSymbol_ + j]);
				deltaVals[6] += abs(sin(2 * M_PI*configIn.carrier_freq*(double)j*timeStep + 13 * M_PI / 16) - rawDataIn[i*dataPointsPerSymbol_ + j]);
				deltaVals[7] += abs(sin(2 * M_PI*configIn.carrier_freq*(double)j*timeStep + 15 * M_PI / 16) - rawDataIn[i*dataPointsPerSymbol_ + j]);
				deltaVals[8] += abs(sin(2 * M_PI*configIn.carrier_freq*(double)j*timeStep + 17 * M_PI / 16) - rawDataIn[i*dataPointsPerSymbol_ + j]);
				deltaVals[9] += abs(sin(2 * M_PI*configIn.carrier_freq*(double)j*timeStep + 19 * M_PI / 16) - rawDataIn[i*dataPointsPerSymbol_ + j]);
				deltaVals[10] += abs(sin(2 * M_PI*configIn.carrier_freq*(double)j*timeStep + 21 * M_PI / 16) - rawDataIn[i*dataPointsPerSymbol_ + j]);
				deltaVals[11] += abs(sin(2 * M_PI*configIn.carrier_freq*(double)j*timeStep + 23 * M_PI / 16) - rawDataIn[i*dataPointsPerSymbol_ + j]);
				deltaVals[12] += abs(sin(2 * M_PI*configIn.carrier_freq*(double)j*timeStep + 25 * M_PI / 16) - rawDataIn[i*dataPointsPerSymbol_ + j]);
				deltaVals[13] += abs(sin(2 * M_PI*configIn.carrier_freq*(double)j*timeStep + 27 * M_PI / 16) - rawDataIn[i*dataPointsPerSymbol_ + j]);
				deltaVals[14] += abs(sin(2 * M_PI*configIn.carrier_freq*(double)j*timeStep + 29 * M_PI / 16) - rawDataIn[i*dataPointsPerSymbol_ + j]);
				deltaVals[15] += abs(sin(2 * M_PI*configIn.carrier_freq*(double)j*timeStep + 31 * M_PI / 16) - rawDataIn[i*dataPointsPerSymbol_ + j]);
			}

		}
		double minOffset = 100000000; 
		int minIndex = -1; 
		for (int j = 0; j < 16; j++) {
			if (deltaVals[j] < minOffset) {
				minOffset = deltaVals[j]; 
				minIndex = j; 
			}
		}
		messageOut.push_back(minIndex); 
		//std::cout << "min index at: " << minIndex << std::endl;

		//std::cout << "[0] =  " << deltaVals[0] << std::endl;
		//std::cout << "[1] =  " << deltaVals[1] << std::endl;
		//std::cout << "[2] =  " << deltaVals[2] << std::endl;
		//std::cout << "[3] =  " << deltaVals[3] << std::endl;
		//std::cout << "[4] =  " << deltaVals[4] << std::endl;
		//std::cout << "[5] =  " << deltaVals[5] << std::endl;
		//std::cout << "[6] =  " << deltaVals[6] << std::endl;
		//std::cout << "[7] =  " << deltaVals[7] << std::endl;
	}
}

void  ReceiveSystem::unparseMessage(std::vector<int> &messageOut, inData &configIn, std::vector<int> &symbols) {
	int index = 0;
	switch (configIn.n_ary) {
	case 2: 
		for (int i = 0; i < symbols.size(); i++) {
			messageOut.push_back(symbols.at(i)); 
		}
		break; 
	case 4: 
		while (index < symbols.size()) {
			if (symbols.at(index) == 0) {
				messageOut.push_back(0); messageOut.push_back(0);
			}
			if (symbols.at(index) == 1) {
				messageOut.push_back(0); messageOut.push_back(1);
			}
			if (symbols.at(index) == 2) {
				messageOut.push_back(1); messageOut.push_back(0);
			}
			if (symbols.at(index) == 3) {
				messageOut.push_back(1); messageOut.push_back(1);
			}
			index++; 
		}
		break;
	case 8: 
		while (index < symbols.size()) {
			if (symbols.at(index) == 0) {
				messageOut.push_back(0); messageOut.push_back(0); messageOut.push_back(0);
			}
			if (symbols.at(index) == 1) {
				messageOut.push_back(0); messageOut.push_back(0); messageOut.push_back(1);
			}
			if (symbols.at(index) == 2) {
				messageOut.push_back(0); messageOut.push_back(1); messageOut.push_back(0);
			}
			if (symbols.at(index) == 3) {
				messageOut.push_back(0); messageOut.push_back(1); messageOut.push_back(1);
			}
			if (symbols.at(index) == 4) {
				messageOut.push_back(1); messageOut.push_back(0); messageOut.push_back(0);
			}
			if (symbols.at(index) == 5) {
				messageOut.push_back(1); messageOut.push_back(0); messageOut.push_back(1);
			}
			if (symbols.at(index) == 6) {
				messageOut.push_back(1); messageOut.push_back(1); messageOut.push_back(0);
			}
			if (symbols.at(index) == 7) {
				messageOut.push_back(1); messageOut.push_back(1); messageOut.push_back(1);
			}
			index++;
		}
		break;
	case 16: 
		while (index < symbols.size()) {
			if (symbols.at(index) == 0) {
				messageOut.push_back(0); messageOut.push_back(0); messageOut.push_back(0); messageOut.push_back(0);
			}
			if (symbols.at(index) == 1) {
				messageOut.push_back(0); messageOut.push_back(0); messageOut.push_back(0); messageOut.push_back(1);
			}
			if (symbols.at(index) == 2) {
				messageOut.push_back(0); messageOut.push_back(0); messageOut.push_back(1); messageOut.push_back(0);
			}
			if (symbols.at(index) == 3) {
				messageOut.push_back(0); messageOut.push_back(0); messageOut.push_back(1); messageOut.push_back(1);
			}
			if (symbols.at(index) == 4) {
				messageOut.push_back(0); messageOut.push_back(1); messageOut.push_back(0); messageOut.push_back(0);
			}
			if (symbols.at(index) == 5) {
				messageOut.push_back(0); messageOut.push_back(1); messageOut.push_back(0); messageOut.push_back(1);
			}
			if (symbols.at(index) == 6) {
				messageOut.push_back(0); messageOut.push_back(1); messageOut.push_back(1); messageOut.push_back(0);
			}
			if (symbols.at(index) == 7) {
				messageOut.push_back(0); messageOut.push_back(1); messageOut.push_back(1); messageOut.push_back(1);
			}
			if (symbols.at(index) == 8) {
				messageOut.push_back(1); messageOut.push_back(0); messageOut.push_back(0); messageOut.push_back(0);
			}
			if (symbols.at(index) == 9) {
				messageOut.push_back(1); messageOut.push_back(0); messageOut.push_back(0); messageOut.push_back(1);
			}
			if (symbols.at(index) == 10) {
				messageOut.push_back(1); messageOut.push_back(0); messageOut.push_back(1); messageOut.push_back(0);
			}
			if (symbols.at(index) == 11) {
				messageOut.push_back(1); messageOut.push_back(0); messageOut.push_back(1); messageOut.push_back(1);
			}
			if (symbols.at(index) == 12) {
				messageOut.push_back(1); messageOut.push_back(1); messageOut.push_back(0); messageOut.push_back(0);
			}
			if (symbols.at(index) == 13) {
				messageOut.push_back(1); messageOut.push_back(1); messageOut.push_back(0); messageOut.push_back(1);
			}
			if (symbols.at(index) == 14) {
				messageOut.push_back(1); messageOut.push_back(1); messageOut.push_back(1); messageOut.push_back(0);
			}
			if (symbols.at(index) == 15) {
				messageOut.push_back(1); messageOut.push_back(1); messageOut.push_back(1); messageOut.push_back(1);
			}
			if (symbols.at(index) == -1) {
				messageOut.push_back(0); messageOut.push_back(0); messageOut.push_back(0); messageOut.push_back(0);
			}
			index++;
		}
		break;
	default: 
		std::cerr << "Unsuported n-ary level, supported levels include 2,4,8, or 16" << std::endl; 

	}
}

int ReceiveSystem::makeASKSymbolDetermination(double &val) {
	if (val < 0.5*askThreshold_) {
		return 0;
	}
	if (val < 1.5*askThreshold_) {
		return 1;
	}
	if (val < 2.5*askThreshold_) {
		return 2;
	}
	if (val < 3.5*askThreshold_) {
		return 3;
	}
	if (val < 4.5*askThreshold_) {
		return 4;
	}
	if (val < 5.5*askThreshold_) {
		return 5;
	}
	if (val < 6.5*askThreshold_) {
		return 6;
	}
	if (val < 7.5*askThreshold_) {
		return 7;
	}
	if (val < 8.5*askThreshold_) {
		return 8;
	}
	if (val < 9.5*askThreshold_) {
		return 9;
	}
	if (val < 10.5*askThreshold_) {
		return 10;
	}
	if (val < 11.5*askThreshold_) {
		return 11;
	}
	if (val < 12.5*askThreshold_) {
		return 12;
	}
	if (val < 13.5*askThreshold_) {
		return 13;
	}
	if (val < 14.5*askThreshold_) {
		return 14;
	}
	if (val < 15.5*askThreshold_) {
		return 15;
	}
	return -1; //error case
}

int ReceiveSystem::makeFSKSymbolDetermination(double &val, inData &configIn) {
	double freqStep = configIn.freq_dev * 2 / configIn.n_ary;

	double modulatedFreq = configIn.carrier_freq;
	double intermediateFreq = modulatedFreq - val; 

	if (intermediateFreq > 0) {
		if (abs(intermediateFreq) < 1.5*freqStep) {
			return 0;
		}
		if (abs(intermediateFreq) < 2.5*freqStep) {
			return 2;
		}
		if (abs(intermediateFreq) < 3.5*freqStep) {
			return 4;
		}
		if (abs(intermediateFreq) < 4.5*freqStep) {
			return 6;
		}
		if (abs(intermediateFreq) < 5.5*freqStep) {
			return 8;
		}
		if (abs(intermediateFreq) < 6.5*freqStep) {
			return 10;
		}
		if (abs(intermediateFreq) < 7.5*freqStep) {
			return 12;
		}
		if (abs(intermediateFreq) < 8.5*freqStep) {
			return 14;
		}
	}
	if (intermediateFreq < 0) {
		if (abs(intermediateFreq) < 1.5*freqStep) {
			return 1;
		}
		if (abs(intermediateFreq) < 2.5*freqStep) {
			return 3;
		}
		if (abs(intermediateFreq) < 3.5*freqStep) {
			return 5;
		}
		if (abs(intermediateFreq) < 4.5*freqStep) {
			return 7;
		}
		if (abs(intermediateFreq) < 5.5*freqStep) {
			return 9;
		}
		if (abs(intermediateFreq) < 6.5*freqStep) {
			return 11;
		}
		if (abs(intermediateFreq) < 7.5*freqStep) {
			return 13;
		}
		if (abs(intermediateFreq) < 8.5*freqStep) {
			return 15;
		}
	}
	std::cout << "FSK symbol not parsed: " << intermediateFreq << std::endl; 
	return -1;  //error case
}