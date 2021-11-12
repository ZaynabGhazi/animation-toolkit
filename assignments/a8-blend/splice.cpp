#include "atk/toolkit.h"
#include "atkui/framework.h"
#include "atkui/skeleton_drawer.h"
#include <algorithm>
#include <string>

using namespace atk;
using namespace atkui;
using namespace glm;

class ASplice : public atkui::Framework
{
public:
   ASplice() : atkui::Framework(atkui::Perspective) {}
   virtual ~ASplice() {}

   void setup()
   {
      _alpha = 0;

      BVHReader reader;
      reader.load("../motions/Beta/walking.bvh", _skeleton, _lower);
      reader.load("../motions/Beta/gangnam_style.bvh", _skeleton, _upper);
      _splice = spliceUpperBody(_lower, _upper, _alpha);
   }

   Motion spliceUpperBody(const Motion &lower, const Motion &upper, float alpha)
   {
      Motion result;
      result.setFramerate(lower.getFramerate());
      for (int i = 0; i < lower.getNumKeys(); i++)
      {
         Pose current = lower.getKey(i);
         Pose splice = upper.getKey(i+120);
         Pose final = current;
         Joint *spine = _skeleton.getByName("Beta:Spine1");
         if (spine == NULL) std::cout<<"null spine"<<std::endl;
         for (int i = 0; i < current.jointRots.size(); i++)
         {
            bool upper_ = false;
            Joint *current_joint = _skeleton.getByID(i);
            Joint *parent = current_joint->getParent();
            while (parent != NULL && parent != _skeleton.getRoot())
            {
               if (parent->getID() == spine->getID())
               {
                  upper_ = true;
                  break;
               }
               parent = parent->getParent();
            }
            if (upper_)
               final.jointRots.at(i) = slerp(current.jointRots.at(i), splice.jointRots.at(i), alpha);
         }
         result.appendKey(final);
      }
      return result;
   }

   void scene()
   {
      _splice.update(_skeleton, elapsedTime() * 0.5);
      SkeletonDrawer drawer;
      drawer.draw(_skeleton, *this);
      drawText("alpha: " + std::to_string(_alpha), 10, 15);
   }

   void keyUp(int key, int mods)
   {
      if (key == GLFW_KEY_UP)
      {
         _alpha = std::min(_alpha + 0.05, 1.0);
         _splice = spliceUpperBody(_lower, _upper, _alpha);
      }
      else if (key == GLFW_KEY_DOWN)
      {
         _alpha = std::max(_alpha - 0.05, 0.0);
         _splice = spliceUpperBody(_lower, _upper, _alpha);
      }
   }

   Skeleton _skeleton;
   Motion _upper;
   Motion _lower;
   Motion _splice;
   float _alpha;
};

int main(int argc, char **argv)
{
   ASplice viewer;
   viewer.run();
   return 0;
}
