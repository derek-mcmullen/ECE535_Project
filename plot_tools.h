/*
Author: Jake Reed
Date: 3/21/19

Simple demo of gnuplot
essentially use system to call a batch file that calls
local version of gnuplot and passes in a 'gnuplot file' which has
all the gnuplot commands to plot.

All plotting configuration should take place within the plot.txt file.
*/



#ifndef plot_tools_h
#define plot_tools_h

#include <iostream>  
#include <fstream>
#include <math.h>  

using namespace std;

bool plot_file(const char* file_to_plot);
int demo(float pi);


#endif