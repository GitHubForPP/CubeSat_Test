/* **************************************************
文件名：    DataTime.h
文件作用：  时间计算文件
创建日期：  2019/10/2
作者：     王小平
*************************************************** */
#ifndef __DATATIME_H__
#define __DATATIME_H__

#include "stdio.h"
#include "TimeSpan.h"

/************************* 结构体声明 *************************/
typedef struct __DATATIME_STR {
	unsigned long m_encoded;
}_DATATIME_STR;

/************************* 函数声明 *************************/
double AbsoluteDays_1(unsigned int year, double doy);
int AbsoluteDays_2(int year, int month, int day);
unsigned long DataTime_Initialise(
	int year,
	int month,
	int day,
	int hour,
	int minute,
	int second);
unsigned long DataTime_Initialise2(
	int year,
	double day);
void FromTicks(int* year, int* month, int* day, unsigned long m_encoded);
int Year(unsigned long t);

#endif


