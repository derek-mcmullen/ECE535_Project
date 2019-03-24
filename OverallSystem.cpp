#include "ConfigLoader.h"
#include "InterferenceSystem.h"
#include "Sampler.h"

#include <iostream>  
#include <fstream>
#include <math.h>   
#include <string>
#include <algorithm>

#include "plot_tools.h"

#define PI 3.14159265

using namespace std; 

int main()
{
	// config data struct
	inData configInData; 

	loadSettings( configInData ); 

	// testing that files have been parsed appropriately
	cout << configInData.mod_type << endl; 

	//demo(PI);
	system("pause"); 

}



