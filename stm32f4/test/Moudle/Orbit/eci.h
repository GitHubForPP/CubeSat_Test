/* **************************************************
文件名：    eci.h
文件作用：  大地坐标系文件
创建日期：  2019/10/2
作者：     王小平
*************************************************** */
#ifndef __ECI_H__
#define __ECI_H__

#include "stdio.h"
#include "Vector.h"

/************* 结构体声明 ***************/
typedef struct __ECI_STR {
	_VECTOR_STR m_position;
	_VECTOR_STR m_velocity;
	unsigned long dt;
}_ECI_STR;

/************* 函数声明 ***************/
_ECI_STR Eci_Init(unsigned long dt, _VECTOR_STR position, _VECTOR_STR velocity);

#endif

