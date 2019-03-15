/* 
 * File:   CMath.h
 * Author: sedamian
 *
 * Created on April 12, 2017, 12:49 AM
 */

#ifndef CMATH_H
#define	CMATH_H

#ifdef	__cplusplus
extern "C" {
#endif

/* module containing implementations of atan2 and euclidean norm */
#ifndef CMATH_H
#define CMATH_H
float arctangent2(float y, float x);
float enorm(float px, float py, float qx, float qy);
#endif


#ifdef	__cplusplus
}
#endif

#endif	/* CMATH_H */

