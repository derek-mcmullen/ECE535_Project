#include "ConfigLoader.h"

#include <iostream>
#include <fstream>
#include <math.h>   
#include <string>
#include <algorithm>


void loadSettings( inData &in ) {
	std::ifstream cFile("settings/config.txt");
	int num_of_plots = 0;
	int plot_counter = 0;

	if (cFile.is_open())
	{
		std::string line;
		while (getline(cFile, line)) {
			line.erase(std::remove_if(line.begin(), line.end(), isspace),
				line.end());
			if (line[0] == '#' || line.empty())
				continue;
			auto delimiterPos = line.find("=");
			auto name = line.substr(0, delimiterPos);
			auto value = line.substr(delimiterPos + 1);
			// std::cout << name << " " << value << '\n';

			if (strcmp(name.c_str(), "mod_type") == 0)
			{
				in.mod_type = value;
			}
			else if (strcmp(name.c_str(), "carrier_freq") == 0)
			{
				in.carrier_freq = stoi(value);    // string to integer
			}
			else if (strcmp(name.c_str(), "sample_rate") == 0)
			{
				in.sample_rate = stoi(value);    // string to integer
			}
			else if (strcmp(name.c_str(), "n_ary") == 0)
			{
				in.n_ary = stoi(value);    // string to integer 
			}
			else if (strcmp(name.c_str(), "samples_per_bit") == 0)
			{
				in.samples_per_bit= stoi(value);    // string to integer 
			}
			/* more else if clauses */

			/*
			Add parsing for plots
			************************************************************
			*/
			else if (strcmp(name.c_str(), "number_of_plots") == 0)
			{
				num_of_plots = stoi(value);    // string to integer 

				// Create number of plot structs
				for (int p = 0; p < num_of_plots; p++) {
					plot_config newPlot;
					in.system_plots.push_back(newPlot);
				}
				plot_counter = 0;		// Reset plot counter just in case
				cout << in.system_plots.size();
			}
			else if (strcmp(name.c_str(), "plot_data_fn") == 0)
			{
				in.system_plots.at(plot_counter).file_to_plot = value;    
			}
			else if (strcmp(name.c_str(), "x_axis_title") == 0)
			{
				in.system_plots.at(plot_counter).x_axis_title = value;    
			}
			else if (strcmp(name.c_str(), "y_axis_title") == 0)
			{
				in.system_plots.at(plot_counter).y_axis_title = value;    
			}
			else if (strcmp(name.c_str(), "main_title") == 0)
			{
				in.system_plots.at(plot_counter).main_title = value;
			}
			else if (strcmp(name.c_str(), "line_color") == 0)
			{
				in.system_plots.at(plot_counter).line_color = value;
			}
			else if (strcmp(name.c_str(), "line_width") == 0)
			{
				in.system_plots.at(plot_counter).line_width = value;
			}
			else if (strcmp(name.c_str(), "line_point") == 0)
			{
				in.system_plots.at(plot_counter).line_point = value;
			}
			// Advance to next plot
			else if (strcmp(name.c_str(), "next_plot") == 0)
			{
				plot_counter++;
			}
			//*********************************************************

			else /* default: */
			{
				std::cout << "Value for " << name << " was not loaded properly" << std::endl;
			}
		}

	}
	else {
		std::cerr << "Couldn't open config file for reading.\n";
	}
}

