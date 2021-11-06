#include <iostream>
#include "atkui/framework.h"
#include "atk/toolkit.h"

using namespace glm;
using namespace atk;

class Butterfly : public atkui::Framework
{
public:
   Butterfly() : atkui::Framework(atkui::Perspective)
   {
   }

   void setup()
   {
      Joint *body = new Joint("Body");
      body->setLocalTranslation(vec3(1, 2, 1) * 100.0f);
      body->setLocalRotation(glm::angleAxis(glm::radians<float>(45.0f), vec3(0, 1, 0)));
      skeleton.addJoint(body);

      Joint *lwing_a = new Joint("LWingA");
      lwing_a->setLocalTranslation(vec3(0.1, 0, 0) * 100.0f);
      skeleton.addJoint(lwing_a, body);
      Joint *lwing_b = new Joint("LWingB");
      lwing_b->setLocalTranslation(vec3(20, 0, 20) * 100.0f);
      skeleton.addJoint(lwing_b, body);

      Joint *rwing_a = new Joint("RWingA");
      rwing_a->setLocalTranslation(vec3(-0.1, 0, 0) * 100.0f);
      skeleton.addJoint(rwing_a, body);
      Joint *rwing_b = new Joint("RWingB");
      rwing_b->setLocalTranslation(vec3(-20, 0, 20) * 100.0f);
      skeleton.addJoint(rwing_b, body);

      skeleton.fk();
   }

   void scene()
   {
      Joint *body = skeleton.getByName("Body");
      body->setLocalTranslation(vec3(1 * sin(elapsedTime()), 2, 1) * 100.0f);

      Joint *lwing_a = skeleton.getByName("LWingA");
      lwing_a->setLocalRotation(glm::angleAxis(sin(elapsedTime()), vec3(0, 0, 1)));
      Joint *rwing_a = skeleton.getByName("RWingA");
      rwing_a->setLocalRotation(glm::angleAxis(-sin(elapsedTime()), vec3(0, 0, 1)));
      skeleton.fk();

      // attach geometry to skeleton
      Transform B = body->getLocal2Global();
      Transform LT = lwing_a->getLocal2Global();
      Transform RT = rwing_a->getLocal2Global();

      //draw axes
      if (show_axis)
      {
         push();
         transform(B);
         setColor(vec3(1, 0, 0));
         drawEllipsoid(vec3(0), vec3(1000, 0, 0), 4);
         setColor(vec3(0, 1, 0));
         drawEllipsoid(vec3(0), vec3(0, 1000, 0), 4);
         setColor(vec3(0, 0, 1));
         drawEllipsoid(vec3(0), vec3(0, 0, 1000), 4);
         pop();
      }

      // draw body
      Transform bodyGeometry(
          glm::angleAxis(glm::pi<float>() * 0.5f, vec3(1, 0, 0)), // rotation
          vec3(0), vec3(25, 200, 25));                            // position, scale

      Transform lwingGeometry(
          eulerAngleRO(XYZ, vec3(0, 0, 0)),
          vec3(-80, 0, 0),
          vec3(120, 20, 200));

      Transform rwingGeometry(
          eulerAngleRO(XYZ, vec3(0, 0, 0)),
          vec3(80, 0, 0),
          vec3(120, 20, 200));

      //2 new parts
      Transform lwingGeometry_(
          eulerAngleRO(XYZ, vec3(0, 0, 0)),
          vec3(-40, 0, 20),
          vec3(60, 50, 60));

      Transform rwingGeometry_(
          eulerAngleRO(XYZ, vec3(0, 0, 0)),
          vec3(40, 0, 20),
          vec3(60, 50, 60));

      setColor(pallet.at(0));
      push();
      transform(B * bodyGeometry);
      drawCylinder(vec3(0), 1);
      pop();

      setColor(pallet.at(1));
      push();
      transform(LT * lwingGeometry);
      drawTorus(vec3(0), 1);
      pop();

      setColor(pallet.at(2));
      push();
      transform(LT * lwingGeometry_);
      drawTorus(vec3(0), 1);
      pop();

      setColor(pallet.at(1));
      push();
      transform(RT * rwingGeometry);
      drawTorus(vec3(0), 1);
      pop();

      setColor(pallet.at(2));
      push();
      transform(RT * rwingGeometry_);
      drawTorus(vec3(0), 1);
      pop();
   }

   virtual void keyUp(int key, int mods)
   {
      if (key == GLFW_KEY_A)
      {
         show_axis = !show_axis;
      }
   }

private:
   Skeleton skeleton;
   bool show_axis = false;
   std::vector<vec3> pallet =
       {
           vec3(0, 165, 227) / 255.0f,
           vec3(141, 215, 191) / 255.0f,
           vec3(255, 150, 197) / 255.0f,
           vec3(255, 87, 104) / 255.0f,
           vec3(255, 162, 58) / 255.0f};
};

int main(int argc, char **argv)
{
   Butterfly viewer;
   viewer.run();
}
