/* **************************************************
文件名：    globals.h
文件作用：  常用计算常量文件
创建日期：  2019/10/2
作者：     王小平
*************************************************** */
#ifndef __GLOBALS_H__
#define __GLOBALS_H__

#include "stdio.h"
#include "math.h"


#define kAE (1.0)
#define kQ0 (120.0)
#define kS0 (78.0)
#define kMU (398600.8)
#define kXKMPER (6378.135)
#define kXJ2 (1.082616e-3)
#define kXJ3 (-2.53881e-6)
#define kXJ4 (-1.65597e-6)

/*
 * alternative XKE
 * affects final results
 * aiaa-2006-6573
 * const double kXKE = 60.0 / sqrt(kXKMPER * kXKMPER * kXKMPER / kMU);
 * dundee
 * const double kXKE = 7.43669161331734132e-2;
 */
#define kXKE (60.0 / sqrt(kXKMPER * kXKMPER * kXKMPER / kMU))
#define kCK2 (0.5 * kXJ2 * kAE * kAE)
#define kCK4 (-0.375 * kXJ4 * kAE * kAE * kAE * kAE)

/*
 * alternative QOMS2T
 * affects final results
 * aiaa-2006-6573
 * #define QOMS2T   (pow(((Q0 - S0) / XKMPER), 4.0))
 * dundee
 * #define QOMS2T   (1.880279159015270643865e-9)
 */
#define kQOMS2T (pow(((kQ0 - kS0) / kXKMPER), 4.0))

#define kS (kAE * (1.0 + kS0 / kXKMPER))
#define kPI (3.14159265358979323846264338327950288419716939937510582)
#define kTWOPI (2.0 * kPI)
#define kTWOTHIRD (2.0 / 3.0)
#define kTHDT (4.37526908801129966e-3)
/*
 * earth flattening
 */
#define kF (1.0 / 298.26)
/*
 * earth rotation per sideral day
 */
#define kOMEGA_E (1.00273790934)
#define kAU (1.49597870691e8)

#define kSECONDS_PER_DAY (86400.0)
#define kMINUTES_PER_DAY (1440.0)
#define kHOURS_PER_DAY (24.0)

// Jan 1.0 1900 = Jan 1 1900 00h UTC
#define kEPOCH_JAN1_00H_1900 (2415019.5)

// Jan 1.5 1900 = Jan 1 1900 12h UTC
#define kEPOCH_JAN1_12H_1900 (2415020.0)

// Jan 1.5 2000 = Jan 1 2000 12h UTC
#define kEPOCH_JAN1_12H_2000 (2451545.0)


#endif
