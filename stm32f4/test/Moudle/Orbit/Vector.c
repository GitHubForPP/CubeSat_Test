/* **************************************************
文件名：    Vector.c
文件作用：  矢量文件
创建日期：  2019/10/2
作者：     王小平
*************************************************** */
#include "Vector.h"

_VECTOR_STR Vector_Init(double x, double y, double z, double w) {
	_VECTOR_STR data = {0};

	data.x = x; 
	data.y = y; 
	data.z = z; 
	data.w = w;

	return (data);
}
