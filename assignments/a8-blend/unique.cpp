#include "atk/toolkit.h"
#include "atkui/framework.h"
#include "atkui/skeleton_drawer.h"
#include <algorithm>
#include <string>

using namespace atk;
using namespace atkui;
using namespace glm;

class Unique : public atkui::Framework
{
public:
   Unique() : atkui::Framework(atkui::Perspective) {}
   virtual ~Unique() {}

   void setup()
   {
      _alpha = 0.5;
      BVHReader reader;
      reader.load("../motions/Beta/samba_dancing.bvh", _skeleton, _motion1);
      reader.load("../motions/Beta/walking.bvh", _skeleton, _motion2);
      reader.load("../motions/Beta/left_strafe_walking.bvh", _skeleton, _motion3);
      reader.load("../motions/Beta/walking.bvh", _skeleton, _motion4);

      _blend_1 = blend(_motion1, _motion2, _alpha);
      _blend_2 = blend(_motion3, _motion4, _alpha);
      _blend_f = blend(_blend_1, _blend_2, _alpha);
   }

   Motion blend(const Motion &m1, const Motion &m2, double alpha)
   {
      Motion blend;
      blend.setFramerate(m1.getFramerate());
      float duration = m1.getDuration() * (1 - alpha) + m2.getDuration() * alpha;
      float delta_t = 1 / m1.getFramerate();
      for (float t = 0.0; t < duration; t += delta_t)
      {
         blend.appendKey(Pose::Lerp(m1.getValue(t, true), m2.getValue(t, true), alpha));
      }

      blend.appendKey(m1.getKey(0)); // placeholder
      return blend;
   }

   void scene()
   {
      drawMotion(_blend_1, vec3(-150, 0, 0));
      drawMotion(_blend_2, vec3(150, 0, 0));
      drawMotion(_blend_f, vec3(0, 0, 0));
      drawText(std::to_string(_alpha), 10, 15);
   }

   void drawMotion(const Motion &m, const vec3 &offset)
   {
      double t = elapsedTime() * 0.5;
      double time = m.getNormalizedDuration(t) * m.getDuration();
      m.update(_skeleton, time, false);

      SkeletonDrawer drawer;
      drawer.pos = offset;
      drawer.draw(_skeleton, *this);
   }

   void keyUp(int key, int mods)
   {
      if (key == GLFW_KEY_UP)
      {
         _alpha = std::min(_alpha + 0.05, 1.0);
         _blend_1 = blend(_motion1, _motion2, _alpha);
         _blend_2 = blend(_motion3, _motion4, _alpha);
         _blend_f = blend(_blend_1, _blend_2, _alpha);
      }
      else if (key == GLFW_KEY_DOWN)
      {
         _alpha = std::max(_alpha - 0.05, 0.0);
         _blend_1 = blend(_motion1, _motion2, _alpha);
         _blend_2 = blend(_motion3, _motion4, _alpha);
         _blend_f = blend(_blend_1, _blend_2, _alpha);
      }
   }

protected:
   Skeleton _skeleton;
   Motion _blend_1;
   Motion _blend_2;
   Motion _blend_f;
   Motion _motion1;
   Motion _motion2;
   Motion _motion3;
   Motion _motion4;
   double _alpha;
};

int main(int argc, char **argv)
{
   Unique viewer;
   viewer.run();
   return 0;
}
