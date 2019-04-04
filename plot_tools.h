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
	string line_color;
	string line_width;
	string line_point;
	string x_axis_title;
	string y_axis_title;
	string main_title;
	string file_to_plot;

	// defining a default constructor with initialized values
	plot_config() :
		line_color("-1"),
		line_width("1"),
		line_point("0"),
		x_axis_title("X-Axis"),
		y_axis_title("Y-Axis"),
		main_title("Title"),
		file_to_plot("")
	{}

};

class sys_plot_config {
private:
	int number_of_plots;
	vector<plot_config> system_plots;
public:
	int getNumberPlots() { return this->number_of_plots; };
	void addPlot(plot_config plot_to_add) { this->system_plots.push_back(plot_to_add); };
	plot_config getPlot(int plot_num) {  return this->system_plots.at(plot_num);  };
};

bool plot_file(vector<plot_config> system_plots);
int demo(float pi);


#endif