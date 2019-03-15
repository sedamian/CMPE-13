/*
 * File:   CMath.c
 * Author: sedamian
 *
 * Created on April 12, 2017, 1:44 AM
 */


#include "xc.h"

#include "CMath.h"




#define QX 1.5
#define QY 2.8
#define PX -2.3
#define PY 8.6
#define DX (QX-PX)
#define DY (QY-PY)




int main(void) {
    BOARD_Init();
    {

        /******************************** Your custom code goes below here *******************************/

        
        
        
        
        
        
        
        
        
        float arctangent2(float y, float x);
        float enorm(float px, float py, float qx, float qy);
        
        double mathHypot=hypotf(DX,DY);
        printf("Hypot returned %f and should return %f\r\n",mathHypot,6.9340);
        
        double mathAtan2=atan2(QX,QY);
        printf("Atan2 returned %f and should return %f\r\n",mathAtan2,.4918);
          


        while (1);
    }
    
    
   
}


