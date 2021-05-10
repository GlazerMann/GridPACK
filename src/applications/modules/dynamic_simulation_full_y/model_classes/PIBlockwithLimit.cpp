/*
 *     Copyright (c) 2021 Battelle Memorial Institute
 *     Licensed under modified BSD License. A copy of this license can be found
 *     in the LICENSE file in the top level directory of this distribution.
 */
// -------------------------------------------------------------
/**
 * @file   PIBlockwithLimit.cpp
 * @author Renke Huang renke.huang@pnnl.gov
 * @Last modified:   May 7, 2021
 * 
 * @brief  
 * 
 * 
 */

#include <vector>
#include <iostream>
#include <cstdio>
#include <cstring>

#include "boost/smart_ptr/shared_ptr.hpp"
#include "PIBlockwithLimit.hpp"

/**
 * Basic constructor
 */
gridpack::dynamic_simulation::PIBlockwithLimit::PIBlockwithLimit(void)
{

}

/**
 * Basic destructor
 */
gridpack::dynamic_simulation::PIBlockwithLimit::~PIBlockwithLimit(void)
{
}

double gridpack::dynamic_simulation::PIBlockwithLimit::init(double dOut, double Kp, double Ki, double Max, double Min)
{
	Kp = Kp;
	Ki = Ki;
	Max = Max;
	Min = Min;
	double dIn;
	if (abs(Ki) > 0.000000001){
		x0 = dOut;
		dIn = 0.0;
	}else{
		x0 = 0.0;
		dIn = dOut/Kp;
	}
	
	x1 = x0;
	dx0 = 0.0;
	dx1 = 0.0;
	
	return dIn;
}

double gridpack::dynamic_simulation::PIBlockwithLimit::predictor(double In, double t_inc, bool flag)
{
    if (!flag) {
		x0 = x1;
	}  
	
	if (abs(Ki) < 0.000000001){
		dx0 = 0.0;
	}else{
		double TempMax = Max - Kp*In;
		double TempMin = Min - Kp*In;
		
		if (x0 > TempMax) {
			x0 = TempMax;
		}
		if (x0 < TempMin) {
			x0 = TempMin;
		}
		
		dx0 = Ki*In;
		
		if (dx0>0.0 && x0>=TempMax) {
			dx0 = 0.0;
		}
		
		if (dx0<0.0 && x0<=TempMin) {
			dx0 = 0.0;
		}

	} // finished dx compuatation
	
	//compute output
	double dOut = In*Kp + x0;
	if ( dOut > Max ){
		dOut = Max;
	}
	if ( dOut < Min ){
		dOut = Min;
	}
	
	x1 = x0 + dx0 * t_inc;
	
	return dOut;
}

double gridpack::dynamic_simulation::PIBlockwithLimit::corrector(double In, double t_inc, bool flag)
{
	if (abs(Ki) < 0.000000001){
		dx1 = 0.0;
	}else{
		double TempMax = Max - Kp*In;
		double TempMin = Min - Kp*In;
		
		if (x1 > TempMax) {
			x1 = TempMax;
		}
		if (x1 < TempMin) {
			x1 = TempMin;
		}
		
		dx1 = Ki*In;
		
		if (dx1>0.0 && x1>=TempMax) {
			dx1 = 0.0;
		}
		
		if (dx1<0.0 && x1<=TempMin) {
			dx1 = 0.0;
		}

	} // finished dx compuatation
	
	//compute output
	double dOut = In*Kp + x1;
	if ( dOut > Max ){
		dOut = Max;
	}
	if ( dOut < Min ){
		dOut = Min;
	}
	
	x1 = x0 + (dx0 + dx1) / 2.0 * t_inc;
	
	return dOut;
}