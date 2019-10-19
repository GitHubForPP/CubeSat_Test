/* **************************************************
文件名：    SGP4.h
文件作用：  SGP4简单摄动轨道推算算法文件
创建日期：  2019/10/2
作者：     王小平
*************************************************** */
#ifndef __SGP4_H__
#define __SGP4_H__

#include "stdio.h"
#include "OrbitalElements.h"
#include "eci.h"
#include "math.h"

/************* 结构体声明 ***************/
struct CommonConstants
{
	double cosio;
	double sinio;
	double eta;
	double t2cof;
	double a3ovk2;
	double x1mth2;
	double x3thm1;
	double x7thm1;
	double aycof;
	double xlcof;
	double xnodcf;
	double c1;
	double c4;
	double omgdot; // secular rate of omega (radians/sec)
	double xnodot; // secular rate of xnode (radians/sec)
	double xmdot;  // secular rate of xmo   (radians/sec)
};

struct NearSpaceConstants
{
	double c5;
	double omgcof;
	double xmcof;
	double delmo;
	double sinmo;
	double d2;
	double d3;
	double d4;
	double t3cof;
	double t4cof;
	double t5cof;
};

typedef struct __SGP4_STR {
	
	/*
	 * the constants used
	 */
	struct CommonConstants common_consts_;
	struct NearSpaceConstants nearspace_consts_;

	/*
	 * the orbit data
	 */
	_ORBITALELEMENTS_STR elements_;

	/*
	 * flags
	 */
	unsigned char use_simple_model_;
	unsigned char use_deep_space_;
}_SGP4_STR;

/************* 函数声明 ***************/
void SGP4_Reset(_SGP4_STR* sgp4_str);
void SGP4_Initialise(_SGP4_STR* sgp4_str, _TLE_STR tle_str);
void SGP4_FindPosition(_SGP4_STR *sgp4_str, _ECI_STR* eci_str, unsigned long now_t);

#endif



