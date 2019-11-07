/* **************************************************
文件名：    OrbitalElements.c
文件作用：  轨道计算基础文件
创建日期：  2019/10/2
作者：     王小平
*************************************************** */
#include "OrbitalElements.h"

void OrbitalElements(_TLE_STR tle, _ORBITALELEMENTS_STR *orbital_elements_str)
{
	/*
	 * extract and format tle data
	 */
	orbital_elements_str->mean_anomoly_ = DegreesToRadians(tle.mean_anomaly_);
	orbital_elements_str->ascending_node_ = DegreesToRadians(tle.right_ascending_node_);
	orbital_elements_str->argument_perigee_ = DegreesToRadians(tle.argument_perigee_);
	orbital_elements_str->eccentricity_ = tle.eccentricity_;
	orbital_elements_str->inclination_ = DegreesToRadians(tle.inclination_);
	orbital_elements_str->mean_motion_ = tle.mean_motion_ * kTWOPI / kMINUTES_PER_DAY;
	orbital_elements_str->bstar_ = tle.bstar_;
	orbital_elements_str->epoch_ = tle.epoch_;

	/*
	 * recover original mean motion (xnodp) and semimajor axis (aodp)
	 * from input elements
	 */
	const double a1 = pow(kXKE / orbital_elements_str->mean_motion_, kTWOTHIRD);
	const double cosio = cos(orbital_elements_str->inclination_);
	const double theta2 = cosio * cosio;
	const double x3thm1 = 3.0 * theta2 - 1.0;
	const double eosq = orbital_elements_str->eccentricity_ * orbital_elements_str->eccentricity_;
	const double betao2 = 1.0 - eosq;
	const double betao = sqrt(betao2);
	const double temp = (1.5 * kCK2) * x3thm1 / (betao * betao2);
	const double del1 = temp / (a1 * a1);
	const double a0 = a1 * (1.0 - del1 * (1.0 / 3.0 + del1 * (1.0 + del1 * 134.0 / 81.0)));
	const double del0 = temp / (a0 * a0);

	orbital_elements_str->recovered_mean_motion_ = orbital_elements_str->mean_motion_ / (1.0 + del0);
	/*
	 * alternative way to calculate
	 * doesnt affect final results
	 * recovered_semi_major_axis_ = pow(XKE / RecoveredMeanMotion(), TWOTHIRD);
	 */
	orbital_elements_str->recovered_semi_major_axis_ = a0 / (1.0 - del0);

	/*
	 * find perigee and period
	 */
	orbital_elements_str->perigee_ = (orbital_elements_str->recovered_semi_major_axis_ * (1.0 - orbital_elements_str->eccentricity_) - kAE) * kXKMPER;
	orbital_elements_str->period_ = kTWOPI / orbital_elements_str->recovered_mean_motion_;
}
//
///*
//	 * XMO
//	 */
//double MeanAnomoly(_ORBITALELEMENTS_STR* orbital_elements_str)
//{
//	return orbital_elements_str->mean_anomoly_;
//}
//
///*
// * XNODEO
// */
//double AscendingNode(_ORBITALELEMENTS_STR *orbital_elements_str) 
//{
//	return orbital_elements_str->ascending_node_;
//}
//
///*
// * OMEGAO
// */
//double ArgumentPerigee(_ORBITALELEMENTS_STR *orbital_elements_str) 
//{
//	return orbital_elements_str->argument_perigee_;
//}
//
///*
// * EO
// */
//double Eccentricity(_ORBITALELEMENTS_STR *orbital_elements_str) 
//{
//	return orbital_elements_str->eccentricity_;
//}
//
///*
// * XINCL
// */
//double Inclination(_ORBITALELEMENTS_STR *orbital_elements_str) 
//{
//	return orbital_elements_str->inclination_;
//}
//
///*
// * XNO
// */
//double MeanMotion(_ORBITALELEMENTS_STR *orbital_elements_str) 
//{
//	return orbital_elements_str->mean_motion_;
//}
//
///*
// * BSTAR
// */
//double BStar(_ORBITALELEMENTS_STR *orbital_elements_str) 
//{
//	return orbital_elements_str->bstar_;
//}
//
///*
// * AODP
// */
//double RecoveredSemiMajorAxis(_ORBITALELEMENTS_STR *orbital_elements_str) 
//{
//	return orbital_elements_str->recovered_semi_major_axis_;
//}
//
///*
// * XNODP
// */
//double RecoveredMeanMotion(_ORBITALELEMENTS_STR *orbital_elements_str) 
//{
//	return orbital_elements_str->recovered_mean_motion_;
//}
//
///*
// * PERIGE
// */
//double Perigee(_ORBITALELEMENTS_STR *orbital_elements_str) 
//{
//	return orbital_elements_str->perigee_;
//}
//
///*
// * Period in minutes
// */
//double Period(_ORBITALELEMENTS_STR *orbital_elements_str) 
//{
//	return orbital_elements_str->period_;
//}
//
///*
// * EPOCH
// */
////DateTime Epoch(_ORBITALELEMENTS_STR *orbital_elements_str) 
////{
////	return epoch_;
////}
