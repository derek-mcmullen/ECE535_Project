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
	 system("pause");
	return 0;
}