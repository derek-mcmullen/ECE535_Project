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

int demo(float pi){

	// Create some iterators
	float x = 0;
	float i = 0;

	// Open file for storing 'signal'
	cout << "Opening file...\n" << endl;
	ofstream signalFile;
	signalFile.open("signal.txt");

	// Create a sine wave with .1 units between sample, for 3 cycles
	for (int z = 0; z < 3; z++){
		while (i < 2 * pi){
			x = sin(i);
			signalFile << x << "\n";
			cout << x << "\n";
			i = i + .1;
			cout << "i=" << i << "\n";
		}
		i = x = 0;
	}
	signalFile.close();
	cout << "Closing File...";

	// Call batch file that handles gnuplot
	cout << "Plotting...";
	system("plotter.bat");
	system("pause");
	return 0;
}