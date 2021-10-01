#ifndef _interpolator_linear_H_
#define _interpolator_linear_H_

#include <string>
#include <vector>
#include "interpolator.h"

//--------------------------------
// Linear 
//--------------------------------

class InterpolatorLinear : public Interpolator
{
public:
    InterpolatorLinear() : Interpolator("Linear") {}
    virtual glm::vec3 interpolate(int segment, double u) const {
       glm::vec3 B0 = mCtrlPoints.at(segment);
       glm::vec3 B1 = mCtrlPoints.at(segment+1);
       return B0 * (float)(1 - u) + B1 *(float) u;
    }

    virtual void computeControlPoints(const std::vector<glm::vec3>& keys) {
       if (keys.size() <= 1) mCtrlPoints = std::vector<glm::vec3>();
       else{
          mCtrlPoints.clear();
          mCtrlPoints = keys;
       }
    }
};

#endif
