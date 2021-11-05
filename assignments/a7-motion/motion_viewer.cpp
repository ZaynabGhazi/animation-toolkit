#include <iostream>
#include "atkui/framework.h"
#include "atk/toolkit.h"

using namespace glm;
using namespace atk;

std::string filepath;

class MotionViewer : public atkui::Framework
{
public:
   MotionViewer() : atkui::Framework(atkui::Perspective)
   {
   }

   void setup()
   {
      BVHReader reader;
      reader.load(filepath, skeleton, motion);
      motion.update(skeleton, 0);
   }

   void scene()
   {
      if (!paused)
      {
         time += (timeScale*dt());
      }
      currentFrame = motion.getKeyID(time);
      motion.update(skeleton, time);

      setColor(vec3(0, 0, 0.8));
      for (int i = 0; i < skeleton.getNumJoints(); i++)
      {
         Joint *joint = skeleton.getByID(i);
         if (joint->getParent() == 0)
            continue;

         vec3 p1 = joint->getGlobalTranslation();
         vec3 p2 = joint->getParent()->getGlobalTranslation();
         drawEllipsoid(p1, p2, 5);
      }

      drawText(paused ? "Paused" : "Playing", 10, 15);
      drawText("Current frame: " + std::to_string(currentFrame), 10, 35);
      drawText("Time scale: " + std::to_string(timeScale), 10, 55);
   }

   virtual void keyUp(int key, int mods)
   {
      switch (key)
      {
      case GLFW_KEY_P:
         paused = !paused;
         break;
      case GLFW_KEY_0:
         time = 0;
         break;
      case GLFW_KEY_PERIOD:
         if (paused)
         {
            time += motion.getDeltaTime();
            (time > motion.getDuration()) ? time = 0 : time = time;
         }
         break;
      case GLFW_KEY_COMMA:
         if (paused)
         {
            time -= motion.getDeltaTime();
            (time < 0) ? time = motion.getDuration() : time = time;
         }
         break;
      case GLFW_KEY_RIGHT_BRACKET:
         timeScale *= 1.2;
         break;
      case GLFW_KEY_LEFT_BRACKET:
         timeScale /=1.2;
         /* Animation will be stuck at 0 at which point you can increment or uncomment below for minimal scale*/
         //(timeScale < 0.01) ? timeScale = 0.5: timeScale = timeScale;
         break;
      }
   }

private:
   Skeleton skeleton;
   Motion motion;

   float timeScale = 1.0f;
   int currentFrame = 0;
   bool paused = false;
   float time = 0;
};

int main(int argc, char **argv)
{
   MotionViewer viewer;
   if (argc > 1)
   {
      filepath = argv[1];
   }
   else
      filepath = "../motions/Beta/jump.bvh";
   viewer.run();
}
