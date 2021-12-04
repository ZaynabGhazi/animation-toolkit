#include "atk/toolkit.h"
#include "atkui/framework.h"
#include "cyclops.h"
#include <algorithm>
#include <string>

using namespace atk;
using namespace glm;
using namespace std;

class ALooker : public atkui::Framework
{
public:
   ALooker() : atkui::Framework(atkui::Perspective) {}
   virtual ~ALooker() {}

   void setup()
   {
      BVHReader reader;
      reader.load("../motions/Beta/samba_dancing.bvh", _skeleton, motion);
      motion.update(_skeleton, 0);

      _drawer.color = vec3(1, 0, 0);
      _head = _skeleton.getByName("Beta:Head");
   }

   void reset(Joint *head)
   {
      head->setLocalRotation(IdentityQ);
      head->fk();
   }

   void lookAtTarget(Joint *head, const vec3 &target)
   {
      // Follow lecture 19
      vec3 desired_direction = target - head->getGlobalTranslation();
      vec3 Z = normalize(desired_direction);
      vec3 X = cross(vec3(0, 1, 0), Z);
      vec3 Y = cross(Z, X);
      mat3 rot = mat3(normalize(X), normalize(Y), normalize(Z));
      // convert to local
      quat local_rot = head->getLocal2Global().inverse().r() * quat(rot);
      head->setLocalRotation(local_rot * head->getLocalRotation());
      head->fk();
   }

   void scene()
   {
      motion.update(_skeleton, elapsedTime());
      float r = 100;
      float angle = elapsedTime();
      _target = vec3(r * cos(angle), r * sin(angle) + r, 150);

      reset(_head);
      lookAtTarget(_head, _target);
      _drawer.draw(_skeleton, *this);

      vec3 globalHeadPos = _head->getGlobalTranslation();
      vec3 globalForward = _head->getGlobalRotation() * vec3(0, 0, 1);

      setColor(vec3(0, 0, 1));
      setColor(vec3(1,0,0));
      drawTorus(_target,10);
   }

   Motion motion;
   Cyclops _drawer;
   Skeleton _skeleton;
   Joint *_head;
   vec3 _target;
};

int main(int argc, char **argv)
{
   ALooker viewer;
   viewer.run();
   return 0;
}
