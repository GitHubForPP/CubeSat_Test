/* **************************************************
文件名：    TimeSpan.c
文件作用：  时间数据文件
创建日期：  2019/10/2
作者：     王小平
*************************************************** */
#include "TimeSpan.h"

//static const unsigned long TicksPerMillisecond = 1000LL;
//static const unsigned long TicksPerMicrosecond = 1LL;
//
//static const unsigned long UnixEpoch = 62135596800000000LL;
//
//static const unsigned long MaxValueTicks = 315537897599999999LL;
//
//// 1582-Oct-15
//static const unsigned long GregorianStart = 49916304000000000LL;

unsigned long TimeSpan_CalculateTicks(int days,
	int hours,
	int minutes,
	int seconds)
{
	unsigned long ticks = 0;
	ticks = days * TicksPerDay +
		(hours * 3600LL + minutes * 60LL + seconds) * TicksPerSecond;

	return ticks;
}
