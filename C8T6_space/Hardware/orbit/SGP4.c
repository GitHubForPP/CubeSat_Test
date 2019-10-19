/* **************************************************
文件名：    SGP4.c
文件作用：  SGP4简单摄动轨道推算算法文件
创建日期：  2019/10/2
作者：     王小平
*************************************************** */
#include "SGP4.h"

/*	void SGP4_Initialise(_SGP4_STR* sgp4_str, _TLE_STR* tle_str)
	SGP4初始化
*/
void SGP4_Initialise(_SGP4_STR* sgp4_str, _TLE_STR tle_str)
{
	OrbitalElements(tle_str, &sgp4_str->elements_);
	/*
	 * reset all constants etc
	 */
	SGP4_Reset(sgp4_str);

	/*
	 * error checks
	 */
	if (sgp4_str->elements_.eccentricity_ < 0.0 || sgp4_str->elements_.eccentricity_ > 0.999)
	{
		printf("Eccentricity out of range\r\n");
	}

	if (sgp4_str->elements_.inclination_ < 0.0 || sgp4_str->elements_.inclination_ > kPI)
	{
		printf("Inclination out of range");
	}

	sgp4_str->common_consts_.cosio = cos(sgp4_str->elements_.inclination_);
	sgp4_str->common_consts_.sinio = sin(sgp4_str->elements_.inclination_);
	const double theta2 = sgp4_str->common_consts_.cosio * sgp4_str->common_consts_.cosio;
	sgp4_str->common_consts_.x3thm1 = 3.0 * theta2 - 1.0;
	const double eosq = sgp4_str->elements_.eccentricity_ * sgp4_str->elements_.eccentricity_;
	const double betao2 = 1.0 - eosq;
	const double betao = sqrt(betao2);

	if (sgp4_str->elements_.period_ >= 225.0)
	{
		sgp4_str->use_deep_space_ = 1;
	}
	else
	{
		sgp4_str->use_deep_space_ = 0;
		sgp4_str->use_simple_model_ = 0;
		/*
		 * for perigee less than 220 kilometers, the simple_model flag is set
		 * and the equations are truncated to linear variation in sqrt a and
		 * quadratic variation in mean anomly. also, the c3 term, the
		 * delta omega term and the delta m term are dropped
		 */
		if (sgp4_str->elements_.perigee_ < 220.0)
		{
			sgp4_str->use_simple_model_ = 1;
		}
	}

	/*
	 * for perigee below 156km, the values of
	 * s4 and qoms2t are altered
	 */
	double s4 = kS;
	double qoms24 = kQOMS2T;
	if (sgp4_str->elements_.perigee_ < 156.0)
	{
		s4 = sgp4_str->elements_.perigee_ - 78.0;
		if (sgp4_str->elements_.perigee_ < 98.0)
		{
			s4 = 20.0;
		}
		qoms24 = pow((120.0 - s4) * kAE / kXKMPER, 4.0);
		s4 = s4 / kXKMPER + kAE;
	}

	/*
	 * generate constants
	 */
	const double pinvsq = 1.0
		/ (sgp4_str->elements_.recovered_semi_major_axis_
			* sgp4_str->elements_.recovered_semi_major_axis_
			* betao2 * betao2);
	const double tsi = 1.0 / (sgp4_str->elements_.recovered_semi_major_axis_ - s4);
	sgp4_str->common_consts_.eta = sgp4_str->elements_.recovered_semi_major_axis_
		* sgp4_str->elements_.eccentricity_ * tsi;
	const double etasq = sgp4_str->common_consts_.eta * sgp4_str->common_consts_.eta;
	const double eeta = sgp4_str->elements_.eccentricity_ * sgp4_str->common_consts_.eta;
	const double psisq = fabs(1.0 - etasq);
	const double coef = qoms24 * pow(tsi, 4.0);
	const double coef1 = coef / pow(psisq, 3.5);
	const double c2 = coef1 * sgp4_str->elements_.recovered_mean_motion_
		* (sgp4_str->elements_.recovered_semi_major_axis_
			* (1.0 + 1.5 * etasq + eeta * (4.0 + etasq))
			+ 0.75 * kCK2 * tsi / psisq * sgp4_str->common_consts_.x3thm1
			* (8.0 + 3.0 * etasq * (8.0 + etasq)));
	sgp4_str->common_consts_.c1 =sgp4_str->elements_.bstar_ * c2;
	sgp4_str->common_consts_.a3ovk2 = -kXJ3 / kCK2 * kAE * kAE * kAE;
	sgp4_str->common_consts_.x1mth2 = 1.0 - theta2;
	sgp4_str->common_consts_.c4 = 2.0 * sgp4_str->elements_.recovered_mean_motion_
		* coef1 * sgp4_str->elements_.recovered_semi_major_axis_ * betao2
		* (sgp4_str->common_consts_.eta * (2.0 + 0.5 * etasq) + sgp4_str->elements_.eccentricity_
			* (0.5 + 2.0 * etasq)
			- 2.0 * kCK2 * tsi / (sgp4_str->elements_.recovered_semi_major_axis_ * psisq)
			* (-3.0 * sgp4_str->common_consts_.x3thm1 * (1.0 - 2.0 * eeta + etasq
				* (1.5 - 0.5 * eeta))
				+ 0.75 * sgp4_str->common_consts_.x1mth2 * (2.0 * etasq - eeta *
				(1.0 + etasq)) * cos(2.0 * sgp4_str->elements_.argument_perigee_)));
	const double theta4 = theta2 * theta2;
	const double temp1 = 3.0 * kCK2 * pinvsq * sgp4_str->elements_.recovered_mean_motion_;
	const double temp2 = temp1 * kCK2 * pinvsq;
	const double temp3 = 1.25 * kCK4 * pinvsq * pinvsq * sgp4_str->elements_.recovered_mean_motion_;
	sgp4_str->common_consts_.xmdot = sgp4_str->elements_.recovered_mean_motion_ + 0.5 * temp1 * betao *
		sgp4_str->common_consts_.x3thm1 + 0.0625 * temp2 * betao *
		(13.0 - 78.0 * theta2 + 137.0 * theta4);
	const double x1m5th = 1.0 - 5.0 * theta2;
	sgp4_str->common_consts_.omgdot = -0.5 * temp1 * x1m5th +
		0.0625 * temp2 * (7.0 - 114.0 * theta2 + 395.0 * theta4) +
		temp3 * (3.0 - 36.0 * theta2 + 49.0 * theta4);
	const double xhdot1 = -temp1 * sgp4_str->common_consts_.cosio;
	sgp4_str->common_consts_.xnodot = xhdot1 + (0.5 * temp2 * (4.0 - 19.0 * theta2) + 2.0 * temp3 *
		(3.0 - 7.0 * theta2)) * sgp4_str->common_consts_.cosio;
	sgp4_str->common_consts_.xnodcf = 3.5 * betao2 * xhdot1 * sgp4_str->common_consts_.c1;
	sgp4_str->common_consts_.t2cof = 1.5 * sgp4_str->common_consts_.c1;

	if (fabs(sgp4_str->common_consts_.cosio + 1.0) > 1.5e-12)
	{
		sgp4_str->common_consts_.xlcof = 0.125 * sgp4_str->common_consts_.a3ovk2 * sgp4_str->common_consts_.sinio * (3.0 + 5.0 * sgp4_str->common_consts_.cosio) / (1.0 + sgp4_str->common_consts_.cosio);
	}
	else
	{
		sgp4_str->common_consts_.xlcof = 0.125 * sgp4_str->common_consts_.a3ovk2 * sgp4_str->common_consts_.sinio * (3.0 + 5.0 * sgp4_str->common_consts_.cosio) / 1.5e-12;
	}

	sgp4_str->common_consts_.aycof = 0.25 * sgp4_str->common_consts_.a3ovk2 * sgp4_str->common_consts_.sinio;
	sgp4_str->common_consts_.x7thm1 = 7.0 * theta2 - 1.0;

	if (sgp4_str->use_deep_space_)
	{
		/*deepspace_consts_.gsto = sgp4_str->elements_.Epoch().ToGreenwichSiderealTime();

		DeepSpaceInitialise(eosq, sgp4_str->common_consts_.sinio, sgp4_str->common_consts_.cosio, betao,
			theta2, betao2,
			sgp4_str->common_consts_.xmdot, sgp4_str->common_consts_.omgdot, sgp4_str->common_consts_.xnodot);*/
	}
	else
	{
		double c3 = 0.0;
		if (sgp4_str->elements_.eccentricity_ > 1.0e-4)
		{
			c3 = coef * tsi * sgp4_str->common_consts_.a3ovk2 * sgp4_str->elements_.recovered_mean_motion_ * kAE *
				sgp4_str->common_consts_.sinio / sgp4_str->elements_.eccentricity_;
		}

		sgp4_str->nearspace_consts_.c5 = 2.0 * coef1 * sgp4_str->elements_.recovered_semi_major_axis_ * betao2 * (1.0 + 2.75 *
			(etasq + eeta) + eeta * etasq);
		sgp4_str->nearspace_consts_.omgcof = sgp4_str->elements_.bstar_ * c3 * cos(sgp4_str->elements_.argument_perigee_);

		sgp4_str->nearspace_consts_.xmcof = 0.0;
		if (sgp4_str->elements_.eccentricity_ > 1.0e-4)
		{
			sgp4_str->nearspace_consts_.xmcof = -kTWOTHIRD * coef * sgp4_str->elements_.bstar_ * kAE / eeta;
		}

		sgp4_str->nearspace_consts_.delmo = pow(1.0 + sgp4_str->common_consts_.eta * (cos(sgp4_str->elements_.mean_anomoly_)), 3.0);
		sgp4_str->nearspace_consts_.sinmo = sin(sgp4_str->elements_.mean_anomoly_);

		if (!sgp4_str->use_simple_model_)
		{
			const double c1sq = sgp4_str->common_consts_.c1 * sgp4_str->common_consts_.c1;
			sgp4_str->nearspace_consts_.d2 = 4.0 * sgp4_str->elements_.recovered_semi_major_axis_ * tsi * c1sq;
			const double temp = sgp4_str->nearspace_consts_.d2 * tsi * sgp4_str->common_consts_.c1 / 3.0;
			sgp4_str->nearspace_consts_.d3 = (17.0 * sgp4_str->elements_.recovered_semi_major_axis_ + s4) * temp;
			sgp4_str->nearspace_consts_.d4 = 0.5 * temp * sgp4_str->elements_.recovered_semi_major_axis_ *
				tsi * (221.0 * sgp4_str->elements_.recovered_semi_major_axis_ + 31.0 * s4) * sgp4_str->common_consts_.c1;
			sgp4_str->nearspace_consts_.t3cof = sgp4_str->nearspace_consts_.d2 + 2.0 * c1sq;
			sgp4_str->nearspace_consts_.t4cof = 0.25 * (3.0 * sgp4_str->nearspace_consts_.d3 + sgp4_str->common_consts_.c1 *
				(12.0 * sgp4_str->nearspace_consts_.d2 + 10.0 * c1sq));
			sgp4_str->nearspace_consts_.t5cof = 0.2 * (3.0 * sgp4_str->nearspace_consts_.d4 + 12.0 * sgp4_str->common_consts_.c1 *
				sgp4_str->nearspace_consts_.d3 + 6.0 * sgp4_str->nearspace_consts_.d2 * sgp4_str->nearspace_consts_.d2 + 15.0 *
				c1sq * (2.0 * sgp4_str->nearspace_consts_.d2 + c1sq));
		}
	}
}

_ECI_STR SGP4_CalculateFinalPositionVelocity(_SGP4_STR *sgp4_str,
	const double tsince,
	const double e,
	const double a,
	const double omega,
	const double xl,
	const double xnode,
	const double xincl,
	const double xlcof,
	const double aycof,
	const double x3thm1,
	const double x1mth2,
	const double x7thm1,
	const double cosio,
	const double sinio)
{
	const double beta2 = 1.0 - e * e;
	const double xn = kXKE / pow(a, 1.5);
	/*
	 * long period periodics
	 */
	const double axn = e * cos(omega);
	const double temp11 = 1.0 / (a * beta2);
	const double xll = temp11 * xlcof * axn;
	const double aynl = temp11 * aycof;
	const double xlt = xl + xll;
	const double ayn = e * sin(omega) + aynl;
	const double elsq = axn * axn + ayn * ayn;

	_VECTOR_STR position, velocity;

	if (elsq >= 1.0)
	{
		printf("Error: (elsq >= 1.0)\r\n");
	}

	/*
	 * solve keplers equation
	 * - solve using Newton-Raphson root solving
	 * - here capu is almost the mean anomoly
	 * - initialise the eccentric anomaly term epw
	 * - The fmod saves reduction of angle to +/-2pi in sin/cos() and prevents
	 * convergence problems.
	 */
	const double capu = fmod(xlt - xnode, kTWOPI);
	double epw = capu;

	double sinepw = 0.0;
	double cosepw = 0.0;
	double ecose = 0.0;
	double esine = 0.0;

	/*
	 * sensibility check for N-R correction
	 */
	const double max_newton_naphson = 1.25 * fabs(sqrt(elsq));

	unsigned char kepler_running = 1;

	for (int i = 0; i < 10 && kepler_running; i++)
	{
		sinepw = sin(epw);
		cosepw = cos(epw);
		ecose = axn * cosepw + ayn * sinepw;
		esine = axn * sinepw - ayn * cosepw;

		double f = capu - epw + esine;

		if (fabs(f) < 1.0e-12)
		{
			kepler_running = 0;
		}
		else
		{
			/*
			 * 1st order Newton-Raphson correction
			 */
			const double fdot = 1.0 - ecose;
			double delta_epw = f / fdot;

			/*
			 * 2nd order Newton-Raphson correction.
			 * f / (fdot - 0.5 * d2f * f/fdot)
			 */
			if (i == 0)
			{
				if (delta_epw > max_newton_naphson)
				{
					delta_epw = max_newton_naphson;
				}
				else if (delta_epw < -max_newton_naphson)
				{
					delta_epw = -max_newton_naphson;
				}
			}
			else
			{
				delta_epw = f / (fdot + 0.5 * esine * delta_epw);
			}

			/*
			 * Newton-Raphson correction of -F/DF
			 */
			epw += delta_epw;
		}
	}
	/*
	 * short period preliminary quantities
	 */
	const double temp21 = 1.0 - elsq;
	const double pl = a * temp21;

	if (pl < 0.0)
	{
		printf("Error: (pl < 0.0)\r\n");
	}

	const double r = a * (1.0 - ecose);
	const double temp31 = 1.0 / r;
	const double rdot = kXKE * sqrt(a) * esine * temp31;
	const double rfdot = kXKE * sqrt(pl) * temp31;
	const double temp32 = a * temp31;
	const double betal = sqrt(temp21);
	const double temp33 = 1.0 / (1.0 + betal);
	const double cosu = temp32 * (cosepw - axn + ayn * esine * temp33);
	const double sinu = temp32 * (sinepw - ayn - axn * esine * temp33);
	const double u = atan2(sinu, cosu);
	const double sin2u = 2.0 * sinu * cosu;
	const double cos2u = 2.0 * cosu * cosu - 1.0;

	/*
	 * update for short periodics
	 */
	const double temp41 = 1.0 / pl;
	const double temp42 = kCK2 * temp41;
	const double temp43 = temp42 * temp41;

	const double rk = r * (1.0 - 1.5 * temp43 * betal * x3thm1)
		+ 0.5 * temp42 * x1mth2 * cos2u;
	const double uk = u - 0.25 * temp43 * x7thm1 * sin2u;
	const double xnodek = xnode + 1.5 * temp43 * cosio * sin2u;
	const double xinck = xincl + 1.5 * temp43 * cosio * sinio * cos2u;
	const double rdotk = rdot - xn * temp42 * x1mth2 * sin2u;
	const double rfdotk = rfdot + xn * temp42 * (x1mth2 * cos2u + 1.5 * x3thm1);

	/*
	 * orientation vectors
	 */
	const double sinuk = sin(uk);
	const double cosuk = cos(uk);
	const double sinik = sin(xinck);
	const double cosik = cos(xinck);
	const double sinnok = sin(xnodek);
	const double cosnok = cos(xnodek);
	const double xmx = -sinnok * cosik;
	const double xmy = cosnok * cosik;
	const double ux = xmx * sinuk + cosnok * cosuk;
	const double uy = xmy * sinuk + sinnok * cosuk;
	const double uz = sinik * sinuk;
	const double vx = xmx * cosuk - cosnok * sinuk;
	const double vy = xmy * cosuk - sinnok * sinuk;
	const double vz = sinik * cosuk;
	/*
	 * position and velocity
	 */
	const double x = rk * ux * kXKMPER;
	const double y = rk * uy * kXKMPER;
	const double z = rk * uz * kXKMPER;
	position = Vector_Init(x, y, z, 0.0);
	const double xdot = (rdotk * ux + rfdotk * vx) * kXKMPER / 60.0;
	const double ydot = (rdotk * uy + rfdotk * vy) * kXKMPER / 60.0;
	const double zdot = (rdotk * uz + rfdotk * vz) * kXKMPER / 60.0;
	velocity = Vector_Init(xdot, ydot, zdot, 0.0);

	if (rk < 1.0)
	{
		/*throw DecayedException(
			elements_.Epoch().AddMinutes(tsince),
			position,
			velocity);*/
	}

	return Eci_Init((unsigned long)(sgp4_str->elements_.epoch_ + (tsince * TicksPerMinute)), position, velocity);
}

void SGP4_FindPositionSGP4(_SGP4_STR *sgp4_str, _ECI_STR* eci_str, double tsince)
{
	/*
	 * the final values
	 */
	double e;
	double a;
	double omega;
	double xl;
	double xnode;
	double xincl;

	/*
	 * update for secular gravity and atmospheric drag
	 */
	const double xmdf = sgp4_str->elements_.mean_anomoly_
		+ sgp4_str->common_consts_.xmdot * tsince;
	const double omgadf = sgp4_str->elements_.argument_perigee_
		+ sgp4_str->common_consts_.omgdot * tsince;
	const double xnoddf = sgp4_str->elements_.ascending_node_
		+ sgp4_str->common_consts_.xnodot * tsince;

	const double tsq = tsince * tsince;
	xnode = xnoddf + sgp4_str->common_consts_.xnodcf * tsq;
	double tempa = 1.0 - sgp4_str->common_consts_.c1 * tsince;
	double tempe = sgp4_str->elements_.bstar_ * sgp4_str->common_consts_.c4 * tsince;
	double templ = sgp4_str->common_consts_.t2cof * tsq;

	xincl = sgp4_str->elements_.inclination_;
	omega = omgadf;
	double xmp = xmdf;

	if (!sgp4_str->use_simple_model_)
	{
		const double delomg = sgp4_str->nearspace_consts_.omgcof * tsince;
		const double delm = sgp4_str->nearspace_consts_.xmcof
			* (pow(1.0 + sgp4_str->common_consts_.eta * cos(xmdf), 3.0)
				* -sgp4_str->nearspace_consts_.delmo);
		const double temp = delomg + delm;

		xmp += temp;
		omega -= temp;

		const double tcube = tsq * tsince;
		const double tfour = tsince * tcube;

		tempa = tempa - sgp4_str->nearspace_consts_.d2 * tsq - sgp4_str->nearspace_consts_.d3
			* tcube - sgp4_str->nearspace_consts_.d4 * tfour;
		tempe += sgp4_str->elements_.bstar_ * sgp4_str->nearspace_consts_.c5
			* (sin(xmp) - sgp4_str->nearspace_consts_.sinmo);
		templ += sgp4_str->nearspace_consts_.t3cof * tcube + tfour
			* (sgp4_str->nearspace_consts_.t4cof + tsince * sgp4_str->nearspace_consts_.t5cof);
	}

	a = sgp4_str->elements_.recovered_semi_major_axis_ * tempa * tempa;
	e = sgp4_str->elements_.eccentricity_ - tempe;
	xl = xmp + omega + xnode + sgp4_str->elements_.recovered_mean_motion_ * templ;

	/*
	 * fix tolerance for error recognition
	 */
	if (e <= -0.001)
	{
		printf("Error: (e <= -0.001)\r\n");
	}
	else if (e < 1.0e-6)
	{
		e = 1.0e-6;
	}
	else if (e > (1.0 - 1.0e-6))
	{
		e = 1.0 - 1.0e-6;
	}

	/*
	 * using calculated values, find position and velocity
	 * we can pass in constants from Initialise() as these dont change
	 */
	*eci_str =  SGP4_CalculateFinalPositionVelocity(sgp4_str, tsince, e,
		a, omega, xl, xnode,
		xincl, sgp4_str->common_consts_.xlcof, sgp4_str->common_consts_.aycof,
		sgp4_str->common_consts_.x3thm1, sgp4_str->common_consts_.x1mth2, sgp4_str->common_consts_.x7thm1,
		sgp4_str->common_consts_.cosio, sgp4_str->common_consts_.sinio);

}

void SGP4_FindPosition(_SGP4_STR *sgp4_str, _ECI_STR *eci_str, unsigned long now_t)
{
	SGP4_FindPositionSGP4(sgp4_str, eci_str, (double)(now_t - sgp4_str->elements_.epoch_)/ (double)TicksPerMinute);
}

void SGP4_Reset(_SGP4_STR* sgp4_str)
{
	sgp4_str->use_simple_model_ = 0;
	sgp4_str->use_deep_space_ = 0;

	/*common_consts_ = Empty_CommonConstants;
	nearspace_consts_ = Empty_NearSpaceConstants;
	deepspace_consts_ = Empty_DeepSpaceConstants;
	integrator_consts_ = Empty_IntegratorConstants;
	integrator_params_ = Empty_IntegratorParams;*/
}


