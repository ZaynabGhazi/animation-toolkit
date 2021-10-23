#include <iostream>
#include <cmath>
#include "atkmath/constants.h"
#include "atkui/framework.h"
#include "atkmath/matrix3.h"
#include "atkmath/quaternion.h"
#include "atkmath/vector3.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

using atkmath::Deg2Rad;
using atkmath::Matrix3;
using atkmath::Quaternion;
using atkmath::Vector3;
using glm::vec3;

class Unique : public atkui::Framework
{
public:
   Unique() : atkui::Framework(atkui::Orthographic)
   {
   }

   virtual void setup()
   {
      rotation_axis = vec3(1,0,0);
      color = pallet[0];
      size = 200;
      angle = 0.0f;
      rate = (3.0f * M_PI) / 10.0f;
      X = 0.5 * width();
      Y = 0.5 * height();
   }

   virtual void scene()
   {
      if (angle >=  (9.0f * M_PI) / 10.0f ){
         angle =0.0f;
         color = pallet.operator[](drand48()*pallet.size());
         size = drand48()* 300+50;
         rotation_axis = vec3(drand48(),drand48(),drand48());
      }
      setColor(color);
      push();
      translate(vec3(X, Y, 0));
      rotate(angle, rotation_axis);
      drawCone(vec3(0), size);
      pop();
      angle = angle + rate * dt();
   }

private:
   float angle;
   float rate;
   vec3 rotation_axis;
   vec3 color;
   int size;
   float X, Y;
   //https://www.color-hex.com/color-palette/82379
   std::vector<vec3> pallet =
       {
           vec3(0, 165, 227) / 255.0f,
           vec3(141, 215, 191) / 255.0f,
           vec3(255, 150, 197) / 255.0f,
           vec3(255, 87, 104) / 255.0f,
           vec3(255, 162, 58) / 255.0f};
};
;

int main(int argc, char **argv)
{
   srand(time(NULL));
   Unique viewer;
   viewer.run();
   return 0;
}