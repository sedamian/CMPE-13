/*
 * File:   CMath.c
 * Author: sedamian
 *
 * Created on April 12, 2017, 1:44 AM
 */


#include "xc.h"

#include "CMath.h"



float myabsfun(float ckr){
    float ret;
        if (ckr < 0 ){
            ret = ckr * (-1);
        }
        else {
            ret = ckr;
        };
            
        return ret;
            
    
}

float enorm(float px, float py, float qx, float qy){
    
    float qpx = qx - px;
    float qpy = qy - py;
   
    
    float DX = myabsfun(qpx);
    float DY = myabsfun(qpy);
    
    float g;
    float e;
    float t;
    float r;
    float s;
    
    
    
    
    if (DX > DY){
        g = DX;
        e = DY;
    }
    else {
        g = DY;
        e = DX;
    }
    
    float ii;
    for (ii = 0; ii < 2 ; ii++ ){
        t = e/g;
        r = t*t;
        s = r/(4.0+r);
        g = g + (2*s*g);
        e = e*s;
        
    }
        return g;
}






float arctangent2(float y, float x){
    float ya = myabsfun(y);
    float theta;
    
    float r;
    if (x > 0){
        r = (x-ya)/(x+ya);
        theta = (0.1963*r*r*r) - (.9817*r) + (3.1415/4);
                
    }
    else {
        r = (x+ya)/(x-ya);
        theta = (0.1963*r*r*r) - (.9817*r) + (3.1415*3/4);
    }
    if (y<0){
        theta = -1 * theta;
    }
    return theta;
}




