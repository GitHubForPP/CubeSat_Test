/* **************************************************
�ļ�����    TimeSpan.h
�ļ����ã�  ʱ�������ļ�
�������ڣ�  2019/10/2
���ߣ�     ��Сƽ
*************************************************** */
#ifndef __TIMESPAN_H__
#define __TIMESPAN_H__

#include "stdio.h"

#define  TicksPerDay		86400
#define  TicksPerHour		3600
#define  TicksPerMinute		60
#define  TicksPerSecond		1

/************* �ṹ������ ***************/
typedef struct __TIMESPAN_STR {
	unsigned long m_ticks;
}_TIMESPAN_STR;

unsigned long TimeSpan_CalculateTicks(int days,
	int hours,
	int minutes,
	int seconds);



#endif


