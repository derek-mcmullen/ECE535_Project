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
bool plot_file(vector<plot_config> system_plots) {
	// Create plot file 
	ofstream plotFile;
	plotFile.open("plot_scripts/plot.txt");
	plotFile << "reset\n";		// Always reset the plot configuration
	if (plotFile.is_open()) {
		// Set up each plot we have in the configuration
		for (int i = 0; i < system_plots.size(); i++) {
			plotFile << "set term wxt " << i << "\n";		// Create a new window for each plot

			// Set up each individual plot settings
			plotFile << "set ylabel '" << system_plots.at(i).y_axis_title.c_str() << "'\n";
			plotFile << "set xlabel '" << system_plots.at(i).x_axis_title.c_str() << "'\n";
			plotFile << "set title '" << system_plots.at(i).main_title.c_str() << "'\n";
			plotFile  << "set style line " << i+5 << " lt " << system_plots.at(i).line_color.c_str() \
				<< " lw " << system_plots.at(i).line_width.c_str() << " pt " << system_plots.at(i).line_point.c_str() << "\n";
			plotFile << "plot \"" << system_plots.at(i).file_to_plot.c_str() << "\" with linespoints ls " << i+5 << "\n";
		}
		plotFile.close();
	}
	else {
		// Break if we cant open file
		cout << "Can't open file plot_scripts/plot.txt";
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