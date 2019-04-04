#ifndef ConfigLoader_h
#define ConfigLoader_h

#include <string>
#include "plot_tools.h"

struct inData {

	// variables that will be set from config file
	int carrier_freq; 
	int sample_rate; 
	int n_ary;
	std::string mod_type; 
	int samples_per_bit;
	
	// collection of plot configuration settings
	vector<plot_config> system_plots;

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
