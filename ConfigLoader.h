#ifndef ConfigLoader_h
#define ConfigLoader_h

#include <string>

struct inData {

	// variables that will be set from config file
	double carrier_freq; 
	double sample_rate; 
	double n_ary;
	std::string mod_type;

	// defining a default constructor with initialized values
	inData() :
		carrier_freq(1000000),
		sample_rate(1000),
		n_ary(2),
		mod_type("fsk")
	{}

};



// function prototypes
void loadSettings(inData &in); 

#endif // !ConfigLoader_h
