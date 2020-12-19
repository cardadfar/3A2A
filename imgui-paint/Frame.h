#ifndef Frame_H
#define Frame_H

#include "Dot.h"
#include "Color.h"

#include <vector>
#include <map>



using namespace std;
typedef vector<Dot> Stroke;

class Frame {
public:

    void addDot(int s, Dot d) {
        strokes[s].push_back(d);
    }

    Stroke getStroke(int s) {
        return strokes[s];
    }

    void clearStroke(int s) {
        strokes[s].clear();
    }

    void setDepth(int s, float minD, float maxD) {

        int nDots = strokes[s].size();
        int i = 0;
        for(Stroke::iterator dot = strokes[s].begin(); dot != strokes[s].end(); dot++) {
            dot->z = minD * (i / static_cast<float>(nDots)) + maxD * ((nDots - i) / static_cast<float>(nDots));
            i++;
        }
    }

    void clearFrame() {
        for(map<int,Stroke>::iterator ptr = strokes.begin(); ptr != strokes.end(); ptr++) {
            ptr->second.clear();
        }
    }

private:
    map<int, Stroke> strokes;
    map<int, float> depths;
};

#endif