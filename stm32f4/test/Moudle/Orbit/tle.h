/* **************************************************
�ļ�����    tle.h
�ļ����ã�  ���и�ʽ���봦���ļ� 
�������ڣ�  2019/10/1
���ߣ�     ��Сƽ
*************************************************** */
#ifndef __TLE_H__
#define __TLE_H__

#include <stdio.h>
#include "string.h"
#include <stdlib.h>
#include "DataTime.h"

typedef struct __TLE_STR {
	double mean_motion_dt2_;
	double mean_motion_ddt6_;
	double bstar_;
	double inclination_;
	double right_ascending_node_;
	double eccentricity_;
	double argument_perigee_;
	double mean_anomaly_;
	double mean_motion_;
	unsigned int norad_number_;
	unsigned int orbit_number_;

	unsigned long epoch_;
}_TLE_STR;

/************* �������� ***************/

void Tle_Initialize(_TLE_STR* tle_str, char* line_one_, char* line_two_);
unsigned char Tle_IsValidLineLength(char* string);
void Tle_ExtractInteger(char* str, unsigned int len, unsigned int* val);
void Tle_ExtractExponential(char* str, unsigned int len, double* val);
void Tle_ExtractDouble(char* str, unsigned int len, int point_pos, double* val);

#endif


