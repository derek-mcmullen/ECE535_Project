#ifndef ConfigLoader_h
#define ConfigLoader_h

#include <string>

struct inData {

	// variables that will be set from config file
	int carrier_freq; 
	int sample_rate; 
	int n_ary;
	std::string mod_type; 
	int samples_per_bit;
	

	// defining a default constructor with initialized values
	inData() :
		carrier_freq(1000000),
		sample_rate(1000),
		n_ary(2),
		mod_type("fsk"), 
		samples_per_bit( 10 )
	{}

};



// function prototypes
void loadSettings(inData &in); 

#endif // !ConfigLoader_h
