/* **************************************************
文件名：    OrbitalElements.h
文件作用：  轨道计算基础文件
创建日期：  2019/10/2
作者：     王小平
*************************************************** */
#ifndef __ORBITALELEMENTS_H__
#define __ORBITALELEMENTS_H__

#include "stdio.h"
#include "tle.h" 
#include "util.h"

/**************结构体定义**************/
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

/************* 函数声明 ***************/
void OrbitalElements(_TLE_STR tle, _ORBITALELEMENTS_STR* orbital_elements_str);

#endif


