/* **************************************************
文件名：    TimeSpan.h
文件作用：  时间数据文件
创建日期：  2019/10/2
作者：     王小平
*************************************************** */
#ifndef __TIMESPAN_H__
#define __TIMESPAN_H__

#include "stdio.h"

#define  TicksPerDay		86400
#define  TicksPerHour		3600
#define  TicksPerMinute		60
#define  TicksPerSecond		1

/************* 结构体声明 ***************/
typedef struct __TIMESPAN_STR {
	unsigned long m_ticks;
}_TIMESPAN_STR;

unsigned long TimeSpan_CalculateTicks(int days,
	int hours,
	int minutes,
	int seconds);



#endif


