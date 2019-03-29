#ifndef SystemConstants_h
#define SystemConstants_h

#include <string>

struct inData {

	// variables that will be set from config file
	int carrier_freq;
	int sample_rate;
	int n_ary;
	std::string mod_type;
	int symbol_rate;
	int freq_dev; 


	// defining a default constructor with initialized values
	inData() :
		carrier_freq(1000000),
		sample_rate(1000),
		n_ary(2),
		mod_type("fsk"),
		symbol_rate(10), 
		freq_dev(1000)
	{}
};

#endif // !SystemConstants_h
