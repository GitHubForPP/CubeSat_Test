/* **************************************************
文件名：    tle.c
文件作用：  两行根式输入处理文件 
创建日期：  2019/10/1
作者：     王小平
*************************************************** */
#include "tle.h" 

//--------------------索引定义--------------------- 
#define TLE1_COL_NORADNUM  2
#define TLE1_LEN_NORADNUM  5
#define TLE1_COL_INTLDESC_A  9
#define TLE1_LEN_INTLDESC_A  2
//  #define TLE1_COL_INTLDESC_B  11
#define TLE1_LEN_INTLDESC_B  3
//  #define TLE1_COL_INTLDESC_C  14
#define TLE1_LEN_INTLDESC_C  3
#define TLE1_COL_EPOCH_A  18
#define TLE1_LEN_EPOCH_A  2
#define TLE1_COL_EPOCH_B  20
#define TLE1_LEN_EPOCH_B  12
#define TLE1_COL_MEANMOTIONDT2  33
#define TLE1_LEN_MEANMOTIONDT2  10
#define TLE1_COL_MEANMOTIONDDT6  44
#define TLE1_LEN_MEANMOTIONDDT6  8
#define TLE1_COL_BSTAR  53
#define TLE1_LEN_BSTAR  8
//  #define TLE1_COL_EPHEMTYPE  62
//  #define TLE1_LEN_EPHEMTYPE  1
//  #define TLE1_COL_ELNUM  64
//  #define TLE1_LEN_ELNUM  4

#define TLE2_COL_NORADNUM  2
#define TLE2_LEN_NORADNUM  5
#define TLE2_COL_INCLINATION  8
#define TLE2_LEN_INCLINATION  8
#define TLE2_COL_RAASCENDNODE  17
#define TLE2_LEN_RAASCENDNODE  8
#define TLE2_COL_ECCENTRICITY  26
#define TLE2_LEN_ECCENTRICITY  7
#define TLE2_COL_ARGPERIGEE  34
#define TLE2_LEN_ARGPERIGEE  8
#define TLE2_COL_MEANANOMALY  43
#define TLE2_LEN_MEANANOMALY  8
#define TLE2_COL_MEANMOTION  52
#define TLE2_LEN_MEANMOTION  11
#define TLE2_COL_REVATEPOCH  63
#define TLE2_LEN_REVATEPOCH  5

#define TLE_LEN_LINE_DATA  69
#define TLE_LEN_LINE_NAME  22
/******************************************************/

/************************* 函数实现 *************************/
/**
 * Initialise the tle object.
 * @exception TleException
 */
void Tle_Initialize(_TLE_STR *tle_str, char * line_one_, char * line_two_) {
	unsigned int sat_number_1 = 0;
	unsigned int sat_number_2 = 0;
	//char tle1_col_buff[TLE1_LEN_NORADNUM] = { 0 }, tle2_col_buff[TLE2_LEN_NORADNUM] = {0};
	//char str_buff_temp[200] = {0};

	if (!Tle_IsValidLineLength(line_one_))
	{
		printf("Invalid length for line one\r\n");
	}

	if (!Tle_IsValidLineLength(line_two_))
	{
		printf("Invalid length for line two\r\n");
	}

	if (line_one_[0] != '1')
	{
		printf("Invalid line beginning for line one\r\n");
	}

	if (line_two_[0] != '2')
	{
		printf("Invalid line beginning for line two\r\n");
	}

	Tle_ExtractInteger(&line_one_[TLE1_COL_NORADNUM], TLE1_LEN_NORADNUM, &sat_number_1);
	Tle_ExtractInteger(&line_two_[TLE2_COL_NORADNUM], TLE2_LEN_NORADNUM, &sat_number_2);

	if (sat_number_1 != sat_number_2)
	{
		printf("Satellite numbers do not match\r\n");
	}

	tle_str->norad_number_ = sat_number_1;

	/*
	 * line 1
	 */
	unsigned int year = 0;
	double day = 0.0f;

	Tle_ExtractInteger(&line_one_[TLE1_COL_EPOCH_A], TLE1_LEN_EPOCH_A, &year);
	Tle_ExtractDouble(&line_one_[TLE1_COL_EPOCH_B], TLE1_LEN_EPOCH_B , 4, &day);
	Tle_ExtractDouble(&line_one_[TLE1_COL_MEANMOTIONDT2], TLE1_LEN_MEANMOTIONDT2, 2, &tle_str->mean_motion_dt2_);
	Tle_ExtractExponential(&line_one_[TLE1_COL_MEANMOTIONDDT6], TLE1_LEN_MEANMOTIONDDT6, &tle_str->mean_motion_ddt6_);
	Tle_ExtractExponential(&line_one_[TLE1_COL_BSTAR], TLE1_LEN_BSTAR, &tle_str->bstar_);

	/*
	 * line 2
	 */
	Tle_ExtractDouble(&line_two_[TLE2_COL_INCLINATION], TLE2_LEN_INCLINATION, 4, &tle_str->inclination_);
	Tle_ExtractDouble(&line_two_[TLE2_COL_RAASCENDNODE], TLE2_LEN_RAASCENDNODE, 4, &tle_str->right_ascending_node_);
	Tle_ExtractDouble(&line_two_[TLE2_COL_ECCENTRICITY], TLE2_LEN_ECCENTRICITY, -1, &tle_str->eccentricity_);
	Tle_ExtractDouble(&line_two_[TLE2_COL_ARGPERIGEE], TLE2_LEN_ARGPERIGEE, 4, &tle_str->argument_perigee_);
	Tle_ExtractDouble(&line_two_[TLE2_COL_MEANANOMALY], TLE2_LEN_MEANANOMALY, 4, &tle_str->mean_anomaly_);
	Tle_ExtractDouble(&line_two_[TLE2_COL_MEANMOTION], TLE2_LEN_MEANMOTION, 3, &tle_str->mean_motion_);
	Tle_ExtractInteger(&line_two_[TLE2_COL_REVATEPOCH], TLE2_LEN_REVATEPOCH, &tle_str->orbit_number_);

	if (year < 57)
		year += 2000;
	else
		year += 1900;

	printf("year:%d\r\n", year);

	tle_str->epoch_ = DataTime_Initialise2(year, day);

}

/**
 * Check
 * @param str The string to check
 * @returns Whether true of the string has a valid length
 */
unsigned char Tle_IsValidLineLength(char *string)
{
	return strlen(string) == TLE_LEN_LINE_DATA ? 1 : 0;
}

/**
 * Convert a string containing an integer
 * @param[in] str The string to convert
 * @param[in] str The len
 * @param[out] val The result
 * @exception TleException on conversion error
 */
void Tle_ExtractInteger(char* str, unsigned int len, unsigned int *val)
{
	unsigned char found_digit = 0;
	unsigned int temp = 0, i;

	for (i = 0; i < len; ++i)
	{
		if ((*(str + i) >= 0x30) && (*(str + i) <= 0x39))
		{
			found_digit = 1;
			temp = (temp * 10) + (*(str + i) - '0');
		}
		else if (found_digit)
		{
			printf("Unexpected non digit\r\n");
			return;
		}
		else if (*(str + i) != ' ')
		{
			printf("Invalid character\r\n");
			return;
		}
	}

	if (!found_digit)
	{
		*val = 0;
	}
	else
	{
		*val = temp;
	}
}

/**
 * Convert a string containing an exponential
 * @param[in] str The string to convert
 * @param[in] str The len
 * @param[out] val The result
 * @exception TleException on conversion error
 */
void Tle_ExtractExponential(char* str, unsigned int len, double *val)
{
	char temp[50] = {0};
	unsigned int i = 0, cnt = 0;

	for (i = 0; i != len; ++i)
	{
		if (i == 0)
		{
			if (*(str + i) == '-' || *(str + i) == '+' || *(str + i) == ' ')
			{
				if (*(str + i) == '-')
				{
					temp[cnt++] = *(str + i);
				}
				temp[cnt++] = '0';
				temp[cnt++] = '.';
			}
			else
			{
				printf("Invalid sign\r\n");
				return;
			}
		}
		else if (i == len - 2)
		{
			if (*(str + i) == '-' || *(str + i) == '+')
			{
				temp[cnt++] = 'e';
				temp[cnt++] = *(str + i);
			}
			else
			{
				printf("Invalid exponential sign\r\n");
				return;
			}
		}
		else
		{
			if ((*(str + i) >= 0x30) && (*(str + i) <= 0x39))
			{
				temp[cnt++] = *(str + i);
			}
			else
			{
				printf("Invalid digit\r\n");
				return;
			}
		}
	}

	temp[cnt++] = 0;

	*val = atof(temp);
}

/**
 * Convert a string containing an double
 * @param[in] str The string to convert
 * @param[in] str The len
 * @param[in] point_pos The position of the decimal point. (-1 if none)
 * @param[out] val The result
 * @exception TleException on conversion error
 */
void Tle_ExtractDouble(char* str, unsigned int len, int point_pos, double* val)
{
	char temp[20] = {0};
	unsigned char found_digit = 0;
	unsigned int i = 0, cnt = 0;
	unsigned char done = 0;

	for (i = 0; i < len; ++i)
	{
		/*
		 * integer part
		 */
		if (point_pos >= 0 && i < (unsigned int)(point_pos - 1))
		{
			if (i == 0)
			{
				if (*(str+i) == '-' || *(str+i) == '+')
				{
					/*
					 * first character could be signed
					 */
					temp[cnt++] = *(str+i);
					done = 1;
				}
			}

			if (!done)
			{
				if ((*(str + i) >= 0x30) && (*(str + i) <= 0x39))
				{
					found_digit = 1;
					temp[cnt++] = *(str + i);
				}
				else if (found_digit)
				{
					printf("Unexpected non digit\r\n");
				}
				else if (*(str+i) != ' ')
				{
					printf("Invalid character\r\n");
				}
			}
		}
		/*
		 * decimal point
		 */
		else if (point_pos >= 0 && i == (point_pos - 1))
		{
			if (cnt == 0)
			{
				/*
				 * integer part is blank, so add a '0'
				 */
				temp[cnt++] = '0';
			}

			if (*(str+i) == '.')
			{
				/*
				 * decimal point found
				 */
				temp[cnt++] = *(str+i);
			}
			else
			{
				printf("Failed to find decimal point\r\n");
			}
		}
		/*
		 * fraction part
		 */
		else
		{
			if (i == 0 && point_pos == -1)
			{
				/*
				 * no decimal point expected, add 0. beginning
				 */
				temp[cnt++] = '0';
				temp[cnt++] = '.';
			}

			/*
			 * should be a digit
			 */
			if ((*(str + i) >= 0x30) && (*(str + i) <= 0x39))
			{
				temp[cnt++] = *(str+i);
			}
			else
			{
				printf("Invalid digit\r\n");
			}
		}
	}

	temp[cnt++] = 0;

	*val = atof(temp);
}


