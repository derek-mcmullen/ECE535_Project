/* 
Author: Jake Reed
Date: 3/21/19

Simple demo of gnuplot
   essentially use system to call a batch file that calls
   local version of gnuplot and passes in a 'gnuplot file' which has
   all the gnuplot commands to plot. 

   All plotting configuration should take place within the plot.txt file.
*/

#include "plot_tools.h"
#include <random>

#define DEBUG false 


// Creates a gnuplot file, takes in a filename of the data we want to plot.
bool plot_file(const char* file_to_plot) {
	// Create plot file 
	// TODO: add functionality for plot configuration from config struct
	ofstream plotFile;
	plotFile.open("./plot_scripts/plot.txt");
	if (plotFile.is_open()) {
		plotFile << "reset\n" << "set ylabel 'Sample Value'\n" << "set xlabel 'Amount of Gayness Derek is'\n" << "set title 'Summary of the Year 2018'\n";
		plotFile << "plot \"./data/" << file_to_plot << "\" with lines ls 1";
		plotFile.close();
	}
	else {
		// Break if we cant open file
		cout << "Can't open file " << file_to_plot;
		return false;
	}

	// Call batch file to plot.
	system("plot_scripts\\plotter.bat");

	return true;
}


// Basic plotting functionality
int demo(float pi){

	// Create some iterators
	float x = 0;
	float i = 0;
	float amplitude = 0;
	float noise;

	cout << "Please Enter Noise Amplitude Between 0-1 ...\n";
	cin >> amplitude;
	cout << "\n\nThats what i thought bitch...\n";
	cout << "Allow me to show you how gay you were in 2018...\n\n\n";
	// Open file for storing 'signal'
	cout << "Opening file..." << endl;
	ofstream signalFile;
	signalFile.open("./data/signal.txt");

	// Create a sine wave with .1 units between sample, for 3 cycles
	for (int z = 0; z < 30; z++){
		while (i < 2 * pi){
			noise = ((double)rand() / (RAND_MAX)) + 1;
			x = sin(i) + (amplitude*noise);
			signalFile << x << "\n";
			i = i + .1;
			if (DEBUG){
				cout << x << "\n";
				cout << "i=" << i << "\n";
			}
		}
		i = x = 0;
	}
	signalFile.close();
	cout << "Closing File...\n";

	// Call batch file that handles gnuplot
	cout << "Plotting...\n";
	system("plot_scripts\\plotter.bat");
	
	if (DEBUG) {
		system("pause");
	}
	return 0;
}