#include "ConfigLoader.h"

#include <iostream>
#include <fstream>
#include <math.h>   
#include <string>
#include <algorithm>


void loadSettings( inData &in ) {
	std::ifstream cFile("settings/config.txt");

	if (cFile.is_open())
	{
		std::string line;
		while (getline(cFile, line)) {
			line.erase(std::remove_if(line.begin(), line.end(), isspace),
				line.end());
			if (line[0] == '#' || line.empty())
				continue;
			auto delimiterPos = line.find("=");
			auto name = line.substr(0, delimiterPos);
			auto value = line.substr(delimiterPos + 1);
			// std::cout << name << " " << value << '\n';

			if (strcmp(name.c_str(), "mod_type") == 0)
			{
				in.mod_type = value;
			}
			else if (strcmp(name.c_str(), "carrier_freq") == 0)
			{
				in.carrier_freq = stod(value);    // string to double 
			}
			else if (strcmp(name.c_str(), "sample_rate") == 0)
			{
				in.sample_rate = stod(value);    // string to double 
			}
			else if (strcmp(name.c_str(), "n_ary") == 0)
			{
				in.n_ary = stoi(value);    // string to double 
			}
			/* more else if clauses */
			else /* default: */
			{
				std::cout << "Value for " << name << " was not loaded properly" << std::endl;
			}
		}

	}
	else {
		std::cerr << "Couldn't open config file for reading.\n";
	}
}

