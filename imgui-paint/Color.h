#ifndef COLOR_H
#define COLOR_H

#include <cmath>

class Color {
public:

    float r;
    float g;
    float b;

    Color( float r = 0, float g = 0, float b = 0)
      : r( r ), g( g ), b( b ) { };

    inline Color scale( float s ) {
        r *= s; g *= s; b *= s;
        return *this;
    }

    // @fairlight1337/hsvrgb-cpp
    static Color ColorFromHSV(float H, float S, float V) {
        
       
       
        float s = S/100;
        float v = V/100;
        float C = s*v;
        float X = C*(1-abs(fmod(H/60.0, 2)-1));
        float m = v-C;
        float rf,gf,bf;
        if(H >= 0 && H < 60){
            rf = C,gf = X,bf = 0;
        }
        else if(H >= 60 && H < 120){
            rf = X,gf = C,bf = 0;
        }
        else if(H >= 120 && H < 180){
            rf = 0,gf = C,bf = X;
        }
        else if(H >= 180 && H < 240){
            rf = 0,gf = X,bf = C;
        }
        else if(H >= 240 && H < 300){
            rf = X,gf = 0,bf = C;
        }
        else{
            rf = C,gf = 0,bf = X;
        }

        rf = (rf+m);
        gf = (gf+m);
        bf = (bf+m);
        

        return Color(rf, gf, bf);

    }


};

#endif