#ifndef SystemConstants_h
#define SystemConstants_h

#include <string>
#include <complex>
#include <iostream>
#include <valarray>

typedef std::complex<double> Complex;
typedef std::valarray<Complex> CArray;

#define NOISE_VARIANCE 0.1

struct inData {

	// variables that will be set from config file
	int carrier_freq;
	int sample_rate;
	int n_ary;
	std::string mod_type;
	int symbol_rate;
	int freq_dev; 
	double noise_variance; 
	double fade; 


	// defining a default constructor with initialized values
	inData() :
		carrier_freq(1000000),
		sample_rate(1000),
		n_ary(2),
		mod_type("fsk"),
		symbol_rate(10), 
		freq_dev(1000), 
		noise_variance(0.01), 
		fade(0)
	{}
};

#endif // !SystemConstants_h
