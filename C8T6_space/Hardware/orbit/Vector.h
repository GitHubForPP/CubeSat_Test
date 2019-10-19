/* **************************************************
文件名：    Vector.h
文件作用：  矢量文件
创建日期：  2019/10/2
作者：     王小平
*************************************************** */
#ifndef __VECTOR_H__
#define __VECTOR_H__

#include "stdio.h"

/************* 结构体声明 ***************/
typedef struct __VECTOR_STR {
	/** x value */
	double x;
	/** y value */
	double y;
	/** z value */
	double z;
	/** w value */
	double w;
}_VECTOR_STR;

/************* 函数声明 ***************/
_VECTOR_STR Vector_Init(double x, double y, double z, double w);

#endif

