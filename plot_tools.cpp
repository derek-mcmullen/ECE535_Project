/* 
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
		plotFile << "reset\n" << "set ylabel 'Amplitude'\n" << "set xlabel 'Time (samples)'\n" << "set title 'Time Domain Modulated Signal'\n";
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

// Creates a gnuplot file, takes in a filename of the data we want to plot.
bool plot_fft(const char* file_to_plot) {
	// Create plot file 
	// TODO: add functionality for plot configuration from config struct
	ofstream plotFile;
	plotFile.open("./plot_scripts/plot.txt");
	if (plotFile.is_open()) {
		plotFile << "reset\n" << "set ylabel 'FFT Value'\n" << "set xlabel 'Frequency Bin'\n" << "set title 'FFT of Modulated Signal'\n";
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
