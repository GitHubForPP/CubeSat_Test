/* **************************************************
文件名：    eci.c
文件作用：  大地坐标系文件
创建日期：  2019/10/2
作者：     王小平
*************************************************** */
#include "eci.h"

_ECI_STR Eci_Init(unsigned long dt, _VECTOR_STR position, _VECTOR_STR velocity) {
	_ECI_STR eci_str;

	eci_str.dt = dt;
	eci_str.m_position = position;
	eci_str.m_velocity = velocity;

	return (eci_str);
}
