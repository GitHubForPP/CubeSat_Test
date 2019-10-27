/* **************************************************
�ļ�����    OrbitalElements.h
�ļ����ã�  �����������ļ�
�������ڣ�  2019/10/2
���ߣ�     ��Сƽ
*************************************************** */
#ifndef __ORBITALELEMENTS_H__
#define __ORBITALELEMENTS_H__

#include "stdio.h"
#include "tle.h" 
#include "util.h"

/**************�ṹ�嶨��**************/
typedef struct __ORBITALELEMENTS_STR {
	double mean_anomoly_;
	double ascending_node_;
	double argument_perigee_;
	double eccentricity_;
	double inclination_;
	double mean_motion_;
	double bstar_;
	double recovered_semi_major_axis_;
	double recovered_mean_motion_;
	double perigee_;
	double period_;

	unsigned long epoch_;
}_ORBITALELEMENTS_STR;

/************* �������� ***************/
void OrbitalElements(_TLE_STR tle, _ORBITALELEMENTS_STR* orbital_elements_str);

#endif


