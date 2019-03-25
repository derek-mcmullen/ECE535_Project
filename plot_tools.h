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
#include <vector>

using namespace std;

// plot config is essentially the parameters of a gnuplot file for each plot.
struct plot_config {
	const char* line_type;
	const char* line_color;
	const char* line_width;
	const char* x_axis_title;
	const char* y_axis_title;
	const char* main_title;
	const char* file_to_plot;

	// defining a default constructor with initialized values
	plot_config() :
		line_color("lt -1");
		line_width = 
	{}
};

class sys_plot_config {
private:
	int number_of_plots;
	vector<plot_config> system_plots;
public:
	int getNumberPlots() { return this->number_of_plots; };
	void addPlot(plot_config plot_to_add) { this->system_plots.push_back(plot_to_add); };
	plot_config popPlot() { return this->system_plots.pop_back; };
};

bool plot_file(const char* file_to_plot);
int demo(float pi);


#endif