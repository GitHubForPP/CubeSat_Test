/* **************************************************
�ļ�����    Vector.c
�ļ����ã�  ʸ���ļ�
�������ڣ�  2019/10/2
���ߣ�     ��Сƽ
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
