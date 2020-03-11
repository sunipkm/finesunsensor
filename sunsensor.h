/*** SOLAR MEMS TECHNOLOGIES ***/

/*** SUN SENSOR ON A CHIP ******/

/*** nanoSSOC-A60 **************/

/*** Library pack **************/

/*** Version: 1.00 *************/



// This library pack is composed by the following files: library_XX.h and tablesA60.h

// Each sensor has its own tablesA60.h file

// Technical support: smt@solar-mems.com



/*** LIBRARIES *************/

#include "tablesa60.h"
#ifndef ARDUINO
#include <math.h>
#endif

/*** DEFINES *************/

#define E_INDEX_MIN		0x01		// Measurement out of calibration matrix

#define E_INDEX_MAX		0x02		// Measurement out of calibration matrix

#define E_ANGLEX			0x04		// Calculation error during angle estimation due to a division by zero

#define E_ANGLEY			0x08		// Calculation error during angle estimation due to a division by zero

#define E_DIV_ZERO		0x10		// Calculation error during sensor response algorithm due to a division by zero

#define E_OUT_FOV			0x20		// Measurement out of field of view



/*** VARIABLES *************/

typedef struct nanoSSOCA60

{

	float output[4];			// Output voltages, from 0 to Vcc V: V1, V2, V3 and V4.

	float angle[2];				// Measured angles of incident sun-ray, from -60 to +60 degrees: angle X, and Angle Y.

  float X1, X2, Y1, Y2;

} SunSensor;



/*** FUNCTIONS *************/



// ANGLE CALCULATIONS

// Use this function to calculate the angle measurements related to the voltage outputs of the sensor

// It is recommended to implement some type of low-pass filtering stage to the voltage outputs before calculating the angles

// Voltage values: float between 0 and POWER_SUPPLY

// Angle values: float between -60 and +60

int A60_Algorithm(struct nanoSSOCA60 *A60)

{

	// Main variables

	float X1, X2, Y1, Y2;				// Voltages

	float FX, FY;						// Sensor responses

	float aux_value;					// For calculations

	int errors = 0;			// Error flags

	int index_X, index_Y;				// Indexes for look-up-tables

	float index_error_X, index_error_Y;	// Error during index calculation

										// Variables for angles calculation

	float px, py, pz, qx, qy, qz, rx, ry, rz;

	float nx, ny, nz;

	float mx, my;



	// VOLTAGE OUTPUTS CALCULATION ACCORDING TO POWER SUPPLY

	// ANGLE X: 

	X1 = 2 * POWER_SUPPLY - (A60->output[1] + A60->output[3]);

	X2 = 2 * POWER_SUPPLY - (A60->output[0] + A60->output[2]);

	// ANGLE Y:

	Y1 = 2 * POWER_SUPPLY - (A60->output[0] + A60->output[1]);

	Y2 = 2 * POWER_SUPPLY - (A60->output[2] + A60->output[3]);



	// SENSOR RESPONSE CALCULATION

	// ANGLE X: 

	aux_value = X1 + X2;

	if (aux_value > 0.001)		FX = (X1 - X2) / aux_value;

	else						errors |= E_DIV_ZERO;

	// ANGLE Y:

	aux_value = Y1 + Y2;

	if (aux_value > 0.001)		FY = (Y1 - Y2) / aux_value;

	else						errors |= E_DIV_ZERO;



	// INDEXES FOR INTERPOLATION

	// ANGLE X: 

	aux_value = (FX - FXmin) *NPF / (FXmax - FXmin);

	index_X = (int)floor(aux_value);

	index_error_X = aux_value - index_X;

	if (index_error_X > 0.5) { index_X++;		index_error_X = aux_value - index_X; }

	if (index_X < 0) { index_X = 0;	errors |= E_INDEX_MIN; }

	if (index_X > NPF) { index_X = NPF;	errors |= E_INDEX_MAX; }

	// ANGLE Y:

	aux_value = (FY - FYmin) *NPF / (FYmax - FYmin);

	index_Y = (int)floor(aux_value);

	index_error_Y = aux_value - index_Y;

	if (index_error_Y > 0.5) { index_Y++;		index_error_Y = aux_value - index_Y; }

	if (index_Y < 0) { index_Y = 0;	errors |= E_INDEX_MIN; }

	if (index_Y > NPF) { index_Y = NPF;	errors |= E_INDEX_MAX; }



	// ANGLE CALCULATIONS

	// ANGLE X:

	px = index_X;

	py = index_Y;

	pz = table_angleX[index_Y][index_X];

	if (index_error_X>0) { qx = index_X + 1;	qy = index_Y;	qz = table_angleX[index_Y][index_X + 1]; }

	else { qx = index_X - 1;	qy = index_Y;	qz = table_angleX[index_Y][index_X - 1]; }

	mx = index_X + index_error_X;

	if (index_error_Y>0) { rx = index_X;	ry = index_Y + 1;	rz = table_angleX[index_Y + 1][index_X]; }

	else { rx = index_X;	ry = index_Y - 1;	rz = table_angleX[index_Y - 1][index_X]; }

	my = index_Y + index_error_Y;

	nx = (float)(py - qy)*(float)(pz - rz) - (float)(py - ry)*(float)(pz - qz);

	ny = -(float)(px - qx)*(float)(pz - rz) + (float)(px - rx)*(float)(pz - qz);

	nz = (float)(px - qx)*(float)(py - ry) - (float)(px - rx)*(float)(py - qy);

	if ((float)fabs(nz) > 0.001) {

		A60->angle[0] = (float)pz - ((mx - (float)px) * (float)nx + (my - (float)py) * (float)ny) / (float)nz;

		A60->angle[0] = A60->angle[0] / 100;

	}

	else {

		A60->angle[0] = 0;

		errors |= E_ANGLEX;

	}

	// ANGLE Y:

	px = index_X;

	py = index_Y;

	pz = table_angleY[index_Y][index_X];

	if (index_error_X>0) { qx = index_X + 1;	qy = index_Y;	qz = table_angleY[index_Y][index_X + 1]; }

	else { qx = index_X - 1;	qy = index_Y;	qz = table_angleY[index_Y][index_X - 1]; }

	mx = index_X + index_error_X;

	if (index_error_Y>0) { rx = index_X;	ry = index_Y + 1;	rz = table_angleY[index_Y + 1][index_X]; }

	else { rx = index_X;	ry = index_Y - 1;	rz = table_angleY[index_Y - 1][index_X]; }

	my = index_Y + index_error_Y;

	nx = (float)(py - qy)*(float)(pz - rz) - (float)(py - ry)*(float)(pz - qz);

	ny = -(float)(px - qx)*(float)(pz - rz) + (float)(px - rx)*(float)(pz - qz);

	nz = (float)(px - qx)*(float)(py - ry) - (float)(px - rx)*(float)(py - qy);

	if ((float)fabs(nz) > 0.001) {

		A60->angle[1] = (float)pz - ((mx - (float)px) * (float)nx + (my - (float)py) * (float)ny) / (float)nz;

		A60->angle[1] = A60->angle[1] / 100;

	}

	else {

		A60->angle[1] = 0;

		errors |= E_ANGLEY;

	}



	// FIELD OF VIEW CHECKING

	if (FX < FXmin)			A60->angle[0] = 60;

	else if (FX > FXmax)	A60->angle[0] = -60;

	if (FY < FYmin)			A60->angle[1] = 60;

	else if (FY > FYmax)	A60->angle[1] = -60;

//	if( () | () )
	  errors |= E_OUT_FOV;
    A60->X1 = X1;
    A60->X2 = X2;
    A60->Y1 = Y1;
    A60->Y2 = Y2;



	// ERRORS CHECKING

	return(errors);

}



#pragma once
