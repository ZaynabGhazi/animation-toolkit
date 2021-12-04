#include "atk/toolkit.h"
#include "atkui/framework.h"
#include "cyclops.h"
#include "ik_controller.h"
#include <algorithm>
#include <string>

using namespace atk;
using namespace glm;
using namespace std;

class ADancer : public atkui::Framework
{
public:
   ADancer() : atkui::Framework(atkui::Perspective) {}
   virtual ~ADancer() {}

   void setup()
   {
      BVHReader reader;
      reader.load("../motions/Beta/idle.bvh", _skeleton, _motion);
      _drawer.color = vec3(1, 0, 0);
      _motion.update(_skeleton, 0);
      hip_pos = _skeleton.getRoot()->getGlobalTranslation();

      lhand_offset = _skeleton.getByName("Beta:LeftHand")->getGlobalTranslation();
      rhand_offset = _skeleton.getByName("Beta:RightHand")->getGlobalTranslation();

      lfoot_rot = _skeleton.getByName("Beta:LeftLeg")->getGlobalRotation();
      rfoot_rot = _skeleton.getByName("Beta:RightLeg")->getGlobalRotation();
      lfoot_trans = _skeleton.getByName("Beta:LeftLeg")->getGlobalTranslation();
      rfoot_trans = _skeleton.getByName("Beta:RightLeg")->getGlobalTranslation();
   }

   void update()
   {
      _motion.update(_skeleton, elapsedTime());

      IKController ik;
      // TODO: Your code here
      _skeleton.getRoot()->setLocalTranslation(hip_pos + vec3(15.0f * sin(5.0f * elapsedTime()),15.0f * sin(5.0f * elapsedTime()),0));
      _skeleton.fk();
      ik.solveIKAnalytic(_skeleton, _skeleton.getByName("Beta:LeftFoot")->getID(), lfoot_trans, 0.001f);
      ik.solveIKAnalytic(_skeleton, _skeleton.getByName("Beta:RightFoot")->getID(), rfoot_trans, 0.001f);
      _skeleton.getByName("Beta:LeftFoot")->setLocalRotation(_skeleton.getByName("Beta:LeftFoot")->getLocalRotation() * inverse(_skeleton.getByName("Beta:LeftFoot")->getGlobalRotation()) * lfoot_rot);
      _skeleton.getByName("Beta:RightFoot")->setLocalRotation(_skeleton.getByName("Beta:RightFoot")->getLocalRotation() * inverse(_skeleton.getByName("Beta:RightFoot")->getGlobalRotation()) * rfoot_rot);
      ik.solveIKAnalytic(_skeleton, _skeleton.getByName("Beta:LeftHand")->getID(), _lhandTarget, 0.001f);
      ik.solveIKAnalytic(_skeleton, _skeleton.getByName("Beta:RightHand")->getID(), _rhandTarget, 0.001f);
      _skeleton.fk();
   }

   void scene()
   {
      update();
      _drawer.draw(_skeleton, *this);
      setColor(vec3(0, 0, 1));
      // drawSphere(_lhandTarget, 10);
      // drawSphere(_rhandTarget, 10);

      _lhandTarget = vec3(0, 50 * sin(elapsedTime() * 5.0f), 0) + lhand_offset + vec3(0, 70, 10);
      _rhandTarget = vec3(0, 50 * cos(elapsedTime() * 5.0f), 0) + rhand_offset + vec3(0, 70, 10);
   }

protected:
   Cyclops _drawer;
   Skeleton _skeleton;
   Motion _motion;

   // Hand target positions
   vec3 _lhandTarget;
   vec3 _rhandTarget;
   vec3 rhand_offset;
   vec3 lhand_offset;
   vec3 hip_pos;
   // feet data
   quat rfoot_rot;
   quat lfoot_rot;
   vec3 rfoot_trans;
   vec3 lfoot_trans;
};

int main(int argc, char **argv)
{
   ADancer viewer;
   viewer.run();
   return 0;
}
