#ifndef ConfigLoader_h
#define ConfigLoader_h

#include <string>

struct inData {

	// variables that will be set from config file
	double carrier_freq; 
	double sample_rate; 
	double n_ary;
	std::string mod_type;
};


// function prototypes
void loadSettings(inData &in); 

#endif // !ConfigLoader_h
