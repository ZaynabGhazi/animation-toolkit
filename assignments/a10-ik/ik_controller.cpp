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

  Joint *j3 = skeleton.getByID(jointid);
  if (!j3->getParent())
  {
    std::cout << "Warning: solveIKAnalytic() needs joint "
                 "with parent and grandparent\n";
    return false;
  }

  Joint *j2 = j3->getParent();
  if (!j2->getParent())
  {
    std::cout << "Warning: solveIKAnalytic() needs joint "
                 "with parent and grandparent\n";
    return false;
  }
  Joint *j1 = j2->getParent();
  // from lecture notes
  // PART1
  float l1 = length(j2->getGlobalTranslation() - j1->getGlobalTranslation());
  float l2 = length(j3->getGlobalTranslation() - j2->getGlobalTranslation());
  float r = length(goalPos - j1->getGlobalTranslation());
  float cos_phi = (r * r - l1 * l1 - l2 * l2) / (-2.0f * l1 * l2);
  cos_phi > 1 ? cos_phi = 1 : cos_phi = (cos_phi < -1 ? -1 : cos_phi);
  float phi = acos(cos_phi);
  float theta_2z = phi - M_PI;
  vec3 limbDir = normalize(j2->getLocalTranslation());
  vec3 axis = cross(limbDir, vec3(0, 0, -1));
  if (limbDir[1] < 0)
    axis = cross(limbDir, vec3(0, 0, 1));
  quat R21 = angleAxis(theta_2z, axis);
  Transform T21(R21, j2->getLocalTranslation());
  j2->setLocal2Parent(T21);
  skeleton.fk();
  // PART2: Method 2
  vec3 p3 = j3->getGlobalTranslation();
  vec3 r_ = p3 - j1->getGlobalTranslation();
  vec3 e_ = goalPos - p3;
  vec3 r_x_e = cross(r_, e_);
  float phi_ = atan2(length(r_x_e), dot(r_, r_) + dot(r_, e_));
  vec3 axis_ = normalize(r_x_e);
  if (length(r_x_e) == 0)
    axis_ = vec3(0, 0, 1);
  axis_ = j1->getLocal2Global().inverse().transformVector(axis_);
  Transform T10 = Transform(angleAxis(phi_, axis_), vec3(0));
  j1->setLocal2Parent(j1->getLocal2Parent() * T10);
  skeleton.fk();
  if (length(goalPos - j3->getGlobalTranslation()) <= epsilon)
    return true;
  return false;
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
      if (length(r_x_e) == 0)
        axis = vec3(0, 0, 1);
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
