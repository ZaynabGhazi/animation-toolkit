#include "atkui/framework.h"

using namespace glm;
using namespace std;

/* draws a curve between a source and a target determined by the user and a sphere traversing it*/
class Unique : public atkui::Framework
{
public:
   //Utility functions:

   //generate curve point using control points:
   vec3 curve(float t, vec3 B0, vec3 B1, vec3 B2, vec3 B3)
   {
      return (float)pow((1 - t), 3) * B0 + 3 * t * (float)pow(1 - t, 2) * B1 + 3 * (float)pow(t, 2) * (1 - t) * B2 + (float)pow(t, 3) * B3;
   }

   //generate random vector in window scope:
   vec3 vector()
   {
      return vec3(rand() % (int)width(), rand() % (int)height(), 0);
   }

   Unique() : atkui::Framework(atkui::Orthographic)
   {
   }

   void setup()
   {
      source = vec3(0, 0, 0);
      target = source;
   }

   void scene()
   {
      if (begin)
      {
         setColor(vec3(1, 0, 0));
         //create curve
         int j = 0;
         for (float t = 0; t <= 1 && j < num_pts; t += (1 / (float)num_pts))
         {
            bezier[j] = curve(t, source, B1, B2, target);
            j++;
         }
         //draw curve
         for (int i = 0; i < num_pts; i++)
         {
            drawSphere(bezier[i], 2);
         }
         //draw sphere moving along curve
         setColor(vec3(1));
         drawSphere(curve(fmod(elapsedTime() / 5, 1), source, B1, B2, target), 10);
      }
   }

   void mouseDown(int button, int mods)
   {
      if (button == GLFW_MOUSE_BUTTON_LEFT)
      {
         begin = true;
         _mouseX = mousePosition().x;
         _mouseY = height() - mousePosition().y;
         source = target;
         target = vec3(_mouseX, _mouseY, 0);
      }
   }

private:
   vec3 source;
   vec3 target;
   int _mouseX;
   int _mouseY;
   const static int num_pts = 1000;
   vec3 bezier[num_pts];
   bool begin = false;
   vec3 B1 = vec3(150, 200, 0);
   vec3 B2 = vec3(250, 100, 0);
};

int main(int argc, char **argv)
{
   Unique viewer;
   viewer.run();
   return 0;
}