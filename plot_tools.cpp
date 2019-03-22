#include "plot_tools.h"

/* Simple demo of gnuplot
   essentially use system to call a batch file that calls
   local version of gnuplot and passes in a 'gnuplot file' which has
   all the gnuplot commands to plot. 

   All plotting configuration should take place within the plot.txt file.
*/
int demo(float pi){
	float x = 0;
	float i = 0;
	cout << "Opening file...\n" << endl;
	ofstream signalFile;
	signalFile.open("signal.txt");
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
	cout << "Plotting...";
	system("plotter.bat");
	system("pause");
	return 0;
}