#include "ik_controller.h"
#include <cmath>
#include <iostream>

using namespace atk;
using namespace glm;
using namespace std;

bool IKController::solveIKAnalytic(Skeleton &skeleton,
                                   int jointid, const vec3 &goalPos, float epsilon)
{
  if (jointid == -1)
    return true;

  Joint *ankle = skeleton.getByID(jointid);
  if (!ankle->getParent())
  {
    std::cout << "Warning: solveIKAnalytic() needs joint "
                 "with parent and grandparent\n";
    return false;
  }

  Joint *knee = ankle->getParent();
  if (!knee->getParent())
  {
    std::cout << "Warning: solveIKAnalytic() needs joint "
                 "with parent and grandparent\n";
    return false;
  }

  Joint *hip = knee->getParent();

  // TODO: Your code here
  return true;
}

bool IKController::solveIKCCD(Skeleton &skeleton, int jointid,
                              const vec3 &goalPos, const std::vector<Joint *> &chain,
                              float threshold, int maxIters, float nudgeFactor)
{
  // There are no joints in the IK chain for manipulation

  if (chain.size() == 0)
    return true;

  bool reached = false;
  int num_iterations = 0;
  vec3 p3 = skeleton.getByID(jointid)->getGlobalTranslation();
  float distance_from_trgt = length(goalPos - p3);
  while (distance_from_trgt > threshold && num_iterations < maxIters)
  {
    num_iterations++;
    // nudge chain
    for (int k = 0; k < chain.size(); k++)
    {
      Joint *current = chain.at(k);
      vec3 r_ = p3 - current->getGlobalTranslation();
      vec3 e_ = goalPos - p3;
      vec3 r_x_e = cross(r_, e_);
      float delta_phi = nudgeFactor * atan2(length(r_x_e), dot(r_, r_) + dot(r_, e_));
      vec3 axis = normalize(r_x_e);
      if (length(r_x_e)==0) axis = vec3(0,0,1);
      axis = current->getLocal2Global().inverse().transformVector(axis);
      Transform rotate(angleAxis(delta_phi, axis), vec3(0));
      current->setLocal2Parent(current->getLocal2Parent() * rotate);
      skeleton.fk();
      p3 = skeleton.getByID(jointid)->getGlobalTranslation();
      distance_from_trgt = length(goalPos - p3);
      if (distance_from_trgt <= threshold)
        reached = true;
    }
  }
  return reached;
}
