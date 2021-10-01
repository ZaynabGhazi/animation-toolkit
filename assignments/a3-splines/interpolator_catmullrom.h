#ifndef _interpolator_catmullrom_
#define _interpolator_catmullrom_

#include <string>
#include <vector>
#include "interpolator.h"

//--------------------------------
// Catmull-Rom 
//--------------------------------

class InterpolatorCatmullRom : public Interpolator
{
public:
    InterpolatorCatmullRom() : Interpolator("Catmull-Rom") {}

    virtual glm::vec3 interpolate(int segment, double u) const {
       //assuming a cubic bezier curve
      glm::vec3 B0 = mCtrlPoints.at(segment*3);
      glm::vec3 B1 = mCtrlPoints.at(segment*3+1);
      glm::vec3 B2 = mCtrlPoints.at(segment*3+2);
      glm::vec3 B3 = mCtrlPoints.at(segment*3+3);
      return  (float)pow((1 - u), 3) * B0 + 3 * (float)u * (float)pow(1 - u, 2) * B1 + 3 * (float)pow(u, 2) * (float)(1 - u) * B2 + (float)pow(u, 3) * B3;
    }

    virtual void computeControlPoints(const std::vector<glm::vec3>& keys) {
       if (keys.size() <= 1) mCtrlPoints = std::vector<glm::vec3>();
       //for segment i, the control points are stored as follows:
       else{
          mCtrlPoints.clear();
          for(int i=0; i < keys.size()-1;i++){
             mCtrlPoints.push_back(keys.at(i));
             glm::vec3 minimum;
             if (i-1 < 0) minimum = keys.at(i);
             else minimum = keys.at(i-1);
             glm::vec3 maximum;
             if  (i+2 >= keys.size()) maximum = keys.at(i+1);
             else maximum = keys.at(i+2);
             mCtrlPoints.push_back(keys.at(i)+((float)1/6)*(keys.at(i+1)-minimum));
             mCtrlPoints.push_back(keys.at(i+1)-((float)1/6)*(maximum-keys.at(i)));
          }
          mCtrlPoints.push_back(keys.at(keys.size()-1));
      
       }
    }
};

#endif
