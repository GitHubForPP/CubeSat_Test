/* **************************************************
�ļ�����    eci.h
�ļ����ã�  �������ϵ�ļ�
�������ڣ�  2019/10/2
���ߣ�     ��Сƽ
*************************************************** */
#ifndef __ECI_H__
#define __ECI_H__

#include "stdio.h"
#include "Vector.h"

/************* �ṹ������ ***************/
typedef struct __ECI_STR {
	_VECTOR_STR m_position;
	_VECTOR_STR m_velocity;
	unsigned long dt;
}_ECI_STR;

/************* �������� ***************/
_ECI_STR Eci_Init(unsigned long dt, _VECTOR_STR position, _VECTOR_STR velocity);

#endif

