/* **************************************************
�ļ�����    Vector.h
�ļ����ã�  ʸ���ļ�
�������ڣ�  2019/10/2
���ߣ�     ��Сƽ
*************************************************** */
#ifndef __VECTOR_H__
#define __VECTOR_H__

#include "stdio.h"

/************* �ṹ������ ***************/
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

/************* �������� ***************/
_VECTOR_STR Vector_Init(double x, double y, double z, double w);

#endif

