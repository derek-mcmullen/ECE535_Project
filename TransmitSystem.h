#ifndef TransmitSystem_h
#define TransmitSystem_h

#define _USE_MATH_DEFINES

#include "ConfigLoader.h"

#include <cmath>   
#include <string>
#include <algorithm>
#include <vector>

void transmit(std::vector<double> &data, inData &in, std::vector<int> &message);

bool validateMessageData(const std::vector<int> &messageIn, const inData &configIn); 
void transmitASK(std::vector<double> &carrierOut, inData &configIn, std::vector<int> &messageIn);
void transmitFSK(std::vector<double> &carrierOut, inData &configIn, std::vector<int> &messageIn);
void transmitPSK(std::vector<double> &carrierOut, inData &configIn, std::vector<int> &messageIn);

#endif // !TransmitSystem_h
